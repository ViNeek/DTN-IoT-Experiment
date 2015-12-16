#ifndef __IOT_NETWORK__H
#define __IOT_NETWORK__H

#define BROADCAST_ADDRESS "255.255.255.255"
#define BROADCAST_PORT 52228
#define RCV_BUFFER_MAX 128
#define SND_BUFFER_MAX 128
#define CACHE_BUFFER_MAX 512
#define CACHE_COUNT 100

#ifdef NATIVE

#include <unistd.h>

#ifdef WIN32

#include <Windows.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#endif // WIN32

#endif // NATIVE

#include "dtn.h"

struct iotClient {

#ifdef NATIVE

	int 					m_Sock;
	char	 				m_Buffer[1024];
	struct sockaddr_in 		m_ClientAddr;

#endif

} iotClient;

#define iotServer iotClient
#define iotBroadcaster iotClient
#define iotListener iotClient

int iot_init_networking();
int iot_uninit_networking();

// Init
int iot_client_create(struct iotClient* c);
int iot_broadcaster_create(struct iotBroadcaster* b);
int iot_listener_create(struct iotListener* b);

// Messaging
int iot_client_sendto(struct iotClient* s, struct iotServer* r, struct iotPacket* p);
int iot_client_recvfrom(struct iotServer* r, struct iotClient* s);

// Closing
int iot_client_close(struct iotClient* c);

#endif  // __IOT_NETWORK__H