#include <iostream>
#include "Headers/vkloader.h"
#include <string>
using std::string;

#define OUT(val) \
    std::cout << val << std::endl;

#define OUT_ERROR(val)       \
    error = val;             \
    if (error != VK_SUCCESS) \
    return error

void printStat(VkPhysicalDevice device);
void printStats();

int main()
{
    int error = 0;
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "Vulkan Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pEngineName = "Example";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo;
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = NULL;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = NULL;
    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.ppEnabledExtensionNames = NULL;

    OUT_ERROR(mvk::createInstance(&instanceInfo));

    OUT_ERROR(vkEnumeratePhysicalDevices(mvk::instance, &mvk::PUcount, NULL));
    mvk::physicalDevices = new VkPhysicalDevice[mvk::PUcount];
    OUT_ERROR(vkEnumeratePhysicalDevices(mvk::instance, &mvk::PUcount, mvk::physicalDevices));

    printStats();

    VkDeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = NULL;
    deviceQueueCreateInfo.flags = 0;
    uint32_t FamilyIndex = 0; //TODO: Search for best Family
    deviceQueueCreateInfo.queueFamilyIndex = FamilyIndex;
    deviceQueueCreateInfo.queueCount = 1; //TODO: Load correct value
    deviceQueueCreateInfo.pQueuePriorities = NULL;

    VkPhysicalDeviceFeatures usedFeatures = {};
    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = NULL;
    deviceCreateInfo.pEnabledFeatures = &usedFeatures;

    //OUT_ERROR(mvk::createDevice(0, &deviceCreateInfo));//TODO: pick best Device
    VkDevice device;
    VkPhysicalDevice v = mvk::physicalDevices[0];
    vkCreateDevice(v, &deviceCreateInfo, NULL, &device);
    getchar();
    return 0;
}

void printStat(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties *queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    OUT("Name: " << properties.deviceName);
    int ver = properties.apiVersion;
    OUT("Type: " << properties.deviceType);
    OUT("API-Version: " << VK_VERSION_MAJOR(ver) << "." << VK_VERSION_MINOR(ver) << "." << VK_VERSION_PATCH(ver));
    OUT("Driver-Version: " << properties.driverVersion);
    OUT("Vendor-ID: " << properties.vendorID);
    OUT("Device-ID: " << properties.deviceID);
    OUT("FEATURES:");
    OUT("GeometryShader:" << features.geometryShader);
    OUT("RobustBufferAccess:" << features.robustBufferAccess);
    OUT("FullDrawIndexUint32:" << features.fullDrawIndexUint32);
    OUT("discreteQueuePriorities:" << properties.limits.discreteQueuePriorities);
    OUT("Queue Families:"
        << "(" << queueFamilyCount << ")");
    for (int i = 0; i < queueFamilyCount; ++i)
    {
        OUT("-#" << i << ": [" << queueFamilies[i].queueCount << "]"
                 << " time:" << queueFamilies[i].timestampValidBits);
        OUT(" VK_QUEUE_GRAPHICS_BIT:" << ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0));
        OUT(" VK_QUEUE_COMPUTE_BIT:" << ((queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0));
        OUT(" VK_QUEUE_TRANSFER_BIT:" << ((queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0));
        OUT(" VK_QUEUE_SPARSE_BINDING_BIT:" << ((queueFamilies[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0));
        OUT(" VK_QUEUE_PROTECTED_BIT:" << ((queueFamilies[i].queueFlags & VK_QUEUE_PROTECTED_BIT) != 0));
        uint32_t width = queueFamilies[i].minImageTransferGranularity.width;
        uint32_t height = queueFamilies[i].minImageTransferGranularity.height;
        uint32_t depth = queueFamilies[i].minImageTransferGranularity.depth;
        OUT(" Min Image Transfer Granularity:[" << width << "|" << height << "] x" << depth);
    }
}

void printStats()
{
    OUT("Devices found: " << mvk::PUcount);
    for (int i = 0; i < mvk::PUcount; ++i)
    {
        OUT(i << ":");
        printStat(mvk::physicalDevices[i]);
    }
}