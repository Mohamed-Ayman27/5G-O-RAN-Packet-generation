#ifndef ETHERNET_H
#define ETHERNET_H

#include <cstdint>

// Constants
const uint8_t PREAMBLE_SFD_SIZE = 8;
const uint8_t ADDRESS_SIZE = 6;
const uint8_t ETHERTYPE_SIZE = 2;
const uint8_t CRC_SIZE = 4;
const uint16_t eth_PacketSize = 1500;  // Assuming 1500 bytes as packet size for simplicity

class EthPacket {
public:
    // Constructor and Destructor
    EthPacket(uint64_t numOfGeneratedPackets, uint64_t payloadSize, uint64_t numOfTotalIFGs, uint64_t numOfTotalIdleBytesPerPeriod);
    ~EthPacket();

    // Main method to fill all packet components
    void eth_fillGeneratedPackets();

private:
    // Memory allocations for different packet fields
    uint8_t* eth_ExtraBytes;
    uint8_t* eth_IdleBytes;
    uint8_t** eth_GeneratedPackets;  // Array of packet pointers
    uint8_t* eth_Payload;
    uint8_t* eth_IFGsPerPacket;
    uint64_t numOfGeneratedPackets;

    // Internal functions to fill packet fields
    void eth_fillPreambleSFD();
    void eth_fillDestAddress();
    void eth_fillSourceAddress();
    void eth_fillEtherType();
    void eth_fillPayload();
    void eth_fillCRC();
    void eth_fillIFGs();

    // Utility functions
    void eth_convertToByteArray(uint64_t data, uint8_t size, uint8_t* output);
    void eth_writeIntoGeneratedPacketsDB(const uint8_t* data, uint64_t size, uint64_t packetIndex);

    // Debugging utility
    void eth_writeGeneratedPackets(const char* filename, uint64_t size);
};

#endif
