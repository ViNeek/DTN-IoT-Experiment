#ifndef __IOT_CACHE__H
#define __IOT_CACHE__H

#include <iot/types.h>
#include <iot/packet.h>

#if TARGET==IOT_PLATFORM_SKY
#define IOT_CACHE_SIZE 20
#else
#define IOT_CACHE_SIZE 20
#endif

struct iotCache {
  iotInt32 m_NextIdx;
  struct iotCachedPacket m_Cache[IOT_CACHE_SIZE];
} iotCache;

// Packet Cache State
iotInt32 iot_cache_clear(struct iotCache *c);
iotInt32 iot_cache_init(struct iotCache *c);
iotInt32 iot_cache_add(struct iotCache *c, iotChar* buff, iotInt32 len);
iotChar *iot_cache_json_desc(struct iotCache *c, iotChar *buffer,
                             iotInt32 *len);
struct iotCachedPacket *iot_cache_next(struct iotCache *c);
iotInt32 iot_cache_random_populate(struct iotCache *c);

#endif // __IOT_CACHE__H
