# Huffman coding

File compression using Huffman coding in C++17.

## Requirements

- `g++` with C++17 support
- `make`

## Building

```sh
make build
```

This produces the `main` executable.

## Usage

Compress a file:

```sh
./main -c <input_file> [output_file]
```

The output goes to `coded.out` by default. Example:

```sh
./main -c test.txt coded.out
```

`make run` is a shortcut that compresses `test.txt` into `coded.out` (you need to create `test.txt` yourself).

Decompression (`-u`) is not implemented yet.

## Benchmark

```sh
make bench
```

The benchmark generates test data (English-like letter distribution, uniformly random bytes, and a single repeated character) at 1 KB, 1 MB and 16 MB. For each data set it prints the median time of each stage (tokenization, dictionary building, encoding), the throughput in MB/s, and the size of the encoded data relative to the original (header not included).

### Results

Intel Core i9-10850K @ 3.60 GHz, WSL2, g++ 15.2 `-O2`, single thread, median of runs:

| Data | Size | Tokens | Dictionary | Encode | Total | Throughput | Ratio |
|---|---:|---:|---:|---:|---:|---:|---:|
| English-like | 1 KB | 0.00 ms | 0.01 ms | 0.01 ms | 0.03 ms | 36.0 MB/s | 52.3% |
| Uniform bytes | 1 KB | 0.00 ms | 0.09 ms | 0.02 ms | 0.10 ms | 9.4 MB/s | 98.0% |
| Single char | 1 KB | 0.00 ms | 0.01 ms | 0.01 ms | 0.01 ms | 74.5 MB/s | 12.5% |
| English-like | 1 MB | 0.24 ms | 9.48 ms | 16.48 ms | 26.20 ms | 38.2 MB/s | 52.3% |
| Uniform bytes | 1 MB | 0.04 ms | 6.47 ms | 15.08 ms | 21.60 ms | 46.3 MB/s | 100.0% |
| Single char | 1 MB | 0.03 ms | 6.41 ms | 7.17 ms | 13.61 ms | 73.5 MB/s | 12.5% |
| English-like | 16 MB | 5.37 ms | 107.17 ms | 268.06 ms | 380.60 ms | 42.0 MB/s | 52.3% |
| Uniform bytes | 16 MB | 4.95 ms | 103.20 ms | 238.18 ms | 346.33 ms | 46.2 MB/s | 100.0% |
| Single char | 16 MB | 1.41 ms | 102.66 ms | 108.76 ms | 212.83 ms | 75.2 MB/s | 12.5% |

## Cleaning up

```sh
make clean
```
