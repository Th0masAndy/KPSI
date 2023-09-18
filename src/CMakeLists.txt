# If not called from root CMakeLists.txt
if(NOT DEFINED SEAL_BUILD_EXAMPLES)
    set(SEAL_BUILD_EXAMPLES ON)

    # Import Microsoft SEAL
    find_package(SEAL 4.1.1 EXACT REQUIRED)
endif()

set(SEAL_USE_ZSTD ON)

# add_library(hash hash.cpp)

# target_compile_options(hash
# PRIVATE
# -march=native
# #-std=c++11
# -ffunction-sections -mpclmul -mbmi2 -maes
# -mavx -msse2 -msse3 -msse4.1 -mrdseed
# #-march=haswell
# -Wall -Wno-strict-overflow -Wno-ignored-attributes -Wno-parentheses)

# target_link_libraries(hash INTERFACE Hashing)

# target_include_directories(hash PUBLIC
# ${PSI_ANALYTICS_SOURCE_ROOT}/src
# ${PSI_ANALYTICS_SOURCE_ROOT}/Hashing)

# include_directories(${PSI_ANALYTICS_SOURCE_ROOT}/Hashing)
# add_subdirectory(HashingTables)
include_directories(${PSI_ANALYTICS_SOURCE_ROOT}/Hashing)
include_directories(${PSI_ANALYTICS_SOURCE_ROOT}/src)

if(SEAL_BUILD_EXAMPLES)
    add_executable(kpsi main.cpp)

    target_include_directories(kpsi PUBLIC ${PSI_ANALYTICS_SOURCE_ROOT}/Hashing ${PSI_ANALYTICS_SOURCE_ROOT}/src)

    # target_link_libraries(kpsi PUBLIC hash)
    set_target_properties(kpsi
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
    )

    if(TARGET SEAL::seal)
        target_link_libraries(kpsi PRIVATE SEAL::seal)
    elseif(TARGET SEAL::seal_shared)
        target_link_libraries(kpsi PRIVATE SEAL::seal_shared)
    else()
        message(FATAL_ERROR "Cannot find target SEAL::seal or SEAL::seal_shared")
    endif()
endif()
