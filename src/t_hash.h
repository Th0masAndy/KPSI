#include <memory>
#include <string>
#include <vector>

namespace ENCRYPTO {

// Performs cuckoo hashing of party's inputs
std::vector<std::uint64_t> cuckoo_hash(std::size_t nbins,
                                       const std::vector<std::uint64_t> &elements);

// Performs simple hashing of party's inputs
std::vector<std::vector<std::uint64_t>> simple_hash(std::size_t nbins,
                                                    const std::vector<std::uint64_t> &elements);
std::vector<uint64_t> GenerateSequentialElements(const std::size_t n);

}  // namespace ENCRYPTO