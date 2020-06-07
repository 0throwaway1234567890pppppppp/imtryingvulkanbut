#ifndef _VKLOADER_H
#define _VKLOADER_H
#include <vulkan/vulkan.h>


namespace mvk
{
    extern VkInstance instance;
    extern uint32_t PUcount;
    extern VkPhysicalDevice *physicalDevices;
    extern VkDevice device;
    int createInstance(VkInstanceCreateInfo *info);
    int createDevice(int physical, VkDeviceCreateInfo *createInfo);
} // namespace mvk
#endif