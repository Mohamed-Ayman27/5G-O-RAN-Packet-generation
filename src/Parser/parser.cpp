#include "parser.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <memory>
using namespace std;

Parser::Parser(string fileName, EthConfig* ptr) 
    : p_config(ptr), fileName(fileName) 
{
    if (fileName == "Error" || p_config == nullptr) {
        throw runtime_error("Invalid file name or null configuration pointer.");
    }

    ifstream setupFile(fileName);
    if (!setupFile.is_open()) {
        throw runtime_error("Couldn't open file: " + fileName);
    }

    string line;
    while (getline(setupFile, line)) {
        lines.push_back(line);
    }
    setupFile.close();

    keys.resize(lines.size());
    values.resize(lines.size());
    convertedValues.resize(lines.size());

    parser_extractSetupParameters();
}

Parser::~Parser() {
    // No manual memory management required with std::vector
}

vector<string> Parser::parser_readFile() {
    ifstream setupFile(fileName);
    if (!setupFile.is_open()) {
        throw runtime_error("Couldn't open file: " + fileName);
    }

    string line;
    int index = 0;
    while (getline(setupFile, line)) {
        keys[index++] = line;
    }

#ifdef DEBUG_MODE_ON
    parser_writeFile("Parser Debug Files/parser_readData.txt", key);
#endif

    return keys;
}

vector<string> Parser::parser_removeComments() {
    for (auto& line : keys) {
        size_t comment_pos = line.find('/');
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }
    }

#ifdef DEBUG_MODE_ON
    parser_writeFile("Parser Debug Files/parser_noComments.txt", key);
#endif

    return keys;
}

uint64_t* Parser::parser_convertToValue() {
    for (size_t i = 0; i < values.size(); i++) {
        try {
            if (values[i].substr(0, 2) == "0x") {
                convertedValues[i] = stoull(values[i], nullptr, 16);
            } else {
                convertedValues[i] = stoull(values[i]);
            }
        } catch (const invalid_argument& e) {
            cerr << "Conversion error in line " << i << ": " << e.what() << endl;
        }
    }

    return convertedValues.data();
}

void Parser::parser_setValuesToConfigurationStructure() {
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys[i] == "Eth.LineRate") {
            p_config->LineRate = static_cast<uint16_t>(convertedValues[i]);
        } else if (keys[i] == "Eth.CaptureSizeMs") {
            p_config->CaptureSizeMs = static_cast<uint16_t>(convertedValues[i]);
        } else if (keys[i] == "Eth.DestAddress") {
            p_config->DestAddress = convertedValues[i];
        }
        // Add other keys here as needed
    }
}

void Parser::parser_extractSetupParameters() {
    parser_readFile();
    parser_removeComments();
    parser_extractKeysAndValues();
    parser_removeSpaces();
    parser_convertToValue();
    parser_setValuesToConfigurationStructure();
}

void Parser::parser_extractKeysAndValues() {
    for (size_t i = 0; i < lines.size(); ++i) {
        size_t pos = keys[i].find('=');
        if (pos != string::npos) {
            values[i] = keys[i].substr(pos + 1);
            keys[i] = keys[i].substr(0, pos);
        }
    }

#ifdef DEBUG_MODE_ON
    parser_writeFile("Parser Debug Files/parser_keys.txt", key);
    parser_writeFile("Parser Debug Files/parser_values.txt", value);
#endif
}

void Parser::parser_removeSpaces() {
    for (auto& key : keys) {
        key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
    }

    for (auto& value : values) {
        value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
    }

#ifdef DEBUG_MODE_ON
    parser_writeFile("Parser Debug Files/parser_keysNoSpaces.txt", key);
    parser_writeFile("Parser Debug Files/parser_valuesNoSpaces.txt", value);
#endif
}
