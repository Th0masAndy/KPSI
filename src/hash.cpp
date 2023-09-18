#include <algorithm>
#include <chrono>

#include <iostream>

#include <random>
#include <ratio>

#include "cuckoo_hashing/cuckoo_hashing.h"
#include "simple_hashing/simple_hashing.h"

namespace ENCRYPTO {

using milliseconds_ratio = std::ratio<1, 1000>;
using duration_millis = std::chrono::duration<double, milliseconds_ratio>;

/*
 * Perform stashless cuckoo hashing, 1 element per bin
 */
std::vector<std::uint64_t> cuckoo_hash(std::size_t nbins,
                                       const std::vector<std::uint64_t>& elements) {
    const auto hashing_start_time = std::chrono::system_clock::now();

    ENCRYPTO::CuckooTable cuckoo_table(nbins);
    cuckoo_table.SetNumOfHashFunctions(3);
    cuckoo_table.Insert(elements);
    cuckoo_table.MapElements();
    // cuckoo_table.Print();

    if (cuckoo_table.GetStashSize() > 0u) {
        std::cerr << "[Error] Stash of size " << cuckoo_table.GetStashSize() << " occured\n";
    }

    auto cuckoo_table_v = cuckoo_table.AsRawVector();

    const auto hashing_end_time = std::chrono::system_clock::now();
    const duration_millis hashing_duration = hashing_end_time - hashing_start_time;

    return cuckoo_table_v;
}

/*
 * Perform simple hashing, multiple elements per bin
 */
std::vector<std::vector<std::uint64_t>> simple_hash(std::size_t nbins,
                                                    const std::vector<std::uint64_t>& elements) {
    const auto hashing_start_time = std::chrono::system_clock::now();

    ENCRYPTO::SimpleTable simple_table(nbins);
    simple_table.SetNumOfHashFunctions(3);
    simple_table.Insert(elements);
    simple_table.MapElements();
    // simple_table.Print();
    std::cout << "max_observed_bin_size: " << simple_table.GetStatistics().max_observed_bin_size_
              << std::endl;
    auto simple_table_v = simple_table.AsRaw2DVector();
    for (auto& vec : simple_table_v) {
        std::cout << vec.size() << " ";
    }
    // context.simple_table = simple_table_v;
    const auto hashing_end_time = std::chrono::system_clock::now();
    const duration_millis hashing_duration = hashing_end_time - hashing_start_time;

    return simple_table_v;
}
std::vector<uint64_t> GenerateSequentialElements(const std::size_t n) {
    std::vector<uint64_t> elements(n);
    std::size_t i = 0;
    std::generate(elements.begin(), elements.end(), [&i]() mutable { return i++; });

    for (auto& e : elements) {
        e = HashingTable::ElementToHash(e) & 0x1FFFFFFFFFFFFFFFull;
    }

    return elements;
}
}  // namespace ENCRYPTO