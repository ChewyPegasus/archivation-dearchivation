#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <string>
#include <vector>
#include <iostream>

class Archiver {
public:
    void compress(const std::string& input_file, const std::string& output_file) const;
    void decompress(const std::string& archive_file, const std::string& output_file) const;

private:

    struct pair_data {
        std::string chars;
        int count;

        pair_data() : chars(""), count(0) {}
        pair_data(const std::string& c, int n) : chars(c), count(n) {}
    };

    std::vector<pair_data> compress_string(const std::string& s) const;
    void normalize_pairs(std::vector<pair_data>& v) const;
    std::vector<pair_data> split_large_pair(const pair_data& a) const;
    void insert_range(std::vector<pair_data>& dest, const std::vector<pair_data>& src, int pos) const;
    void write_binary(const std::vector<std::vector<pair_data>>& data, std::ostream& out) const;

    int binary_to_int(const std::string& binary_str) const;
    void write_decompressed(std::ostream& out, const std::vector<std::vector<pair_data>>& data) const;
};

#endif // ARCHIVER_H
