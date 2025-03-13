#include <stdio.h>
#include <stdlib.h>

#include "Loader.h"
#include "Vulkan.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"

typedef struct RenderData
{
	VkRenderPass renderpass;
	VkFramebuffer* framebuffers;
	mlx_context mlx;
	mlx_window win;
	mlx_image img;
} RenderData;

void VulkanRender(VkCommandBuffer cmd, void* param)
{
	RenderData* data = (RenderData*)param;

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	igNewFrame();

	igShowDemoWindow(NULL);

	igRender();
	ImDrawData* draw_data = igGetDrawData();

	VkRenderPassBeginInfo info = { 0 };
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = data->renderpass;
	info.framebuffer = data->framebuffers[GetMlxFuncs()->mlx_get_current_vk_swapchain_image_index(data->mlx, data->win)];
	info.renderArea.extent.width = GetMlxFuncs()->mlx_get_vk_swapchain_extent(data->mlx, data->win).width;
	info.renderArea.extent.height = GetMlxFuncs()->mlx_get_vk_swapchain_extent(data->mlx, data->win).height;
	GetVulkanFuncs()->vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_RenderDrawData(draw_data, cmd, VK_NULL_HANDLE);

	GetVulkanFuncs()->vkCmdEndRenderPass(cmd);
}

void WindowHook(int event, void* param)
{
	if(event == 0)
		mlx_loop_end((mlx_context)param);
}

void CheckVk(VkResult result)
{
	(void)result;
}

void MlxRender(void* param)
{
	static int i = 0;
	RenderData* data = (RenderData*)param;

	mlx_clear_window(data->mlx, data->win, (mlx_color){ .rgba = 0x334D4DFF });
	mlx_put_transformed_image_to_window(data->mlx, data->win, data->img, 220, 40, 0.5f, 0.5f, i);

	uint32_t color = 0;
	for(int j = 0; j < 400; j++)
	{
		mlx_pixel_put(data->mlx, data->win, j, j, (mlx_color){ .rgba = 0x0000FFFF + (color << 24) });
		mlx_pixel_put(data->mlx, data->win, 399 - j, j, (mlx_color){ .rgba = 0x0000FFFF });
		color += (color < 255);
	}
	i++;
	if(i > 360)
		i = 0;
}

int main(void)
{
	mlx_context mlx = mlx_init();

	mlx_window_create_info info = { 0 };
	info.title = "MLX Window";
	info.width = 1250;
	info.height = 720;
	info.is_resizable = false;
	mlx_window win = mlx_new_window(mlx, &info);

	LoadMlxBindings(mlx, GetMlxFuncs());
	LoadVulkanBindings(mlx, GetMlxFuncs(), GetVulkanFuncs());

	mlx_on_event(mlx, win, MLX_WINDOW_EVENT, WindowHook, mlx);
	GetMlxFuncs()->mlx_set_sdl_input_hook(mlx, (sdl_hook)ImGui_ImplSDL2_ProcessEvent);

	ImGuiContext* ctx = igCreateContext(NULL);
	igGetIO()->IniFilename = NULL;

	VkInstance instance = GetMlxFuncs()->mlx_get_vk_instance(mlx);
	VkPhysicalDevice physical_device = GetMlxFuncs()->mlx_get_vk_physical_device(mlx);
	VkDevice device = GetMlxFuncs()->mlx_get_vk_device(mlx);
	VkDescriptorPool pool = VulkanCreateDescriptorPool(mlx);
	VkRenderPass renderpass = VulkanCreateRenderPass(mlx, win);
	VkFramebuffer* framebuffers = VulkanCreateFramebuffers(mlx, win, renderpass);

	ImGui_ImplVulkan_LoadFunctions(ImGuiLoadVulkan, instance);

	VkSurfaceCapabilitiesKHR capabilities = { 0 };
	GetVulkanFuncs()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, GetMlxFuncs()->mlx_get_vk_surface(mlx, win), &capabilities);

	ImGui_ImplSDL2_InitForVulkan((SDL_Window*)GetMlxFuncs()->mlx_get_window_handle(mlx, win));
	ImGui_ImplVulkan_InitInfo init_info = { 0 };
		init_info.Instance = instance;
		init_info.PhysicalDevice = physical_device;
		init_info.Device = device;
		init_info.QueueFamily = GetMlxFuncs()->mlx_get_vk_graphics_queue_family(mlx);
		init_info.Queue = GetMlxFuncs()->mlx_get_vk_graphics_queue(mlx);
		init_info.DescriptorPool = pool;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.MinImageCount = capabilities.minImageCount;
		init_info.ImageCount = GetMlxFuncs()->mlx_get_vk_swapchain_image_count(mlx, win);
		init_info.CheckVkResultFn = CheckVk;
		init_info.RenderPass = renderpass;
	ImGui_ImplVulkan_Init(&init_info);

	RenderData data = { 0 };
	data.renderpass = renderpass;
	data.mlx = mlx;
	data.win = win;
	data.framebuffers = framebuffers;
	data.img = mlx_new_image_from_file(mlx, "MacroLibX/example/42_logo.png", NULL, NULL);

	mlx_add_loop_hook(mlx, MlxRender, &data);
	GetMlxFuncs()->mlx_add_pre_render_hook(mlx, win, VulkanRender, &data);
	mlx_loop(mlx);

	GetVulkanFuncs()->vkDeviceWaitIdle(device);

	for(int i = 0; i < GetMlxFuncs()->mlx_get_vk_swapchain_image_count(mlx, win); i++)
		GetVulkanFuncs()->vkDestroyFramebuffer(device, framebuffers[i], NULL);
	free(framebuffers);

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	igDestroyContext(ctx);

	mlx_destroy_window(mlx, win);

	GetVulkanFuncs()->vkDestroyDescriptorPool(device, pool, NULL);
	GetVulkanFuncs()->vkDestroyRenderPass(device, renderpass, NULL);

	mlx_destroy_image(mlx, data.img);
	mlx_destroy_context(mlx);

	return 0;
}
