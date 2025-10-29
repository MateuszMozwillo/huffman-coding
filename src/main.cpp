#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <bitset>

#include "hoffman.hpp"

using namespace std;

int main(int argc, const char* argv[]) {

    if (argc == 2) {
        //compress
        if (strcmp(argv[1], "-c")) {

        }
        //uncompress
        else if (strcmp(argv[1], "-u")) {

        }
    }
    const size_t len = load_text_from_file("./test.txt").size();
    std::vector<char> tokens = text_to_tokens(load_text_from_file("./test.txt"));
    char chars[len];
    auto dictionary = create_dictionary_from_tokens(tokens);

    string coded = "";

    for (auto const& token : tokens) {
        coded += dictionary[token];
    }

    string coded_as_bytes  = "";
    int paded_bits = 0;
    for (size_t i = 0; i < coded.size(); i+=8) {
        string byte = coded.substr(i, 8);
        if (byte.size() < 8) {
            paded_bits = 8 - byte.length();
            byte.append(paded_bits, '0');
        }
        bitset<8> bits(byte);
        unsigned char byte_to_write = static_cast<unsigned char>(bits.to_ulong());
        coded_as_bytes.push_back(byte_to_write);
    }

    auto map_as_string = dictionary_to_string(dictionary);

    ofstream output("coded.out");
    if (!output) {
        cerr << "Error: Could not open file for writing!" << endl;
        return 1;
    }
    output << map_as_string.size() << '\n' << coded_as_bytes.size()  << '\n' << paded_bits << '\n' <<map_as_string << '\n';
    output.write(reinterpret_cast<const char*>(coded_as_bytes.data()),coded_as_bytes.size());
    // output.write(reinterpret_cast<const char*>(coded_as_bytes.data()), coded_as_bytes.size());
    output << coded_as_bytes;
    output.close();


    // cout << coded_as_bytes;

    return 0;
}
