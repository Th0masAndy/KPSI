#include <vector>
#include "hash.h"

class BloomFilter {
   public:
    explicit BloomFilter(std::uint64_t m_bits, std::uint64_t seed)
        : storage(m_bits), m_bits(m_bits), seed(seed) {}

    void insert(std::vector<std::uint64_t> elements) {
        for (auto &e : elements) {
            storage.at(hash(e) % m_bits) = true;
        }
    }
    bool contains(std::uint64_t element) { return storage.at(hash(element)); }

    unsigned int hash(std::uint64_t input) {
        auto h1 = Hash::Murmur_32(input >> 32, seed);
        auto h2 = Hash::Murmur_32(input & 0xffffffff, seed);
        return h1 + h2;
    }

    unsigned int cnt() {
        int res = 0;
        for (auto i = 0; i < storage.size(); i++) {
            if (storage[i] == true) {
                res += 1;
            }
        }
        return res;
    }

    std::vector<int> AsVector() {
        std::vector<int> res(storage.size(), 0);
        for (auto i = 0; i < storage.size(); i++) {
            if (storage[i] == true) {
                res[i] = 1;
            }
        }
        return res;
    }

   private:
    std::vector<bool> storage;
    std::uint64_t m_bits;
    std::uint64_t seed;
};
