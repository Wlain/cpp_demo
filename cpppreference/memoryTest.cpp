//
// Created by ytech on 2021/11/2.
//
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <fstream>
#include <iostream>

#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <cmath>
#include <iosfloat getUseageMemory()
{
float processMemory = 0;
task_vm_info_data_t info;
mach_msg_type_number_t size = TASK_VM_INFO_COUNT;
kern_return_t kerr = task_info(mach_task_self(),
                               TASK_VM_INFO,
                               (task_info_t)&info,
                               &size);
if (kerr == KERN_SUCCESS) {
if (info.phys_footprint != 0 && info.phys_footprint < 0x100000000LL) { // iOS8
processMemory = static_cast<float>(info.phys_footprint / 1024.0f);
} else {
processMemory = static_cast<float>(info.internal / 1024.0f);
}
} else {
processMemory = 0;
}
return processMemory / 1000.0f;
}tream>



bool is_monotonic_mode_ = false;
static const int32_t USE_MONOTONIC   = 2;
static const int32_t USE_UNKNOWN     = -1;
static const int32_t CPU_STATS_NUM   = 10;
static const int32_t USE_PROC_STAT   = 0;
static const int32_t USE_PROC_UPTIME = 1;
static const int32_t SEC_TO_MS_FAC   = 1000;
static const uint32_t kResourceMonitorInterval = 2000;
int64_t system_cpu_last_total_time_ = 0;
int64_t system_cpu_total_time_ = 0;
int64_t system_cpu_last_use_time_;
int64_t theory_thres_ = 0;
std::atomic<uint32_t> aprocessor_number_;
int32_t idle_factor_ = 0;
int32_t total_factor_ = 0;
int64_t last_ok_idle_ = 0;
int64_t system_cpu_use_time_ = 0;

static bool threadSafeReadFile(const char* name, const int size, char* data) {
    int bytes = 0;
    if (name != NULL && size != 0) {
        int fd = open(name, 0);

        if (fd != -1) {
            struct flock lock;
            int locked = -1;

            // Initialization file lock
            memset(&lock, 0, sizeof(struct flock));
            lock.l_type  = F_RDLCK;
            lock.l_whence = SEEK_SET;

            locked = fcntl(fd, F_SETLKW, &lock);
            bytes = read(fd, data, size - 1);
            if (bytes > 0) {
                data[bytes] = '\0';
            }
            if (locked != -1) {
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
            }
            close(fd);
        }
    }
    return bytes;
}

static inline const char* skipBlackSpace(const char* p) {
    while (*p == ' ') {
        p++;
    }
    return p;
}

static int32_t readProcStat(int64_t& total, int64_t& idle) {
    const char* name = "/proc/stat";
    char data[128] = {0};

    if (threadSafeReadFile(name, 128, data) > 0) {
        const char* p1 = data, *p2 = nullptr;
        int32_t value = 0;

        p2 = strstr(p1, " ");
        p1 = skipBlackSpace(p2);
        for (int i = 0; i < CPU_STATS_NUM; i++) {
            p2 = strstr(p1, " ");
            if (p2 != NULL) {
                value = atoi(p1);
                total += value;
                if (3 == i) {
                    idle = value;
                }
                p1 = skipBlackSpace(p2);
            } else {
                return USE_UNKNOWN;
            }
        }
        return USE_PROC_STAT;
    } else {
//    printf("[ResourceMonitor] Read %s failed\n", name);
        return USE_UNKNOWN;
    }
}


static int32_t readProcUptime(int64_t& total, int64_t& idle) {
    const char* name = "/proc/uptime";
    char data[64] = {0};

    if (threadSafeReadFile(name, 64, data) > 0) {
        const char* p1 = data, *p2 = NULL;

        p2 = strstr(p1, " ");
        if (p2 != NULL) {
            float freal = atof(p1);
            p1 = skipBlackSpace(p2);
            float fidle = atof(p1);
            total = static_cast<int64_t>(freal * SEC_TO_MS_FAC);
            idle  = static_cast<int64_t>(fidle * SEC_TO_MS_FAC);
            return USE_PROC_UPTIME;
        }
    } else {
//    printf("[ResourceMonitor] Read %s failed\n", name);
    }
    return USE_UNKNOWN;
}

static int readSystemCpu(int64_t& total, int64_t& idle) {
    int ret = readProcStat(total, idle);
    if (ret < 0) {
        return readProcUptime(total, idle);
    }
    return ret;
}

float UpdateSystemCpuUsageLocal() {
    float system_cpu_usage_;
    int64_t total_time = 0, idle_time = 0;
    bool is_first_time = false;
    int ret = 2;
    if (!is_monotonic_mode_ && ((ret = readSystemCpu(total_time, idle_time)) < 0)) {
        is_monotonic_mode_ = true;
        ret = USE_MONOTONIC;
        return 0.0f;
    } else if (is_monotonic_mode_) {
        return 0.0f;
    }

    if (system_cpu_last_use_time_ == -1) {
        is_first_time = true;
    }

    if (USE_PROC_UPTIME == ret) {
        total_time = (total_time * total_factor_) / SEC_TO_MS_FAC;
        idle_time = (idle_time * idle_factor_) / SEC_TO_MS_FAC;
    }
    // check error points of "/proc/stat" file
    if (!is_first_time) {
        uint32_t processor = aprocessor_number_;
        if (processor < processor_number_) {
            processor = processor_number_;
        }
        theory_thres_ = ((int64_t)kResourceMonitorInterval *
                         idle_factor_ * processor) / SEC_TO_MS_FAC;
        if (abs(idle_time - last_ok_idle_) < theory_thres_) {
            theory_thres_ = 0;
        }
    }
    system_cpu_last_total_time_ = system_cpu_total_time_;
    system_cpu_last_use_time_   = system_cpu_use_time_;

    last_ok_idle_ = idle_time;
    system_cpu_total_time_ = total_time;
    system_cpu_use_time_ = total_time - idle_time;
    if (!is_first_time && !theory_thres_) {
        int64_t total_diff = system_cpu_total_time_ - system_cpu_last_total_time_;
        int64_t use_diff = system_cpu_use_time_ - system_cpu_last_use_time_;
        if (total_diff > 0 && use_diff >= 0 && use_diff <= total_diff) {
            system_cpu_usage_ = std::round(100.0f * use_diff, total_diff);
        }
    }
    return system_cpu_usage_;
}