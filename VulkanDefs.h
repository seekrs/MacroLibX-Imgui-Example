// No header guard

#ifdef VK_VERSION_1_0
	#ifdef VULKAN_GLOBAL_FUNCTION
		VULKAN_GLOBAL_FUNCTION(vkCreateInstance)
		VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)
		VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)
		VULKAN_GLOBAL_FUNCTION(vkGetInstanceProcAddr)
	#endif

	#ifdef VULKAN_INSTANCE_FUNCTION
		VULKAN_INSTANCE_FUNCTION(vkCreateDevice)
		VULKAN_INSTANCE_FUNCTION(vkDestroyInstance)
		VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)
		VULKAN_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)
		VULKAN_INSTANCE_FUNCTION(vkGetDeviceProcAddr)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceImageFormatProperties)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)
		#ifdef DEBUG
			#ifdef VK_EXT_debug_utils
				VULKAN_INSTANCE_FUNCTION(vkSetDebugUtilsObjectNameEXT)
				//VULKAN_INSTANCE_FUNCTION(vkSetDebugUtilsObjectTagEXT)
			#endif
		#endif
	#endif

	#ifdef VULKAN_DEVICE_FUNCTION
		VULKAN_DEVICE_FUNCTION(vkAllocateCommandBuffers)
		VULKAN_DEVICE_FUNCTION(vkAllocateDescriptorSets)
		VULKAN_DEVICE_FUNCTION(vkAllocateMemory)
		VULKAN_DEVICE_FUNCTION(vkBeginCommandBuffer)
		VULKAN_DEVICE_FUNCTION(vkBindBufferMemory)
		VULKAN_DEVICE_FUNCTION(vkBindImageMemory)
		VULKAN_DEVICE_FUNCTION(vkCmdBeginRenderPass)
		VULKAN_DEVICE_FUNCTION(vkCmdBindDescriptorSets)
		VULKAN_DEVICE_FUNCTION(vkCmdBindIndexBuffer)
		VULKAN_DEVICE_FUNCTION(vkCmdBindPipeline)
		VULKAN_DEVICE_FUNCTION(vkCmdBindVertexBuffers)
		VULKAN_DEVICE_FUNCTION(vkCmdClearAttachments)
		VULKAN_DEVICE_FUNCTION(vkCmdClearColorImage)
		VULKAN_DEVICE_FUNCTION(vkCmdClearDepthStencilImage)
		VULKAN_DEVICE_FUNCTION(vkCmdCopyBuffer)
		VULKAN_DEVICE_FUNCTION(vkCmdCopyBufferToImage)
		VULKAN_DEVICE_FUNCTION(vkCmdCopyImage)
		VULKAN_DEVICE_FUNCTION(vkCmdCopyImageToBuffer)
		VULKAN_DEVICE_FUNCTION(vkCmdDraw)
		VULKAN_DEVICE_FUNCTION(vkCmdDrawIndexed)
		VULKAN_DEVICE_FUNCTION(vkCmdEndRenderPass)
		VULKAN_DEVICE_FUNCTION(vkCmdPipelineBarrier)
		VULKAN_DEVICE_FUNCTION(vkCmdPushConstants)
		VULKAN_DEVICE_FUNCTION(vkCmdSetScissor)
		VULKAN_DEVICE_FUNCTION(vkCmdSetViewport)
		VULKAN_DEVICE_FUNCTION(vkCreateBuffer)
		VULKAN_DEVICE_FUNCTION(vkCreateCommandPool)
		VULKAN_DEVICE_FUNCTION(vkCreateDescriptorPool)
		VULKAN_DEVICE_FUNCTION(vkCreateDescriptorSetLayout)
		VULKAN_DEVICE_FUNCTION(vkCreateFence)
		VULKAN_DEVICE_FUNCTION(vkCreateFramebuffer)
		VULKAN_DEVICE_FUNCTION(vkCreateGraphicsPipelines)
		VULKAN_DEVICE_FUNCTION(vkCreateImage)
		VULKAN_DEVICE_FUNCTION(vkCreateImageView)
		VULKAN_DEVICE_FUNCTION(vkCreatePipelineLayout)
		VULKAN_DEVICE_FUNCTION(vkCreateRenderPass)
		VULKAN_DEVICE_FUNCTION(vkCreateSampler)
		VULKAN_DEVICE_FUNCTION(vkCreateSemaphore)
		VULKAN_DEVICE_FUNCTION(vkCreateShaderModule)
		VULKAN_DEVICE_FUNCTION(vkDestroyBuffer)
		VULKAN_DEVICE_FUNCTION(vkDestroyCommandPool)
		VULKAN_DEVICE_FUNCTION(vkDestroyDescriptorPool)
		VULKAN_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout)
		VULKAN_DEVICE_FUNCTION(vkDestroyDevice)
		VULKAN_DEVICE_FUNCTION(vkDestroyFence)
		VULKAN_DEVICE_FUNCTION(vkDestroyFramebuffer)
		VULKAN_DEVICE_FUNCTION(vkDestroyImage)
		VULKAN_DEVICE_FUNCTION(vkDestroyImageView)
		VULKAN_DEVICE_FUNCTION(vkDestroyPipeline)
		VULKAN_DEVICE_FUNCTION(vkDestroyPipelineLayout)
		VULKAN_DEVICE_FUNCTION(vkDestroyRenderPass)
		VULKAN_DEVICE_FUNCTION(vkDestroySampler)
		VULKAN_DEVICE_FUNCTION(vkDestroySemaphore)
		VULKAN_DEVICE_FUNCTION(vkDestroyShaderModule)
		VULKAN_DEVICE_FUNCTION(vkDeviceWaitIdle)
		VULKAN_DEVICE_FUNCTION(vkEndCommandBuffer)
		VULKAN_DEVICE_FUNCTION(vkFlushMappedMemoryRanges)
		VULKAN_DEVICE_FUNCTION(vkFreeCommandBuffers)
		VULKAN_DEVICE_FUNCTION(vkFreeMemory)
		VULKAN_DEVICE_FUNCTION(vkGetBufferMemoryRequirements)
		VULKAN_DEVICE_FUNCTION(vkGetDeviceMemoryCommitment)
		VULKAN_DEVICE_FUNCTION(vkGetDeviceQueue)
		VULKAN_DEVICE_FUNCTION(vkGetFenceStatus)
		VULKAN_DEVICE_FUNCTION(vkGetImageMemoryRequirements)
		VULKAN_DEVICE_FUNCTION(vkGetImageSubresourceLayout)
		VULKAN_DEVICE_FUNCTION(vkInvalidateMappedMemoryRanges)
		VULKAN_DEVICE_FUNCTION(vkMapMemory)
		VULKAN_DEVICE_FUNCTION(vkQueueSubmit)
		VULKAN_DEVICE_FUNCTION(vkQueueWaitIdle)
		VULKAN_DEVICE_FUNCTION(vkResetCommandBuffer)
		VULKAN_DEVICE_FUNCTION(vkResetDescriptorPool)
		VULKAN_DEVICE_FUNCTION(vkResetEvent)
		VULKAN_DEVICE_FUNCTION(vkResetFences)
		VULKAN_DEVICE_FUNCTION(vkUnmapMemory)
		VULKAN_DEVICE_FUNCTION(vkUpdateDescriptorSets)
		VULKAN_DEVICE_FUNCTION(vkWaitForFences)
	#endif
#endif
#ifdef VK_KHR_swapchain
	#ifdef VULKAN_DEVICE_FUNCTION
		VULKAN_DEVICE_FUNCTION(vkAcquireNextImageKHR)
		VULKAN_DEVICE_FUNCTION(vkCreateSwapchainKHR)
		VULKAN_DEVICE_FUNCTION(vkDestroySwapchainKHR)
		VULKAN_DEVICE_FUNCTION(vkGetSwapchainImagesKHR)
		VULKAN_DEVICE_FUNCTION(vkQueuePresentKHR)
	#endif
#endif
#ifdef VK_KHR_surface
	#ifdef VULKAN_INSTANCE_FUNCTION
		VULKAN_INSTANCE_FUNCTION(vkDestroySurfaceKHR)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR)
		VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)
	#endif
#endif

