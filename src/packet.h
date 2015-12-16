#ifndef __IOT_PACKET__H
#define __IOT_PACKET__H

#include "common.h"

struct iotCachedPacket {
	int 	m_Len;
	char	m_Buff[CACHE_BUFFER_MAX];
} iotCachedPacket;

struct iotSendPacket {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[SND_BUFFER_MAX];
} iotSendPacket;

struct iotRecvPacket {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[RCV_BUFFER_MAX];
} iotRecvPacket;

// Packet Cache State
int 						iot_cache_clear();
int 						iot_cache_init();
int 						iot_cache_add(struct iotRecvPacket* p);
struct iotCachedPacket* 	iot_cache_find(struct iotRecvPacket* p);

// Packet Creation
int 						iot_packet_create(struct iotSendPacket* p, char* msg);
int 						iot_packet_serialize(struct iotSendPacket* p);
int 						iot_packet_create_json(char* json);
void* 						iot_packet_create_serialized_json(char* json);

#endif  // __IOT_PACKET__H