#include <iot/packet.h>

static char* g_PacketTypes[MAX_TYPE] = {
    "Event",
    "Traffic",
    "Discount",
    "Parking",
    "Food"
};

const char* iot_packet_type(enum iotPacketTypes t) {
    return g_PacketTypes[t];
}
