#include <cstdint>

#pragma pack(push, 1) // padding ruins packet layout

struct DataPacket {

	// type 1 = auth, 2 = message, 3 = disconnect
	uint16_t packetType;
	uint32_t dataLength;
	char payload[512];

};

#pragma pack(pop)