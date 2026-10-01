#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "../src/huffman.hpp"

using namespace std;
using Clock = chrono::steady_clock;

static double time_ms(int runs, const function<void()>& fn) {
    vector<double> samples;
    for (int i = 0; i < runs; i++) {
        auto start = Clock::now();
        fn();
        samples.push_back(chrono::duration<double, milli>(Clock::now() - start).count());
    }
    sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

static string uniform_text(size_t size, mt19937& rng) {
    uniform_int_distribution<int> dist(0, 255);
    string s(size, '\0');
    for (auto& c : s) c = static_cast<char>(dist(rng));
    return s;
}

static string english_like_text(size_t size, mt19937& rng) {
    const string alphabet = " etaoinshrdlcumwfgypbvkjxqz";
    const vector<double> weights = {18, 12.7, 9.1, 8.2, 7.5, 7.0, 6.7, 6.3, 6.1, 6.0, 4.3, 4.0, 2.8, 2.8,
                                    2.4, 2.4, 2.2, 2.0, 2.0, 1.9, 1.5, 1.0, 0.8, 0.2, 0.2, 0.1, 0.1};
    discrete_distribution<int> dist(weights.begin(), weights.end());
    string s(size, '\0');
    for (auto& c : s) c = alphabet[dist(rng)];
    return s;
}

static void run_case(const char* name, const string& text) {
    const int runs = text.size() >= (1 << 24) ? 3 : 7;
    const double mb = text.size() / (1024.0 * 1024.0);

    vector<char> tokens;
    unordered_map<char, string> dict;
    string packed;
    int padded_bits = 0;

    double t_tokens = time_ms(runs, [&] { tokens = text_to_tokens(text); });
    double t_dict = time_ms(runs, [&] { dict = create_dictionary_from_tokens(tokens); });
    double t_encode = time_ms(runs, [&] { packed = encode_tokens(tokens, dict, padded_bits); });
    double total = t_tokens + t_dict + t_encode;

    printf("%-14s %9.2f MB | tokens %8.2f ms | dict %8.2f ms | encode %8.2f ms | total %8.2f ms | %7.1f MB/s | ratio %5.1f%%\n",
           name, mb, t_tokens, t_dict, t_encode, total, mb / (total / 1000.0),
           100.0 * packed.size() / text.size());
}

int main() {
    mt19937 rng(42);
    const vector<size_t> sizes = {1 << 10, 1 << 20, 1 << 24};

    printf("Huffman benchmark (median of runs, single thread)\n\n");
    for (size_t size : sizes) {
        run_case("english-like", english_like_text(size, rng));
        run_case("uniform-bytes", uniform_text(size, rng));
        run_case("single-char", string(size, 'a'));
    }
    return 0;
}
