//
// Created by ytech on 2021/11/2.
//
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>

float getUsageMemory()
{
    float processMemory = 0;
    task_vm_info_data_t info;
    mach_msg_type_number_t size = TASK_VM_INFO_COUNT;
    kern_return_t kerr = task_info(mach_task_self(),
                                   TASK_VM_INFO,
                                   (task_info_t)&info,
                                   &size);
    if (kerr == KERN_SUCCESS)
    {
        if (info.phys_footprint != 0 && info.phys_footprint < 0x100000000LL)
        { // iOS8
            processMemory = static_cast<float>(info.phys_footprint / 1024.0f);
        }
        else
        {
            processMemory = static_cast<float>(info.internal / 1024.0f);
        }
    }
    else
    {
        processMemory = 0;
    }
    return processMemory / 1000.0f;
}
