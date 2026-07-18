// net_loop.hpp -- loopback network driver declarations
#pragma once

struct qsocket_s;
typedef struct qsocket_s qsocket_t;
struct sizebuf_t;

namespace Net {

int Loop_Init();
void Loop_Listen(bool state);
void Loop_SearchForHosts(bool xmit);
qsocket_t* Loop_Connect(const char* host);
qsocket_t* Loop_CheckNewConnections();
int Loop_GetMessage(qsocket_t* sock);
int Loop_SendMessage(qsocket_t* sock, sizebuf_t* data);
int Loop_SendUnreliableMessage(qsocket_t* sock, sizebuf_t* data);
bool Loop_CanSendMessage(qsocket_t* sock);
bool Loop_CanSendUnreliableMessage();
void Loop_Close(qsocket_t* sock);
void Loop_Shutdown();

} // namespace Net
