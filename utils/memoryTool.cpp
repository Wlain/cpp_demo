//
// Created by cwb on 2022/4/8.
//

#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <cstring>


static inline const char* skipBlackSpace(const char* p) {
    while (*p == ' ') {
        p++;
    }
    return p;
}

static int32_t readProcessMemory(int pid) {
    int32_t procMemSize = 0;
    char name[32], data[256];

    sprintf(name, "/proc/%d/status", pid);
    int fd = open(name, O_RDONLY);
    if (fd != -1) {
        struct flock lock;
        int bytes = 0, locked = -1;

        // Initialization file lock
        memset(&lock, 0, sizeof(struct flock));
        lock.l_type  = F_RDLCK;
        lock.l_whence = SEEK_SET;

        locked = fcntl(fd, F_SETLKW, &lock);
        while ((bytes = read(fd, data, 256 - 1)) > 0) {
            const char* p1 = data, *p2 = NULL;
            data[bytes] = '\0';

            p2 = strstr(p1, "VmRSS:");
            if (p2 != NULL) {
                p2 += sizeof("VmRSS:");
                p1 = skipBlackSpace(p2);
                procMemSize = atoi(p1);
                break;
            }
        }
        if (locked != -1) {
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
    } else {
        //printf("[ResourceMonitor] Read %s failed\n", name);
    }
    return procMemSize;
}

float getUsageMemoryForAndroid() {
    pid_t pid = getpid();
    int32_t size = readProcessMemory(pid);
    uint32_t processMemorySizeKb = 0;
    if (size > 0) {
        processMemorySizeKb = static_cast<uint32_t>(size);
    } else {
        processMemorySizeKb = 0;
    }
    return  processMemorySizeKb / 1024.0f;
}

