#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include "hash.h"
class Simplehash {
   private:
    std::vector<std::vector<std::uint64_t>> hashtable_;

   public:
    Simplehash(size_t nbins);
    ~Simplehash();
    void Insert(std::vector<std::uint64_t> elements);
    std::vector<std::vector<std::uint64_t>> GetVector();
    std::size_t max_bin_size = 0;
};

Simplehash::Simplehash(size_t nbins) { hashtable_.resize(nbins); }

Simplehash::~Simplehash() {}

void Simplehash::Insert(std::vector<std::uint64_t> elements) {
    for (auto &e : elements) {
        auto pos = Hash::Murmur_32(e >> 32, e & 0xffffffff);
        auto p1 = pos >> 22;
        auto p2 = (pos >> 10) & 0x3ff;
        auto p3 = pos & 0x3ff;
        hashtable_[p1].push_back(e);
        max_bin_size = std::max(max_bin_size, hashtable_[p1].size());
        hashtable_[p2].push_back(e);
        max_bin_size = std::max(max_bin_size, hashtable_[p2].size());
        hashtable_[p3].push_back(e);
        max_bin_size = std::max(max_bin_size, hashtable_[p3].size());
    }
}

std::vector<std::vector<std::uint64_t>> Simplehash::GetVector() { return hashtable_; }