#include <cstdint>
#include <vector>
#include <winsock2.h>

#pragma pack(push, 1) // padding ruins packet layout

struct DataPacket {
public:
	// type 1 = auth, 2 = message, 3 = disconnect
	uint16_t packetType; 
	uint32_t dataLength;
	char payload[512];

private:
	std::vector<uint8_t> serializePacket(DataPacket& orgPacket);
	DataPacket deserializePacket(std::vector<uint8_t> buffer);
};


std::vector<uint8_t> DataPacket::serializePacket(DataPacket& orgPacket) {
	// Don't want to edit original
	DataPacket newPacket = orgPacket;

	newPacket.packetType = htons(newPacket.packetType);
	newPacket.dataLength = htonl(newPacket.dataLength);

	// We dynamically allocated a block of memory that is the size of the sum of variable sizes
	std::vector<uint8_t> buffer(sizeof(DataPacket));

	// buffer.data() returns pointer to beginning of vector
	std::memcpy(buffer.data(), &newPacket, sizeof(DataPacket));

	return buffer;
}


DataPacket DataPacket::deserializePacket(std::vector<uint8_t> buffer) {
	DataPacket ret;

	// Memory copies all of the data from the vector buffer to the memory where datapacket is
	std::memcpy(&ret, buffer.data(), sizeof(DataPacket));

	ret.packetType = ntohs(ret.packetType);
	ret.dataLength = ntohl(ret.dataLength);

	return ret;
}


#pragma pack(pop)