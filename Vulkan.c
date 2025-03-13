#include "Vulkan.h"
#include "Loader.h"

#include <stdlib.h>

VkDescriptorPool VulkanCreateDescriptorPool(mlx_context mlx)
{
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
	VkDescriptorPool pool;
	GetVulkanFuncs()->vkCreateDescriptorPool(GetMlxFuncs()->mlx_get_vk_device(mlx), &pool_info, NULL, &pool);
	return pool;
}

VkRenderPass VulkanCreateRenderPass(mlx_context mlx, mlx_window win)
{
	VkAttachmentDescription attachment = { 0 };
	attachment.format = GetMlxFuncs()->mlx_get_vk_swapchain_format(mlx, win);
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
	VkRenderPass renderpass;
	GetVulkanFuncs()->vkCreateRenderPass(GetMlxFuncs()->mlx_get_vk_device(mlx), &renderpass_create_info, NULL, &renderpass);
	return renderpass;
}

VkFramebuffer* VulkanCreateFramebuffers(mlx_context mlx, mlx_window win, VkRenderPass renderpass)
{
	VkFramebuffer* framebuffers = (VkFramebuffer*)calloc(GetMlxFuncs()->mlx_get_vk_swapchain_image_count(mlx, win), sizeof(VkFramebuffer));
	if(framebuffers == NULL)
		return NULL;
	for(int i = 0; i < GetMlxFuncs()->mlx_get_vk_swapchain_image_count(mlx, win); i++)
	{
		VkImageView view = GetMlxFuncs()->mlx_get_vk_swapchain_image_view(mlx, win, i);
		VkFramebufferCreateInfo framebuffer_info = { 0 };
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = renderpass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = &view;
		framebuffer_info.width = GetMlxFuncs()->mlx_get_vk_swapchain_extent(mlx, win).width;
		framebuffer_info.height = GetMlxFuncs()->mlx_get_vk_swapchain_extent(mlx, win).height;
		framebuffer_info.layers = 1;
		GetVulkanFuncs()->vkCreateFramebuffer(GetMlxFuncs()->mlx_get_vk_device(mlx), &framebuffer_info, NULL, &framebuffers[i]);
	}
	return framebuffers;
}

void VulkanDestroyFramebuffers(mlx_context mlx, mlx_window win, VkFramebuffer* framebuffers)
{
	VkDevice device = GetMlxFuncs()->mlx_get_vk_device(mlx);
	GetVulkanFuncs()->vkDeviceWaitIdle(device);
	for(int i = 0; i < GetMlxFuncs()->mlx_get_vk_swapchain_image_count(mlx, win); i++)
		GetVulkanFuncs()->vkDestroyFramebuffer(device, framebuffers[i], NULL);
	free(framebuffers);
}
