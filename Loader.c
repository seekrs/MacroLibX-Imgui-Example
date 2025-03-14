#include "Loader.h"

#include <string.h>

void LoadMlxBindings(mlx_context mlx, MlxFuncs* mlx_funcs)
{
	#define MLX_FUNCTION(fn) mlx_funcs->fn = (PFN_##fn)mlx_get_proc_addr(mlx, #fn);
		#include "MlxDefs.h"
	#undef MLX_FUNCTION
}

void LoadVulkanBindings(mlx_context mlx, MlxFuncs* mlx_funcs, VulkanFuncs* vulkan_funcs)
{
	#define VULKAN_GLOBAL_FUNCTION(fn)   vulkan_funcs->fn = (PFN_##fn)mlx_funcs->mlx_get_vk_fn(mlx, #fn);
	#define VULKAN_INSTANCE_FUNCTION(fn) vulkan_funcs->fn = (PFN_##fn)mlx_funcs->mlx_get_vk_fn(mlx, #fn);
	#define VULKAN_DEVICE_FUNCTION(fn)   vulkan_funcs->fn = (PFN_##fn)mlx_funcs->mlx_get_vk_fn(mlx, #fn);
		#include "VulkanDefs.h"
	#undef VULKAN_GLOBAL_FUNCTION
	#undef VULKAN_INSTANCE_FUNCTION
	#undef VULKAN_DEVICE_FUNCTION
}

PFN_vkVoidFunction ImGuiLoadVulkan(const char* name, void* data)
{
	#define VULKAN_GLOBAL_FUNCTION(fn)   if(strcmp(name, #fn) == 0) return (PFN_vkVoidFunction)GetVulkanFuncs()->fn;
	#define VULKAN_INSTANCE_FUNCTION(fn) if(strcmp(name, #fn) == 0) return (PFN_vkVoidFunction)GetVulkanFuncs()->fn;
	#define VULKAN_DEVICE_FUNCTION(fn)   if(strcmp(name, #fn) == 0) return (PFN_vkVoidFunction)GetVulkanFuncs()->fn;
		#include "VulkanDefs.h"
	#undef VULKAN_GLOBAL_FUNCTION
	#undef VULKAN_INSTANCE_FUNCTION
	#undef VULKAN_DEVICE_FUNCTION
	return GetVulkanFuncs()->vkGetInstanceProcAddr((VkInstance)data, name);
}

MlxFuncs* GetMlxFuncs()
{
	static MlxFuncs funcs;
	return &funcs;
}

VulkanFuncs* GetVulkanFuncs()
{
	static VulkanFuncs funcs;
	return &funcs;
}
