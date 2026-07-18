// net_udp.hpp -- UDP network driver declarations
#pragma once

#include <cstdint>

struct qsockaddr;

namespace Net {

int UDP_Init();
void UDP_Shutdown();
void UDP_Listen(bool state);
int UDP_OpenSocket(int port);
int UDP_CloseSocket(int socket);
int UDP_Connect(int socket, qsockaddr* addr);
int UDP_CheckNewConnections();
int UDP_Read(int socket, std::uint8_t* buf, int len, qsockaddr* addr);
int UDP_Write(int socket, std::uint8_t* buf, int len, qsockaddr* addr);
int UDP_Broadcast(int socket, std::uint8_t* buf, int len);
char* UDP_AddrToString(qsockaddr* addr);
int UDP_StringToAddr(const char* string, qsockaddr* addr);
int UDP_GetSocketAddr(int socket, qsockaddr* addr);
int UDP_GetNameFromAddr(qsockaddr* addr, char* name);
int UDP_GetAddrFromName(const char* name, qsockaddr* addr);
int UDP_AddrCompare(qsockaddr* addr1, qsockaddr* addr2);
int UDP_GetSocketPort(qsockaddr* addr);
int UDP_SetSocketPort(qsockaddr* addr, int port);

} // namespace Net
