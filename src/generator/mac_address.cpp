#include "mac_address.h"
#include <iostream>
#include <sstream>
#include <array>
#ifdef _WIN32
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#endif

std::string get_mac_address() {
#ifdef _WIN32
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);
    GetAdaptersInfo(AdapterInfo, &dwBufLen);
    std::ostringstream mac;
    mac << std::hex << std::uppercase;
    for (int i = 0; i < 6; i++) {
        mac << (int)AdapterInfo->Address[i];
        if (i < 5) mac << ":";
    }
    return mac.str();
#else
    struct ifaddrs* ifaddr, * ifa;
    std::string mac;
    if (getifaddrs(&ifaddr) == 0) {
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
                struct sockaddr_ll* s = (struct sockaddr_ll*)ifa->ifa_addr;
                std::ostringstream oss;
                for (int i = 0; i < 6; i++) {
                    oss << std::hex << (int)s->sll_addr[i];
                    if (i < 5) oss << ":";
                }
                mac = oss.str();
                break;
            }
        }
        freeifaddrs(ifaddr);
    }
    return mac;
#endif
}
