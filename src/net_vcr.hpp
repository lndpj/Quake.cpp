// net_vcr.hpp -- VCR network driver declarations
#pragma once

struct qsocket_s;
typedef struct qsocket_s qsocket_t;
struct sizebuf_t;

namespace Net {

constexpr int VCR_OP_CONNECT = 1;
constexpr int VCR_OP_GETMESSAGE = 2;
constexpr int VCR_OP_SENDMESSAGE = 3;
constexpr int VCR_OP_CANSENDMESSAGE = 4;
constexpr int VCR_MAX_MESSAGE = 4;

int VCR_Init();
void VCR_SearchForHosts(bool xmit);
qsocket_t* VCR_Connect(const char* host);
qsocket_t* VCR_CheckNewConnections();
int VCR_GetMessage(qsocket_t* sock);
int VCR_SendMessage(qsocket_t* sock, sizebuf_t* data);
bool VCR_CanSendMessage(qsocket_t* sock);
void VCR_Close(qsocket_t* sock);
void VCR_Shutdown();

} // namespace Net
