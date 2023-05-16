#pragma once

#include <algorithm>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <string>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <sys/uio.h>
#include <fstream>

namespace mem {
    inline pid_t pid = 0;

    inline pid_t get_pid_by_name(const std::string& name) {
        std::vector<std::string> pids;
        for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
            if (entry.is_directory()) {
                std::string path = entry.path();
                std::string pid = path.substr(path.find_last_of('/') + 1);
                if (std::all_of(pid.begin(), pid.end(), ::isdigit)) {
                    std::ifstream cmdline(path + "/comm");
                    std::string line;
                    std::getline(cmdline, line);
                    if (line.find(name) != std::string::npos) {
                        return std::stoi(pid);
                    }
                }
            }
        }
        return 0;
    }
    
    template <typename T>
    inline ssize_t read(uintptr_t address, T& buffer) {
        struct iovec local[1], remote[1];
        local[0].iov_base = &buffer;
        local[0].iov_len = sizeof(T);
        remote[0].iov_base = reinterpret_cast<void*>(address);
        remote[0].iov_len = sizeof(T);
        ssize_t nread = process_vm_readv(pid, local, 1, remote, 1, 0);
        if (nread != sizeof(T)) {
            return -1;
        }
        return nread;
    }

    template <typename T>
    inline ssize_t write(uintptr_t address, T value) {
        struct iovec local[1], remote[1];
        local[0].iov_base = &value;
        local[0].iov_len = sizeof(T);
        remote[0].iov_base = reinterpret_cast<void*>(address);
        remote[0].iov_len = sizeof(T);
        ssize_t nread = process_vm_writev(pid, local, 1, remote, 1, 0);
        if (nread != sizeof(T)) {
            return false;
        }
        return nread;
    }

    template <typename T>
    inline ssize_t read_array(uintptr_t address, T* buffer, size_t size) {
        struct iovec local[1], remote[1];
        local[0].iov_base = buffer;
        local[0].iov_len = sizeof(T) * size;
        remote[0].iov_base = reinterpret_cast<void*>(address);
        remote[0].iov_len = sizeof(T) * size;
        ssize_t nread = process_vm_readv(pid, local, 1, remote, 1, 0);
        if (nread != sizeof(T) * size) {
            return false;
        }
        return nread;
    }

    template <typename T>
    inline ssize_t read_multiple(std::vector<std::pair<uintptr_t, T>>& addresses) {
        struct iovec local[addresses.size()], remote[addresses.size()];
        for (size_t i = 0; i < addresses.size(); i++) {
            local[i].iov_base = &addresses[i].second;
            local[i].iov_len = sizeof(T);
            remote[i].iov_base = reinterpret_cast<void*>(addresses[i].first);
            remote[i].iov_len = sizeof(T);
        }
        ssize_t nread = process_vm_readv(pid, local, addresses.size(), remote, addresses.size(), 0);
        if (nread != sizeof(T) * addresses.size()) {
            return false;
        }
        return nread;
    }

    inline std::string read_string(uintptr_t address, size_t size) {
        std::string buffer(size, '\0');
        ssize_t nread = read_array(address, buffer.data(), size);
        if (nread != size) {
            return "";
        }
        return buffer;
    }
}