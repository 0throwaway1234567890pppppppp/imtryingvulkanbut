#include "Headers/vkloader.h" // <- declarations

VkInstance mvk::instance;
VkDevice mvk::device;
uint32_t mvk::PUcount = 0;
VkPhysicalDevice *mvk::physicalDevices;

int mvk::createInstance(VkInstanceCreateInfo *info)
{
    return vkCreateInstance(info, NULL, &instance);
}
int mvk::createDevice(int physical, VkDeviceCreateInfo *createInfo)
{
    return vkCreateDevice(physicalDevices[physical], createInfo, NULL, &device);
}