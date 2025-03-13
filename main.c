#include <stdio.h>
#include <stdlib.h>
#include "mlx.h"
#include "mlx_extended.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan_core.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"

#define IM_UNUSED(_VAR)  ((void)(_VAR))
#define IM_ASSERT(_EXPR) assert(_EXPR)
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

typedef VkInstance (*PFN_mlx_get_vk_instance)(mlx_context);
typedef VkDevice (*PFN_mlx_get_vk_device)(mlx_context);
typedef VkPhysicalDevice (*PFN_mlx_get_vk_physical_device)(mlx_context);
typedef VkSurfaceKHR (*PFN_mlx_get_vk_surface)(mlx_context, mlx_window);
typedef VkQueue (*PFN_mlx_get_vk_graphics_queue)(mlx_context);
typedef unsigned int (*PFN_mlx_get_vk_graphics_queue_family)(mlx_context);
typedef mlx_function (*PFN_mlx_get_vk_fn)(mlx_context, const char*);
typedef unsigned int (*PFN_mlx_get_vk_swapchain_image_count)(mlx_context, mlx_window);
typedef VkImage (*PFN_mlx_get_vk_swapchain_image)(mlx_context, mlx_window, unsigned int);
typedef VkImageView (*PFN_mlx_get_vk_swapchain_image_view)(mlx_context, mlx_window, unsigned int);
typedef unsigned int (*PFN_mlx_get_current_vk_swapchain_image_index)(mlx_context, mlx_window);
typedef VkExtent2D (*PFN_mlx_get_vk_swapchain_extent)(mlx_context, mlx_window);
typedef VkFormat (*PFN_mlx_get_vk_swapchain_format)(mlx_context, mlx_window);
typedef void* (*PFN_mlx_get_window_handle)(mlx_context, mlx_window);
typedef void (*PFN_mlx_set_sdl_input_hook)(mlx_context, void(*f)(void*));
typedef void (*PFN_mlx_add_pre_render_hook)(mlx_context, mlx_window, void(*f)(VkCommandBuffer, void*), void* param);

typedef void(*sdl_hook)(void*);

#define DECLARE_FUNCTION(fn) PFN_##fn fn

typedef struct Mlx
{
	DECLARE_FUNCTION(mlx_get_vk_instance);
	DECLARE_FUNCTION(mlx_get_vk_physical_device);
	DECLARE_FUNCTION(mlx_get_vk_graphics_queue);
	DECLARE_FUNCTION(mlx_get_vk_graphics_queue_family);
	DECLARE_FUNCTION(mlx_get_vk_surface);
	DECLARE_FUNCTION(mlx_get_vk_device);
	DECLARE_FUNCTION(mlx_get_vk_fn);
	DECLARE_FUNCTION(mlx_get_window_handle);
	DECLARE_FUNCTION(mlx_get_vk_swapchain_image_count);
	DECLARE_FUNCTION(mlx_get_vk_swapchain_extent);
	DECLARE_FUNCTION(mlx_get_vk_swapchain_format);
	DECLARE_FUNCTION(mlx_get_vk_swapchain_image);
	DECLARE_FUNCTION(mlx_get_vk_swapchain_image_view);
	DECLARE_FUNCTION(mlx_get_current_vk_swapchain_image_index);
	DECLARE_FUNCTION(mlx_add_pre_render_hook);
	DECLARE_FUNCTION(mlx_set_sdl_input_hook);

	mlx_context mlx;
	mlx_window win;
} Mlx;

static Mlx mlx = { 0 };

typedef struct Vulkan
{
	#define VULKAN_GLOBAL_FUNCTION(fn)   PFN_##fn fn;
	#define VULKAN_INSTANCE_FUNCTION(fn) PFN_##fn fn;
	#define VULKAN_DEVICE_FUNCTION(fn)   PFN_##fn fn;
		#include "VulkanDefs.h"
	#undef VULKAN_GLOBAL_FUNCTION
	#undef VULKAN_INSTANCE_FUNCTION
	#undef VULKAN_DEVICE_FUNCTION

	VkRenderPass render_pass;
	VkFramebuffer* framebuffers;
} Vulkan;

static Vulkan vulkan = { 0 };

