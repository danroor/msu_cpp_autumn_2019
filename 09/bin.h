#ifndef BIN_H
#define BIN_H

#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <random>

namespace bin {

    //1 MB of uint64_t numbers to store in one temporary file
    constexpr size_t BUFSZ_STORE  = 1024 * 128;

    //size of reading/writing buffer
    constexpr size_t BUFSZ_RD_WRT = 1024 * 32;

    constexpr char FILE_ERROR[]  = "Error: unable to open file";
    constexpr char SORTED_FILE[] = "SortedPortionOfData_";
    constexpr char FORMAT[]      = ".bin";

    constexpr std::ios_base::openmode INFLAGS  = std::ios::in  | std::ios::binary;
    constexpr std::ios_base::openmode OUTFLAGS = std::ios::out | std::ios::binary;

    void generate_random_file(const std::string &name, size_t sz);

    //splits sorted portions of data from fin file to several files
    //Writes the quantity of files to n_files (n_files must be equal to zero)
    void split_data(std::ifstream &fin, size_t &n_files, std::mutex &mut);

    //merges n_files sorted files to one sorted file
    void merge_data(std::ofstream &fout, size_t n_files);

    void sort(const std::string &from, const std::string &to);

};

#endif
