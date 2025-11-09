#include "archiver.h"
#include <fstream>
#include <algorithm>
#include <bitset>
#include <stdexcept>

void Archiver::compress(const std::string& input_file, const std::string& output_file) const {
    std::ifstream fin(input_file);
    std::ofstream fout(output_file);

    if (!fin || !fout) {
        throw std::runtime_error("Failed to open files for compression");
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    std::vector<std::vector<pair_data>> compressed_data;
    compressed_data.reserve(lines.size());

    for (const auto& str : lines) {
        compressed_data.push_back(compress_string(str));
    }

    write_binary(compressed_data, fout);
}

void Archiver::decompress(const std::string& archive_file, const std::string& output_file) const {
    std::ifstream fin(archive_file);
    std::ofstream fout(output_file);

    if (!fin || !fout) {
        throw std::runtime_error("Failed to open files for decompression");
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    std::vector<std::vector<int>> parsed_data(lines.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string current_line = lines[i];
        size_t pos = 0;
        while ((pos = current_line.find(' ')) != std::string::npos) {
            parsed_data[i].push_back(binary_to_int(current_line.substr(0, pos)));
            current_line.erase(0, pos + 1);
        }
    }

    std::vector<std::vector<pair_data>> result(lines.size());
    for (size_t i = 0; i < parsed_data.size(); ++i) {
        size_t j = 0;
        while (j < parsed_data[i].size()) {
            pair_data pair;
            pair.count = parsed_data[i][j];
            
            if (pair.count > 128) {
                if (++j < parsed_data[i].size()) {
                    pair.chars += static_cast<char>(parsed_data[i][j]);
                }
            } else {
                for (int k = 0; k < pair.count && ++j < parsed_data[i].size(); ++k) {
                    pair.chars += static_cast<char>(parsed_data[i][j]);
                }
            }
            ++j;
            result[i].push_back(pair);
        }
    }
    
    write_decompressed(fout, result);
}

std::vector<Archiver::pair_data> Archiver::compress_string(const std::string& s) const {
    if (s.empty()) return {};
    
    std::vector<pair_data> result;
    
    size_t i = 0;
    while (i < s.size()) {
        size_t j = i;
        while (j < s.size() && s[j] == s[i]) {
            ++j;
        }
        result.emplace_back(std::string(1, s[i]), j - i);
        i = j;
    }
    
    for (int i = result.size() - 1; i > 0; --i) {
        if (result[i].count == 1 && result[i - 1].count == 1) {
            result[i - 1].chars += result[i].chars;
            result[i - 1].count += result[i].count;
            result[i].count = 0;
        }
    }
    
    result.erase(
        std::remove_if(result.begin(), result.end(), [](const pair_data& p) { return p.count == 0; }),
        result.end()
    );
    
    normalize_pairs(result);
    
    for (auto& pair : result) {
        if (pair.count == 27) pair.count += 100;
        if (pair.chars.length() == 1) pair.count += 128;
    }
    
    return result;
}

void Archiver::normalize_pairs(std::vector<pair_data>& v) const {
    int i = 0;
    while (i < static_cast<int>(v.size())) {
        if (v[i].count > 126) {
            auto replacements = split_large_pair(v[i]);
            v.erase(v.begin() + i);
            insert_range(v, replacements, i);
        }
        ++i;
    }
}

std::vector<Archiver::pair_data> Archiver::split_large_pair(const pair_data& a) const {
    constexpr int MAX_LENGTH = 126;
    int chunks = (a.count + MAX_LENGTH - 1) / MAX_LENGTH;
    std::vector<pair_data> result;
    
    if (a.chars.size() > 1) {
        result.reserve(chunks);
        for (int i = 0; i < chunks - 1; ++i) {
            result.emplace_back(a.chars.substr(i * MAX_LENGTH, MAX_LENGTH), MAX_LENGTH);
        }
        if (a.chars.length() % MAX_LENGTH != 0) {
            result.emplace_back(a.chars.substr((chunks - 1) * MAX_LENGTH), a.chars.length() % MAX_LENGTH);
        }
    } else {
        for (int i = 0; i < chunks - 1; ++i) {
            result.emplace_back(a.chars, MAX_LENGTH);
        }
        if (a.count % MAX_LENGTH != 0) {
            result.emplace_back(a.chars, a.count % MAX_LENGTH);
        }
    }
    return result;
}

void Archiver::insert_range(std::vector<pair_data>& dest, const std::vector<pair_data>& src, int pos) const {
    dest.reserve(dest.size() + src.size());
    dest.insert(dest.begin() + pos, src.begin(), src.end());
}

void Archiver::write_binary(const std::vector<std::vector<pair_data>>& data, std::ostream& out) const {
    for (const auto& row : data) {
        for (const auto& pair : row) {
            out << std::bitset<8>(pair.count) << ' ';
            for (char c : pair.chars) {
                out << std::bitset<8>(c) << ' ';
            }
        }
        out << '\n';
    }
}

int Archiver::binary_to_int(const std::string& binary_str) const {
    return std::stoi(binary_str, nullptr, 2);
}

void Archiver::write_decompressed(std::ostream& out, const std::vector<std::vector<pair_data>>& data) const {
    for (const auto& row : data) {
        for (const auto& pair : row) {
            if (pair.count > 128) {
                for (int i = 0; i < pair.count - 128; ++i) {
                    out << pair.chars;
                }
            } else {
                out << pair.chars;
            }
        }
        out << '\n';
    }
}
