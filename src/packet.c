#include "packet.h"

// Mule data cache
static struct iotCachedPacket	g_DataCache[CACHE_COUNT];

int iot_cache_init() {
	iot_cache_clear();
}

int iot_cache_clear() {
	memset(g_DataCache, 0, sizeof(g_DataCache));
}

int iot_cache_add(struct iotRecvPacket* p) {
	
}

struct iotCachedPacket* iot_cache_find(struct iotPacket* p) {
	
}

int iot_packet_serialize(struct iotSendPacket* p);