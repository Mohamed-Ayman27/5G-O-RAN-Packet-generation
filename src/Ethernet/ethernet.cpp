#include "ethernet.h"
#include <iostream>
#include <new>  // For nothrow
#include <cstring>  // For memcpy
using namespace std;

EthPacket::EthPacket(uint64_t numOfGeneratedPackets, uint64_t payloadSize, uint64_t numOfTotalIFGs, uint64_t numOfTotalIdleBytesPerPeriod)
{
    // Allocate memory and initialize members
    try {
        eth_ExtraBytes = new uint8_t[numOfGeneratedPackets];
        eth_IdleBytes = new uint8_t[numOfTotalIdleBytesPerPeriod];
        eth_GeneratedPackets = new uint8_t*[numOfGeneratedPackets];
        for (uint64_t i = 0; i < numOfGeneratedPackets; ++i)
            eth_GeneratedPackets[i] = new uint8_t[eth_PacketSize];

        eth_Payload = new uint8_t[payloadSize];
        eth_IFGsPerPacket = new uint8_t[numOfTotalIFGs];
    } catch (const bad_alloc&) {
        cerr << "Memory Allocation Failure" << endl;
        exit(EXIT_FAILURE);
    }
}

EthPacket::~EthPacket() {
    // Free allocated memory
    delete[] eth_ExtraBytes;
    delete[] eth_IdleBytes;
    for (uint64_t i = 0; i < numOfGeneratedPackets; ++i)
        delete[] eth_GeneratedPackets[i];
    delete[] eth_GeneratedPackets;
    delete[] eth_Payload;
    delete[] eth_IFGsPerPacket;
}

void EthPacket::eth_fillGeneratedPackets() {
    eth_fillPreambleSFD();
    eth_fillDestAddress();
    eth_fillSourceAddress();
    eth_fillEtherType();
    eth_fillPayload();
    eth_fillCRC();
    eth_fillIFGs();
}

void EthPacket::eth_fillPreambleSFD() {
    try {
        uint8_t temp_ptr[PREAMBLE_SFD_SIZE];
        eth_convertToByteArray(PREAMBLE_SFD, PREAMBLE_SFD_SIZE, temp_ptr);
        eth_writeIntoGeneratedPacketsDB(temp_ptr, PREAMBLE_SFD_SIZE, 0);

#ifdef DEBUG_MODE_ON
        eth_writeGeneratedPackets("Ethernet Debug Files/Generated_Preamble.txt", PREAMBLE_SFD_SIZE);
#endif
    } catch (const bad_alloc&) {
        cerr << "Memory Failure in eth_fillPreambleSFD" << endl;
        exit(EXIT_FAILURE);
    }
}

// Similar improvements would be made for eth_fillDestAddress, eth_fillSourceAddress, etc.
