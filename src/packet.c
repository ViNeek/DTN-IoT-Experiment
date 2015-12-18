#include <string.h>

#include "packet.h"

// Mule data cache
static struct iotCachedPacket	g_DataCache[CACHE_COUNT];

int iot_cache_init() {
	iot_cache_clear();

	return 0;
}

int iot_cache_clear() {
	memset(g_DataCache, 0, sizeof(g_DataCache));

	return 0;
}

int iot_cache_add(struct iotRecvPacket* p) {
	
	return 0;
}

struct iotCachedPacket* iot_cache_find(struct iotRecvPacket* p) {
	
	return NULL;
}

int iot_packet_serialize(struct iotSendPacket* p) {

	return 0;
}