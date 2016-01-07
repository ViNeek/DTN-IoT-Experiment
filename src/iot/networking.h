#ifndef __IOT_NETWORK__H
#define __IOT_NETWORK__H

#define BROADCAST_ADDRESS "255.255.255.255"
#define BROADCAST_PORT 52228
#define RCV_BUFFER_MAX 128
#define SND_BUFFER_MAX 128
#define CACHE_BUFFER_MAX 512
#define CACHE_COUNT 100

#include <iot/types.h>

#ifdef DTN_NATIVE

#include <unistd.h>

#ifdef WIN32

#include <Windows.h>

#else

#ifdef WIN32

#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#endif // WIN32

#endif // DTN_NATIVE

#if defined(DTN_IOTLAB) || defined(DTN_IOTLAB_X86)

#endif

// Forward declare roles
//struct iotProducer;
//struct iotConsumer;
//struct iotDataMule;

// Forward declare packets
struct iotCachedPacket;
struct iotSendPacket;
struct iotRecvPacket;

struct iotClient;

/*
struct iotClient {

#ifdef DTN_NATIVE

  int m_Sock;
  char m_Buffer[1024];
  struct sockaddr_in m_ClientAddr;

  //struct broadcast_conn broadcast;

#endif

#if defined(DTN_IOTLAB) || defined(DTN_IOTLAB_X86)

  //struct broadcast_conn broadcast;

#endif

} iotClient;
*/

//#define iotServer iotClient
//#define iotBroadcaster iotClient
//#define iotListener iotClient

int iot_init_networking();
int iot_uninit_networking();

// Access callback arrays
const struct unicast_callbacks* iot_mule_unicast_callbacks();
const struct broadcast_callbacks* iot_mule_broadcast_callbacks();
const struct unicast_callbacks* iot_client_unicast_callbacks();
const struct broadcast_callbacks* iot_client_broadcast_callbacks();

// Init
//int iot_client_create(struct iotClient *c);
//int iot_broadcaster_create(struct iotBroadcaster *b);
//int iot_listener_create(struct iotListener *b);

// Messaging
int iot_client_sendto(struct iotClient *s, struct iotClient* r,
                      struct iotSendPacket *p);
//int iot_client_recvfrom(struct iotServer *r, struct iotClient *s);

// Closing
//int iot_client_close(struct iotClient *c);

#endif // __IOT_NETWORK__H
