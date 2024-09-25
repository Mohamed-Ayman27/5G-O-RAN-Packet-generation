#include <iostream>
#include "../src/Parser/parser.h"
#include "../src/Ethernet/ethernet.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <setup_file> <packet_data>" << std::endl;
        return 1;
    }

    // Initialize Ethernet configuration
    EthConfig ethConfig;

    // Create Parser object and handle setup file
    Parser parser(argv[1], &ethConfig);

    // Create EthPacket object for handling packet data
    EthPacket packetHandler(&ethConfig, argv[2]);

    std::cout << "Ethernet configuration and packet setup complete." << std::endl;

    return 0;
}
