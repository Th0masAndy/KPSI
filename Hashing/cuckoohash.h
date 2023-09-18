#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include "hash.h"

class Cuckoohash {
   private:
    std::vector<std::uint64_t> hashtable_;
    std::vector<bool> exist_map;
    std::vector<std::uint64_t> stash;
    void InsertOne(std::uint64_t element);

   public:
    Cuckoohash(size_t nbins);
    ~Cuckoohash();
    void Insert(std::vector<std::uint64_t> elements);
    std::vector<std::uint64_t> GetVector();
    size_t GetStashSize();
    std::size_t max_try = 1024;
};

Cuckoohash::Cuckoohash(size_t nbins) {
    hashtable_.resize(nbins);
    exist_map.assign(nbins, false);
}

Cuckoohash::~Cuckoohash() {}

void Cuckoohash::InsertOne(std::uint64_t element) {
    auto times = 0;
    auto cur_element = element;
    while (++times < 1024) {
        auto pos = Hash::Murmur_32(cur_element >> 32, cur_element & 0xffffffff);
        auto p1 = (pos >> 20) & 0x3ff;
        auto p2 = (pos >> 10) & 0x3ff;
        auto p3 = pos & 0x3ff;
        unsigned int p[] = {p1, p2, p3};

        for (auto &pi : p) {
            if (!exist_map[pi]) {
                hashtable_[pi] = cur_element;
                exist_map[pi] = true;
                return;
            }
        }

        auto cur_pos = p[rand() % 3];
        auto tmp = hashtable_[cur_pos];
        hashtable_[cur_pos] = cur_element;
        cur_element = tmp;
    }
    stash.push_back(cur_element);
}

void Cuckoohash::Insert(std::vector<std::uint64_t> elements) {
    for (auto &e : elements) {
        InsertOne(e);
    }
}

std::vector<std::uint64_t> Cuckoohash::GetVector() { return hashtable_; }

size_t Cuckoohash::GetStashSize() { return stash.size(); }