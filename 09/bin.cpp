#include "bin.h"

void bin::generate_random_file(const std::string &name, size_t sz) {
    using namespace bin;

    std::ofstream f(name, OUTFLAGS);
    if (!f)
        throw std::runtime_error(FILE_ERROR);

    std::random_device gen;
    
    uint64_t *buf = new uint64_t[BUFSZ_RD_WRT];
    size_t n = sz / BUFSZ_RD_WRT;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < BUFSZ_RD_WRT; ++j) {
            buf[j] = ( static_cast <uint64_t> (gen())  << 32u ) + gen();
        }
        f.write( (char *) buf, sizeof(buf[0]) * BUFSZ_RD_WRT);
    }

    n = sz % BUFSZ_RD_WRT;

    if (n != 0) {
        for (size_t j = 0; j < n; ++j) {
            buf[j] = ( static_cast <uint64_t> (gen())  << 32u ) + gen();
        }
        f.write( (char *) buf, sizeof(buf[0]) * n  );
    }

    delete [] buf;
}


//splits sorted portions of data from fin file to several files
//Writes the quantity of files to n_files (n_files must be equal to zero)
void bin::split_data(std::ifstream &fin, size_t &n_files, std::mutex &mut) { 
    using namespace bin;

    uint64_t *buf = new uint64_t[BUFSZ_STORE];
    
    size_t rdwr_sz = sizeof(buf[0]) * BUFSZ_RD_WRT;
    size_t blocks = BUFSZ_STORE / BUFSZ_RD_WRT;

    size_t cnt, n_blocks, size;

    while (true) {
        std::stringstream out;
        size = 0;

        std::unique_lock <std::mutex> Lock(mut);

        for (size_t i = 0; i < blocks; ++i) {
            if ( !fin.read((char *) (buf + i * BUFSZ_RD_WRT), rdwr_sz) ) {
                size += fin.gcount(); 
                break;
            }
            size += fin.gcount(); 
        }
        
        if (size == 0)
            break;

        cnt = n_files;
        n_files++;
        out << SORTED_FILE << cnt << FORMAT;

        Lock.unlock();    

        size /= sizeof(buf[0]);
        n_blocks = size == BUFSZ_STORE ? blocks : size / BUFSZ_RD_WRT;

        std::sort(buf, buf + size); 
        std::ofstream fout(out.str(), OUTFLAGS);

        for (size_t i = 0; i < n_blocks; ++i)
            fout.write((char *) (buf + i * BUFSZ_RD_WRT), rdwr_sz);

        size %= BUFSZ_RD_WRT;

        if (size > 0)
            fout.write((char *) (buf + n_blocks * BUFSZ_RD_WRT), size * sizeof(buf[0]));
    }

    delete [] buf;
}

//merges n_files sorted files to one sorted file
void bin::merge_data(std::ofstream &fout, size_t n_files) {
    using namespace bin;

    using T = std::pair < uint64_t, size_t >;
    std::priority_queue < T, std::vector<T>, std::greater <T> > queue;
    std::vector <std::ifstream> files;

    for (size_t i = 0; i < n_files; ++i) {
        std::stringstream strm;
        strm << SORTED_FILE << i << FORMAT;
        files.emplace_back(strm.str(), INFLAGS);

        if (!files[i]) {
            files.clear();
            throw std::runtime_error(FILE_ERROR);
        }

        uint64_t buf;
        if (files[i].read((char *) &buf, sizeof(buf))) {
            queue.push(std::make_pair(buf, i));
        }
    }

    size_t size = 0;
    uint64_t *buf = new uint64_t[BUFSZ_RD_WRT];

    while (!queue.empty()) {
        auto [tmp, n_file] = queue.top();
        queue.pop();

        if (size == BUFSZ_RD_WRT) {
            fout.write((char *) buf, sizeof(buf[0]) * BUFSZ_RD_WRT);
            size = 0;
        }

        buf[size++] = tmp;

        if (files[n_file].read((char *) &tmp, sizeof(tmp))) {
            queue.push(std::make_pair(tmp, n_file));
        }
    }

    if (size > 0)
        fout.write((char *) buf, sizeof(buf[0]) * size);

    delete [] buf;

    for (size_t i = 0; i < n_files; ++i) {
        std::stringstream strm;
        strm << SORTED_FILE << i << FORMAT;
        std::remove(strm.str().c_str());
    }
}

void bin::sort(const std::string &from, const std::string &to) {
    using namespace bin;

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
