#ifndef __IOT_PACKET__H
#define __IOT_PACKET__H

#include <iot/types.h>

#define IOT_PACKET_SIZE 128
#define IOT_PACKET_HEADER_OFFSET 0
#define IOT_PACKET_PAYLOAD_OFFSET 32

struct iotClient;
struct iotDataMule;

// Available packet types
enum iotPacketTypes {
  EVENT,
  TRAFFIC,
  DISCOUNT,
  PARKING,
  FOOD,
  MAX_TYPE,
  UNKNOWN_TYPE
};

struct iotCachedPacket {
  iotInt32 m_Len;
  enum iotPacketTypes m_Type;
  char m_Buff[IOT_PACKET_SIZE];
} iotCachedPacket;

void iot_packet_test();
const iotChar* iot_packet_generate_forward(struct iotClient* c, iotChar* buffer, iotInt32* len);
const iotChar* iot_packet_generate_request(struct iotClient* c, const iotChar* data, iotChar* buffer, iotInt32* len);
const iotChar* iot_packet_generate_response(struct iotDataMule* c, const iotChar* data, iotChar* buffer, iotInt32* len);
const char *iot_packet_type(enum iotPacketTypes t);
enum iotPacketTypes iot_packet_type_by_string(const char *desc);

#endif
