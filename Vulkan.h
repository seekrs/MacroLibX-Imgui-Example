#ifndef VULKAN_MANAGER_H
#define VULKAN_MANAGER_H

#include <mlx.h>
#include <mlx_extended.h>
#include <vulkan/vulkan_core.h>

VkDescriptorPool VulkanCreateDescriptorPool(mlx_context mlx);
VkRenderPass VulkanCreateRenderPass(mlx_context mlx, mlx_window win);
VkFramebuffer* VulkanCreateFramebuffers(mlx_context mlx, mlx_window win, VkRenderPass renderpass);

#endif
