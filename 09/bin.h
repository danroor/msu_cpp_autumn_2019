#pragma once
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <tuple>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <queue>

namespace bin {

    static constexpr size_t BUFSZ = 128 * 1024; //1 MB of uint64_t numbers

    static constexpr char FILE_ERROR[]  = "Error: unable to open file";
    static constexpr char SORTED_FILE[] = "SortedPortionOfData_";
    static constexpr char FORMAT[]      = ".bin";

    static constexpr decltype(std::ios::in) INFLAGS  = std::ios::in  | std::ios::binary;
    static constexpr decltype(std::ios::in) OUTFLAGS = std::ios::out | std::ios::binary;

    void generate_random_file(std::string &name, size_t sz) {
        std::srand(std::time(nullptr));
        uint64_t cur;

        std::ofstream f(name, OUTFLAGS);
        if (!f)
            throw std::runtime_error(FILE_ERROR);

        for (size_t i = 0; i < sz; ++i) {
            cur = std::rand();
            f.write( (char *) &cur, sizeof(cur)  );
        }
    }

    //splits sorted portions of data from fin file to several files
    //Writes the quantity of files to n_files (n_files must be equal to zero)
    void split_data(std::ifstream &fin, size_t &n_files, std::mutex &mut) { 

        uint64_t *buf = new uint64_t[BUFSZ];
        uint64_t cur;
        size_t size, cnt;

        while (true) {
            size = 0;
            for (size_t i = 0; i < BUFSZ; i++) {
                std::unique_lock <std::mutex> Lock(mut);
                if ( !fin.read( (char *) &cur, sizeof(cur)  ) ) {
                    if (size == 0) {
                        delete [] buf;
                        return;
                    }
                    break;
                }
                Lock.unlock();
                buf[size++] = cur;
            }

            std::sort(buf, buf + size);
            std::stringstream out;

            std::unique_lock <std::mutex> Lock1(mut);

            cnt = n_files;
            n_files++;
            out << SORTED_FILE << cnt << FORMAT;

            Lock1.unlock();

            std::ofstream fout(out.str(), OUTFLAGS);

            for (size_t i = 0; i < size; i++) {
                fout.write((char *) &buf[i], sizeof(buf[i]));
            }
        }
    }

    //merges n_files sorted files to one sorted file
    void merge_data(std::ofstream &fout, size_t n_files) {

        using T = std::pair < uint64_t, size_t >;
        std::priority_queue < T, std::vector<T>, std::greater <T> > queue;
        std::vector <std::ifstream> files;
        uint64_t buf, n_file;

        for (size_t i = 0; i < n_files; ++i) {
            std::stringstream strm;
            strm << SORTED_FILE << i << FORMAT;
            files.emplace_back(strm.str(), INFLAGS);

            if (!files[i]) {
                files.clear();
                throw std::runtime_error(FILE_ERROR);
            }

            if (files[i].read((char *) &buf, sizeof(buf))) {
                queue.push(std::make_pair(buf, i));
            }
        }

        while (!queue.empty()) {
            std::tie(buf, n_file) = queue.top();
            queue.pop();

            fout.write((char *) &buf, sizeof(buf));

            if (files[n_file].read((char *) &buf, sizeof(buf))) {
                queue.push(std::make_pair(buf, n_file));
            }
        }

        for (size_t i = 0; i < n_files; ++i) {
            std::stringstream strm;
            strm << SORTED_FILE << i << FORMAT;
            std::remove(strm.str().c_str());
        }
    }

    void sort(const std::string &from, const std::string &to) {

        std::ifstream fin(from, INFLAGS);
        if (!fin)
            throw std::runtime_error(FILE_ERROR);

        std::ofstream fout(to, OUTFLAGS);
        if (!fout)
            throw std::runtime_error(FILE_ERROR);

        size_t cnt = 0;
        std::mutex mut;

        std::thread t1(split_data, std::ref(fin), std::ref(cnt), std::ref(mut));
        std::thread t2(split_data, std::ref(fin), std::ref(cnt), std::ref(mut));
        t1.join();
        t2.join();

        merge_data(fout, cnt);
    }

};
