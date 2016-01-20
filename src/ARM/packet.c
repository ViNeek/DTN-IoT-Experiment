#include <iot/common.h>

#include <iot/mjson.h>

#include <string.h>

struct iotClient;

static char* g_PacketTypes[MAX_TYPE] = {
    "Event",
    "Traffic",
    "Discount",
    "Parking",
    "Food"
};

static iotChar *g_StringPtrs[MAX_TYPE];
static iotChar g_StringBuffer[IOT_PACKET_SIZE];
static iotInt32 g_StringCount;

static const struct json_array_t g_AvailableTypes = {
    .element_type = t_string,
    .arr.strings.ptrs = g_StringPtrs,
    .arr.strings.store = g_StringBuffer,
    .arr.strings.storelen = sizeof(g_StringBuffer),
    .count = &g_StringCount,
    .maxlen = sizeof(g_StringPtrs) / sizeof(g_StringPtrs[0]),
};

static const struct json_attr_t g_RequestType[] = {
    {"R", t_array, .addr.array.element_type = t_string,
                       .addr.array.arr.strings.ptrs = g_StringPtrs,
                       .addr.array.arr.strings.store = g_StringBuffer,
                       .addr.array.arr.strings.storelen = sizeof(g_StringBuffer),
                       .addr.array.count = &g_StringCount,
                       .addr.array.maxlen = sizeof(g_StringPtrs) / sizeof(g_StringPtrs[0])},
    {NULL},
};

void iot_packet_test() {
	int status;

	status = json_read_object(json_str6, g_ForwardType, NULL);
	IOT_LOG_INFO("Count %d", g_StringCount);
	IOT_LOG_INFO("String %s", g_StringPtrs[0]);
	IOT_LOG_INFO("String %d %d", strstr(json_str6, "\"F\""), strstr(json_str6, "\"R\""));
}

const iotChar* iot_packet_generate_forward(struct iotClient* c, iotChar* buffer, iotInt32* len) {
  iotInt32 status;
  iotInt32 val;
  iotInt32 randType;
  iotInt32 length = 2;
  iotBool empty = IOT_TRUE;
  buffer[0] = 0;

  strcat(buffer, "{\"F\":[");
  randType = iot_random_in_range(0, MAX_TYPE);
  
  strcat(buffer, "\"");
  strcat(buffer, iot_packet_type(randType));
  strcat(buffer, "\"");
  strcat(buffer, ",");	
  strcat(buffer, "\"");
  strcat(buffer, "Random Text");
  strcat(buffer, "\""); 
  strcat(buffer, "]}");

  length = strlen(buffer);

  IOT_LOG_INFO("Forward %s", buffer);
  *len = length;

  return buffer;
}

const iotChar* iot_packet_generate_request(struct iotClient* c, const iotChar* data, iotChar* buffer, iotInt32* len) {
  iotInt32 status;
  iotInt32 val;
  iotInt32 length = 2;
  iotBool empty = IOT_TRUE;
  buffer[0] = 0;

  status = json_read_array(data, &g_AvailableTypes, NULL);

  strcat(buffer, "{\"R\":[");
  for (iotInt32 i = 0; i < g_StringCount; ++i ) {
    val = iot_packet_type_by_string(g_StringPtrs[i]);
    if ( c->m_Interests[i] == val ) {
    	empty = IOT_FALSE;
    	//IOT_LOG_INFO("Want about %s", g_StringPtrs[i]);
    	strcat(buffer, "\"");
        strcat(buffer, g_StringPtrs[i]);
        strcat(buffer, "\"");
        strcat(buffer, ",");
    }
  }

  if ( !empty ) {
    // Erase dangling 'comma'
    length = strlen(buffer);
    buffer[length-1] = 0;
  }

  strcat(buffer, "]");

  *len = length;

  return buffer;
}

const iotChar* iot_packet_generate_response(struct iotDataMule* c, const iotChar* data, iotChar* buffer, iotInt32* len) {
  iotInt32 status;
  iotInt32 val;
  iotInt32 length = 2;
  iotBool empty = IOT_TRUE;
  buffer[0] = 0;

  status = json_read_array(data, &g_AvailableTypes, NULL);

  strcat(buffer, "{");
  for (iotInt32 i = 0; i < g_StringCount; ++i ) {
    //val = iot_packet_type_by_string(g_StringPtrs[i]);
    strcat(buffer, "\"");
    strcat(buffer, g_StringPtrs[i]);
    strcat(buffer, "\"");
    strcat(buffer, ":[");
    for (iotInt32 j = 0; j < IOT_CACHE_SIZE; ++j) {
  		if ( c->m_PacketCache.m_Cache[j].m_Type != UNKNOWN_TYPE ) {
  			iotChar *header = &c->m_PacketCache.m_Cache[j].m_Buff[IOT_PACKET_HEADER_OFFSET];
    		iotChar *payload = &c->m_PacketCache.m_Cache[j].m_Buff[IOT_PACKET_PAYLOAD_OFFSET];
    		//IOT_LOG_INFO("Buffered response %s", header);
    		//IOT_LOG_INFO("Buffered response %s", payload);
  			if ( strcmp(header, g_StringPtrs[i]) == 0 ) {
    			empty = IOT_FALSE;
    			strcat(buffer, "\"");
        		strcat(buffer, payload);
        		strcat(buffer, "\"");
        		strcat(buffer, ",");
    		}
  		}
  	}
  	if ( !empty ) {
    	// Erase dangling 'comma'
    	length = strlen(buffer);
    	buffer[length-1] = 0;
  	}	
  	strcat(buffer, "],");
  }
  
  // Erase dangling 'comma'
  length = strlen(buffer);
  buffer[length-1] = 0;

  strcat(buffer, "]");

  IOT_LOG_INFO("Buffered response %s", buffer);

  *len = length;

  return buffer;
}

const char* iot_packet_type(enum iotPacketTypes t) {
    return g_PacketTypes[t];
}

enum iotPacketTypes iot_packet_type_by_string(const char *desc) {
	for ( iotInt32 i = 0; i < MAX_TYPE; ++i) {
		if ( strcmp(desc, g_PacketTypes[i]) == 0 ) {
			return i;
		}	
	}

	return UNKNOWN_TYPE;
}