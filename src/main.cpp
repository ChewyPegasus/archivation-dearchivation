#include "archiver/archiver.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <fstream>

#ifdef _WIN32
    #define PAUSE() system("pause")
#else
    #define PAUSE() do { std::cout << "Press Enter to continue..."; std::cin.get(); } while(0)
#endif

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    Archiver archiver;

    try {
        std::string input_file = "input.txt";
        std::string archive_file = "archived.txt";
        std::string output_file = "output.txt";

        if (!fs::exists(input_file)) {
            std::cerr << "Error: " << input_file << " not found. Creating a sample file.\n";
            std::ofstream sample(input_file);
            sample << "AAAAAABBBBBCCCCCDDDDD\nHello World!!!\n";
            sample.close();
        }

        std::cout << "Compressing '" << input_file << "' to '" << archive_file << "'...\n";
        auto start = std::chrono::high_resolution_clock::now();
        archiver.compress(input_file, archive_file);
        auto mid = std::chrono::high_resolution_clock::now();
        std::cout << "✓ Done in " << std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count() << " ms\n\n";

        PAUSE();
        
        std::cout << "\nDecompressing '" << archive_file << "' to '" << output_file << "'...\n";
        archiver.decompress(archive_file, output_file);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "✓ Done in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count() << " ms\n";

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
