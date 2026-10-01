#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "huffman.hpp"

using namespace std;

static int compress(const string& input_path, const string& output_path) {
    const string text = load_text_from_file(input_path);
    const vector<char> tokens = text_to_tokens(text);
    const auto dictionary = create_dictionary_from_tokens(tokens);

    int padded_bits = 0;
    const string coded_as_bytes = encode_tokens(tokens, dictionary, padded_bits);
    const string map_as_string = dictionary_to_string(dictionary);

    ofstream output(output_path, ios::binary);
    if (!output) {
        cerr << "Error: Could not open file for writing: " << output_path << endl;
        return 1;
    }
    output << map_as_string.size() << '\n' << coded_as_bytes.size() << '\n' << padded_bits << '\n' << map_as_string << '\n';
    output.write(coded_as_bytes.data(), coded_as_bytes.size());

    cout << input_path << ": " << text.size() << " B -> " << output_path << ": "
         << static_cast<size_t>(output.tellp()) << " B\n";
    return 0;
}

static void usage(const char* prog) {
    cerr << "Usage:\n"
         << "  " << prog << " -c <input> [output]   compress (default output: coded.out)\n"
         << "  " << prog << " -u <input> [output]   decompress (not implemented yet)\n";
}

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    try {
        if (strcmp(argv[1], "-c") == 0) {
            return compress(argv[2], argc > 3 ? argv[3] : "coded.out");
        }
        if (strcmp(argv[1], "-u") == 0) {
            cerr << "Decompression is not implemented yet\n";
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    usage(argv[0]);
    return 1;
}
