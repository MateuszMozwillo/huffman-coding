#ifndef HOFFMAN_HPP
#define HOFFMAN_HPP

#include <string>
#include <vector>
#include <unordered_map>

std::string load_text_from_file(const std::string& file_path);
std::vector<char> text_to_tokens(const std::string& to_tokenize);
std::unordered_map<char, std::string> create_dictionary_from_tokens(const std::vector<char>& chars);
std::string dictionary_to_string(const std::unordered_map<char, std::string> &dict);

#endif