void render(VkCommandBuffer cmd, void* param)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	igNewFrame();

	igShowDemoWindow(NULL);

	igRender();
	ImDrawData* draw_data = igGetDrawData();

	VkRenderPassBeginInfo info = { 0 };
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = vulkan.render_pass;
	info.framebuffer = vulkan.framebuffers[mlx.mlx_get_current_vk_swapchain_image_index(mlx.mlx, mlx.win)];
	info.renderArea.extent.width = mlx.mlx_get_vk_swapchain_extent(mlx.mlx, mlx.win).width;
	info.renderArea.extent.height = mlx.mlx_get_vk_swapchain_extent(mlx.mlx, mlx.win).height;
	vulkan.vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_RenderDrawData(draw_data, cmd, VK_NULL_HANDLE);

	vulkan.vkCmdEndRenderPass(cmd);
}

void window_hook(int event, void* param)
{
	if(event == 0)
		mlx_loop_end(mlx.mlx);
}

PFN_vkVoidFunction ImGuiLoadVulkan(const char* fn, void* data)
{
	return vulkan.vkGetInstanceProcAddr((VkInstance)data, fn);
}

void checkVk(VkResult result)
{
	(void)result;
}

int main(void)
{
	mlx_context mlx_ctx = mlx_init();

	mlx_window_create_info info = { 0 };
	info.title = "MLX Window";
	info.width = 1250;
	info.height = 720;
	info.is_resizable = false;
	mlx_window win = mlx_new_window(mlx_ctx, &info);

	mlx.mlx = mlx_ctx;
	mlx.win = win;

	#define POPULATE_FUNCTION(fn) mlx.fn = (PFN_##fn)mlx_get_proc_addr(mlx_ctx, #fn)
	POPULATE_FUNCTION(mlx_get_vk_instance);
	POPULATE_FUNCTION(mlx_get_vk_physical_device);
	POPULATE_FUNCTION(mlx_get_vk_graphics_queue);
	POPULATE_FUNCTION(mlx_get_vk_graphics_queue_family);
	POPULATE_FUNCTION(mlx_get_vk_surface);
	POPULATE_FUNCTION(mlx_get_vk_device);
	POPULATE_FUNCTION(mlx_get_vk_fn);
	POPULATE_FUNCTION(mlx_get_window_handle);
	POPULATE_FUNCTION(mlx_get_vk_swapchain_image_count);
	POPULATE_FUNCTION(mlx_get_vk_swapchain_extent);
	POPULATE_FUNCTION(mlx_get_vk_swapchain_format);
	POPULATE_FUNCTION(mlx_get_vk_swapchain_image);
	POPULATE_FUNCTION(mlx_get_vk_swapchain_image_view);
	POPULATE_FUNCTION(mlx_get_current_vk_swapchain_image_index);
	POPULATE_FUNCTION(mlx_add_pre_render_hook);
	POPULATE_FUNCTION(mlx_set_sdl_input_hook);

	mlx_on_event(mlx_ctx, win, MLX_WINDOW_EVENT, window_hook, NULL);
	mlx.mlx_set_sdl_input_hook(mlx_ctx, (sdl_hook)ImGui_ImplSDL2_ProcessEvent);

	#define VULKAN_GLOBAL_FUNCTION(fn)   vulkan.fn = (PFN_##fn)mlx.mlx_get_vk_fn(mlx_ctx, #fn);
	#define VULKAN_INSTANCE_FUNCTION(fn) vulkan.fn = (PFN_##fn)mlx.mlx_get_vk_fn(mlx_ctx, #fn);
	#define VULKAN_DEVICE_FUNCTION(fn)   vulkan.fn = (PFN_##fn)mlx.mlx_get_vk_fn(mlx_ctx, #fn);
	#include "VulkanDefs.h"

	VkDescriptorPool pool = VK_NULL_HANDLE;

	VkDescriptorPoolSize pool_sizes[] = {
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = { 0 };
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	vulkan.vkCreateDescriptorPool(mlx.mlx_get_vk_device(mlx_ctx), &pool_info, NULL, &pool);

	ImGuiContext* ctx = igCreateContext(NULL);
	igGetIO()->IniFilename = NULL;

	ImGui_ImplVulkan_LoadFunctions(ImGuiLoadVulkan, mlx.mlx_get_vk_instance(mlx_ctx));

	VkAttachmentDescription attachment = { 0 };
	attachment.format = mlx.mlx_get_vk_swapchain_format(mlx_ctx, win);
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	VkAttachmentReference color_reference = { 0 };
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = { 0 };
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_reference;

	VkRenderPassCreateInfo renderpass_create_info = { 0 };
	renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderpass_create_info.attachmentCount = 1;
	renderpass_create_info.pAttachments = &attachment;
	renderpass_create_info.subpassCount = 1;
	renderpass_create_info.pSubpasses = &subpass;
	vulkan.vkCreateRenderPass(mlx.mlx_get_vk_device(mlx_ctx), &renderpass_create_info, NULL, &vulkan.render_pass);

	vulkan.framebuffers = (VkFramebuffer*)calloc(mlx.mlx_get_vk_swapchain_image_count(mlx_ctx, win), sizeof(VkFramebuffer));

	for(int i = 0; i < mlx.mlx_get_vk_swapchain_image_count(mlx_ctx, win); i++)
	{
		VkImageView view = mlx.mlx_get_vk_swapchain_image_view(mlx_ctx, win, i);
		VkFramebufferCreateInfo framebuffer_info = { 0 };
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = vulkan.render_pass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = &view;
		framebuffer_info.width = mlx.mlx_get_vk_swapchain_extent(mlx_ctx, win).width;
		framebuffer_info.height = mlx.mlx_get_vk_swapchain_extent(mlx_ctx, win).height;
		framebuffer_info.layers = 1;
		vulkan.vkCreateFramebuffer(mlx.mlx_get_vk_device(mlx_ctx), &framebuffer_info, NULL, &vulkan.framebuffers[i]);
	}

	VkSurfaceCapabilitiesKHR capabilities = { 0 };
	vulkan.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mlx.mlx_get_vk_physical_device(mlx_ctx), mlx.mlx_get_vk_surface(mlx_ctx, win), &capabilities);

	ImGui_ImplSDL2_InitForVulkan((SDL_Window*)mlx.mlx_get_window_handle(mlx_ctx, win));
	ImGui_ImplVulkan_InitInfo init_info = { 0 };
		init_info.Instance = mlx.mlx_get_vk_instance(mlx_ctx);
		init_info.PhysicalDevice = mlx.mlx_get_vk_physical_device(mlx_ctx);
		init_info.Device = mlx.mlx_get_vk_device(mlx_ctx);
		init_info.QueueFamily = mlx.mlx_get_vk_graphics_queue_family(mlx_ctx);
		init_info.Queue = mlx.mlx_get_vk_graphics_queue(mlx_ctx);
		init_info.DescriptorPool = pool;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.MinImageCount = capabilities.minImageCount;
		init_info.ImageCount = mlx.mlx_get_vk_swapchain_image_count(mlx_ctx, win);
		init_info.CheckVkResultFn = checkVk;
		init_info.RenderPass = vulkan.render_pass;
	ImGui_ImplVulkan_Init(&init_info);

	mlx_clear_window(mlx_ctx, win, (mlx_color){ .rgba = 0x334D4DFF });

	uint32_t color = 0;
	for(int j = 0; j < 400; j++)
	{
		mlx_pixel_put(mlx_ctx, win, j, j, (mlx_color){ .rgba = 0x0000FFFF + (color << 24) });
		mlx_pixel_put(mlx_ctx, win, 399 - j, j, (mlx_color){ .rgba = 0x0000FFFF });
		color += (color < 255);
	}

	mlx.mlx_add_pre_render_hook(mlx_ctx, win, render, NULL);
	mlx_loop(mlx_ctx);

	vulkan.vkDeviceWaitIdle(mlx.mlx_get_vk_device(mlx_ctx));

	for(int i = 0; i < mlx.mlx_get_vk_swapchain_image_count(mlx_ctx, win); i++)
		vulkan.vkDestroyFramebuffer(mlx.mlx_get_vk_device(mlx_ctx), vulkan.framebuffers[i], NULL);
	free(vulkan.framebuffers);

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	igDestroyContext(ctx);

	mlx_destroy_window(mlx_ctx, win);

	vulkan.vkDestroyDescriptorPool(mlx.mlx_get_vk_device(mlx_ctx), pool, NULL);
	vulkan.vkDestroyRenderPass(mlx.mlx_get_vk_device(mlx_ctx), vulkan.render_pass, NULL);

	mlx_destroy_context(mlx_ctx);

	return 0;
}
