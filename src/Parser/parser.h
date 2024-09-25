#ifndef PARSER_H
#define PARSER_H

#include "../Ethernet/ethernet.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class Parser {
    enum class Selection {
        Key,
        Value
    } sel;

    std::ifstream setupFile;
    std::vector<std::string> keys;
    std::vector<std::string> values;
    std::vector<uint64_t> convertedValues;
    std::string temp;
    std::string fileName;
    int linesCounter = 0;
    std::ofstream outFile;
    EthConfig *p_config;

    // Private method for writing to file
    void writeFile(const std::string&, int);

public:
    // Constructor and Destructor
    Parser(const std::string&, EthConfig*);
    ~Parser();

    // Public methods for file parsing and data extraction
    std::vector<std::string> readFile();
    std::vector<std::string> removeComments();
    void extractKeysAndValues(const std::vector<std::string>&, std::vector<std::string>&);
    void removeSpaces(std::vector<std::string>&, std::vector<std::string>&);
    std::vector<uint64_t> convertToValue();
    void setValuesToConfigurationStructure();
    void extractSetupParameters();
};

#endif
