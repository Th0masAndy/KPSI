#pragma once

#include <algorithm>
#include <vector>

#include <iostream>

#include <random>

namespace Hash {

unsigned int Murmur_32(unsigned int key, unsigned int seed) {
    auto hash = seed;
    key *= 0xcc9e2d51;
    key = (key << 15) | (key >> 17);
    key *= 0x1b873593;

    hash ^= key;
    hash = (hash << 13) | (hash >> 19);
    hash *= 5;
    hash += 0xe6546b64;

    hash ^= 4;

    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    return hash;
}

}  // namespace Hash
