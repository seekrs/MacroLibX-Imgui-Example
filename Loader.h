#ifndef LOADER_H
#define LOADER_H

#include <mlx.h>
#include <mlx_extended.h>
#include <vulkan/vulkan_core.h>

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

#define IM_UNUSED(_VAR)  ((void)(_VAR))
#define IM_ASSERT(_EXPR) assert(_EXPR)
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

typedef struct MlxFuncs
{
	#define MLX_FUNCTION(fn) PFN_##fn fn;
		#include "MlxDefs.h"
	#undef MLX_FUNCTION
} MlxFuncs;

typedef struct VulkanFuncs
{
	#define VULKAN_GLOBAL_FUNCTION(fn)   PFN_##fn fn;
	#define VULKAN_INSTANCE_FUNCTION(fn) PFN_##fn fn;
	#define VULKAN_DEVICE_FUNCTION(fn)   PFN_##fn fn;
		#include "VulkanDefs.h"
	#undef VULKAN_GLOBAL_FUNCTION
	#undef VULKAN_INSTANCE_FUNCTION
	#undef VULKAN_DEVICE_FUNCTION
} VulkanFuncs;

void LoadMlxBindings(mlx_context mlx, MlxFuncs* mlx_funcs);
void LoadVulkanBindings(mlx_context mlx, MlxFuncs* mlx_funcs, VulkanFuncs* vulkan_funcs);
PFN_vkVoidFunction ImGuiLoadVulkan(const char* fn, void* data);

MlxFuncs* GetMlxFuncs();
VulkanFuncs* GetVulkanFuncs();

#endif
