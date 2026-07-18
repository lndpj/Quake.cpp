// net_dgrm.hpp -- datagram network driver declarations
#pragma once

struct qsocket_s;
typedef struct qsocket_s qsocket_t;
struct sizebuf_t;

namespace Net {

int Datagram_Init();
void Datagram_Listen(bool state);
void Datagram_SearchForHosts(bool xmit);
qsocket_t* Datagram_Connect(const char* host);
qsocket_t* Datagram_CheckNewConnections();
int Datagram_GetMessage(qsocket_t* sock);
int Datagram_SendMessage(qsocket_t* sock, sizebuf_t* data);
int Datagram_SendUnreliableMessage(qsocket_t* sock, sizebuf_t* data);
bool Datagram_CanSendMessage(qsocket_t* sock);
bool Datagram_CanSendUnreliableMessage();
void Datagram_Close(qsocket_t* sock);
void Datagram_Shutdown();

} // namespace Net
