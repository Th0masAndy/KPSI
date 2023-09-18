// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "bloom.h"
#include "cuckoohash.h"
#include "examples.h"
#include "poly.h"
#include "simplehash.h"
#include <cstdlib>

using namespace std;
using namespace seal;

void Multest();

std::thread threads[32];

void funcMulPlain(Evaluator& evaluator, RelinKeys& relin_keys, map<int, Ciphertext>& powerbasis,
                  vector<Plaintext>& coeffs_pt, Ciphertext& big_power, Ciphertext& block) {
    for (int j = 2; j < 64; j++) {
        Ciphertext tmp;
        evaluator.multiply_plain(powerbasis[j / 2], coeffs_pt[j / 2], tmp);
        evaluator.add_inplace(block, tmp);
    }
    evaluator.multiply_inplace(block, big_power);
}

void funcMul(Evaluator& evaluator, RelinKeys& relin_keys, Ciphertext& a, Ciphertext& b, Ciphertext& c) {
    evaluator.multiply(a, b, c);
    evaluator.relinearize_inplace(c, relin_keys);
}

void Multest() {
    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 8192 * 2 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    // auto bits = { 55 , 55 , 55 , 55 , 55 , 55 , 55 , 55 , 55 , 55 , 55 , 55 ,
    // 55 , 55 , 55 }; auto coeff_mod =
    // CoeffModulus::Create(poly_modulus_degree, bits);
    // parms.set_coeff_modulus(coeff_mod);

    /*
    To enable batching, we need to set the plain_modulus to be a prime number
    congruent to 1 modulo 2*poly_modulus_degree. Microsoft SEAL provides a
    helper method for finding such a prime. In this example we create a 20-bit
    prime that supports batching.
    */

    // Modulus t(65537);

    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 17));

    SEALContext context(parms);
    print_parameters(context);
    cout << endl;

    /*
    We can verify that batching is indeed enabled by looking at the encryption
    parameter qualifiers created by SEALContext.
    */
    auto qualifiers = context.first_context_data()->qualifiers();
    cout << "Batching enabled: " << boolalpha << qualifiers.using_batching << endl;

    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    BatchEncoder batch_encoder(context);

    /*
    The total number of batching `slots' equals the poly_modulus_degree, N, and
    these slots are organized into 2-by-(N/2) matrices that can be encrypted and
    computed on. Each slot contains an integer modulo plain_modulus.
    */
    size_t slot_count = batch_encoder.slot_count();
    size_t row_size = slot_count / 2;
    cout << "Plaintext matrix row size: " << row_size << endl;

    vector<uint64_t> one_mat(slot_count, 1ULL);
    Plaintext one_pt;
    Ciphertext one;
    batch_encoder.encode(one_mat, one_pt);
    encryptor.encrypt(one_pt, one);
    Plaintext plain_result;
    vector<uint64_t> pod_result;

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; i++) {
        evaluator.square_inplace(one);
        evaluator.relinearize_inplace(one, relin_keys);

        // cout << i << "    + Noise budget in result: " <<
        // decryptor.invariant_noise_budget(one) << " bits" << endl;
        // decryptor.decrypt(one, plain_result);
        // batch_encoder.decode(plain_result, pod_result);
        // cout << "    + Result plaintext matrix ...... Correct." << endl;
        // print_matrix(pod_result, row_size);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
    }

    for (int i = 0; i < 80; i++) {
        evaluator.add_inplace(one, one);

        // cout << i << "    + Noise budget in result: " <<
        // decryptor.invariant_noise_budget(one) << " bits" << endl;
        // decryptor.decrypt(one, plain_result);
        // batch_encoder.decode(plain_result, pod_result);
        // cout << "    + Result plaintext matrix ...... Correct." << endl;
        // print_matrix(pod_result, row_size);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
    }

    for (int i = 0; i < 4; i++) {
        evaluator.square_inplace(one);
        evaluator.relinearize_inplace(one, relin_keys);

        // cout << i << "    + Noise budget in result: " <<
        // decryptor.invariant_noise_budget(one) << " bits" << endl;
        // decryptor.decrypt(one, plain_result);
        // batch_encoder.decode(plain_result, pod_result);
        // cout << "    + Result plaintext matrix ...... Correct." << endl;
        // print_matrix(pod_result, row_size);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
    }
}

void GenPower(Evaluator& evaluator, RelinKeys& relin_keys, map<int, Ciphertext>& powerbasis, int n) {
    // compute the second layer

    struct mulTask {
        Ciphertext src1;
        Ciphertext src2;
        int dst;
    } task[n];

    vector<int> power;
    for (auto it = powerbasis.begin(); it != powerbasis.end(); it++) {
        power.push_back((*it).first);
    }

    Ciphertext dummy = powerbasis[power[0]];

    auto thread_cnt = 0;
    for (auto i = 0; i < power.size(); i++) {
        for (auto j = 0; j < power.size(); j++) {
            if (powerbasis.find(power[i] + power[j]) == powerbasis.end()) {
                task[thread_cnt].src1 = powerbasis[power[i]];
                task[thread_cnt].src2 = powerbasis[power[j]];
                task[thread_cnt].dst = power[i] + power[j];

                thread_cnt++;
                powerbasis[power[i] + power[j]] = dummy;
            }
        }
    }
    for (auto i = 0; i < thread_cnt; i++) {
        Ciphertext res;

        funcMul(evaluator, relin_keys, task[i].src1, task[i].src2, res);

        powerbasis[task[i].dst] = res;
    }
    // for (auto i = 0;i < thread_cnt;i++) {
    //     threads[i].join();
    // }

    // compute the third layer

    power.clear();
    for (auto it = powerbasis.begin(); it != powerbasis.end(); it++) {
        power.push_back((*it).first);
    }
    thread_cnt = 0;
    for (auto i = 0; i < power.size(); i++) {
        for (auto j = 0; j < power.size(); j++) {
            if (powerbasis.find(power[i] + power[j]) == powerbasis.end()) {
                task[thread_cnt].src1 = powerbasis[power[i]];
                task[thread_cnt].src2 = powerbasis[power[j]];
                task[thread_cnt].dst = power[i] + power[j];
                thread_cnt++;
                powerbasis[power[i] + power[j]] = dummy;
            }
        }
    }
    for (auto i = 0; i < thread_cnt; i++) {
        Ciphertext res;

        // threads[i] = std::thread(funcMul, ref(evaluator), ref(relin_keys),
        // ref(task[i].src1), ref(task[i].src2), ref(res));

        funcMul(evaluator, relin_keys, task[i].src1, task[i].src2, res);

        powerbasis[task[i].dst] = res;
    }
    // for (auto i = 0;i < thread_cnt;i++) {
    //     threads[i].join();
    // }
}

void computeBF(vector<vector<uint64_t>>& table, int seed, LagrangeInterpolation& interpolator,
               vector<vector<int>>& out) {
    for (auto b = 0; b < table.size(); b++) {
        BloomFilter bf(1024, seed);
        bf.insert(table[b]);
        auto y_points = bf.AsVector();
        auto polycoeff = interpolator.Interpolation(y_points);
        out[b * 16 + seed] = polycoeff;
    }
}

int main() {

    // srand(time(NULL));
    // vector<uint64_t> client_inputs((int)(1024 / 1.48));
    // for (auto i = 0; i < client_inputs.size(); i++) {
    //     client_inputs[i] = rand();
    // }

    // Cuckoohash cuckoo_table(1024);
    // cuckoo_table.Insert(client_inputs);
    // if (cuckoo_table.GetStashSize() != 0) {
    //     std::cerr << "Insert elements into cockoo table failed" << endl;
    //     exit(0);
    // }
    // auto cuckoo_table_v = cuckoo_table.GetVector();

    // vector<vector<uint64_t>> server_inputs(80);
    // vector<vector<vector<uint64_t>>> server_tables;
    // for (auto& vec : server_inputs) {
    //     vec.resize(50000);
    //     for (auto& e : vec) {
    //         e = rand();
    //     }
    // }
    // for (auto& vec : server_inputs) {
    //     Simplehash simple_table(1024);
    //     simple_table.Insert(vec);
    //     server_tables.push_back(simple_table.GetVector());
    //     cout << simple_table.max_bin_size << endl;
    // }

    // LagrangeInterpolation interpolator;

    // vector<vector<vector<int>>> poly_coeffs;
    // poly_coeffs.resize(server_tables.size());

    // auto offline_begin = std::chrono::high_resolution_clock::now();

    // for (auto& table : server_tables) {
    //     vector<vector<int>> table_coeff(16384);
    //     for (auto seed = 0; seed < 16; seed++) {
    //         threads[seed] = std::thread(computeBF, ref(table), seed, ref(interpolator), ref(table_coeff));
    //     }
    //     for (auto seed = 0; seed < 16; seed++) {
    //         threads[seed].join();
    //     }
    //     poly_coeffs.push_back(table_coeff);
    // }

    // auto offline_end = std::chrono::high_resolution_clock::now();
    // auto offline_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(offline_end - offline_begin);
    // printf("Time measured: %.3f seconds.\n", offline_elapsed.count() * 1e-9);

    // // transpose coeffs to support SIMD

    // vector<vector<vector<uint64_t>>> coeffs_fhe(80);
    // for (auto& table : poly_coeffs) {
    //     vector<vector<uint64_t>> coeffs_in_table(1024, vector<uint64_t>(16384));
    //     for (auto i = 0; i < table.size(); i++) {
    //         for (auto j = 0; j < table[i].size(); j++) {
    //             coeffs_in_table[j][i] = table[i][j];
    //         }
    //     }
    //     coeffs_fhe.push_back(coeffs_in_table);
    // }

    // offline_end = std::chrono::high_resolution_clock::now();
    // offline_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(offline_end - offline_begin);
    // printf("Time measured: %.3f seconds.\n", offline_elapsed.count() * 1e-9);

    // exit(0);

    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 8192 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 17));

    SEALContext context(parms);
    print_parameters(context);
    cout << endl;

    auto qualifiers = context.first_context_data()->qualifiers();
    cout << "Batching enabled: " << boolalpha << qualifiers.using_batching << endl;

    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    GaloisKeys galois_keys;
    keygen.create_galois_keys(galois_keys);
    BatchEncoder batch_encoder(context);

    size_t slot_count = batch_encoder.slot_count();
    size_t row_size = slot_count / 2;
    cout << "Plaintext matrix row size: " << row_size << endl;

    vector<uint64_t> mat(slot_count, 3ULL);
    Plaintext pt;
    Ciphertext ct;
    batch_encoder.encode(mat, pt);
    encryptor.encrypt(pt, ct);

    vector<uint64_t> zero_mat(slot_count, 0ULL);
    Plaintext zero_pt;
    Ciphertext zero;
    batch_encoder.encode(zero_mat, zero_pt);
    encryptor.encrypt(zero_pt, zero);

    vector<uint64_t> one_mat(slot_count, 1ULL);
    Plaintext one_pt;
    Ciphertext one;
    batch_encoder.encode(one_mat, one_pt);
    encryptor.encrypt(one_pt, one);

    Plaintext plain_result;
    vector<uint64_t> mat_res;

    map<int, Ciphertext> powerbasis;

    int pre_power[] = {1, 5, 7, 8};
    for (auto i = 0; i < 4; i++) {
        vector<uint64_t> mat(slot_count, (i + 1));
        Plaintext pt;
        Ciphertext ct;
        batch_encoder.encode(mat, pt);
        encryptor.encrypt(pt, ct);
        Plaintext plain_result;
        vector<uint64_t> mat_res;
        powerbasis[pre_power[i]] = ct;
    }

    filebuf pre;
    if (pre.open("./cipher_prepower", ios::out) == nullptr) {
        cerr << "stdout open failed" << endl;
        return -1;
    }
    ostream cipherout(&pre);
    ct.save(cipherout);

    //--------------------ONLINE--------------------//

    auto begin = std::chrono::high_resolution_clock::now();

    GenPower(evaluator, relin_keys, powerbasis, 31);

    map<int, Ciphertext> big_powerbasis;

    int big_pre_power[] = {32, 160, 224, 256};
    for (auto i = 0; i < 4; i++) {
        vector<uint64_t> mat(slot_count, (i + 1));
        Plaintext pt;
        Ciphertext ct;
        batch_encoder.encode(mat, pt);
        encryptor.encrypt(pt, ct);
        Plaintext plain_result;
        vector<uint64_t> mat_res;
        big_powerbasis[big_pre_power[i]] = ct;
    }

    GenPower(evaluator, relin_keys, big_powerbasis, 31);
    big_powerbasis[0] = one;

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

    vector<Ciphertext> block(32);
    Ciphertext rotate;
    Ciphertext sum = zero;
    vector<vector<Plaintext>> coeffs(32);

    for (auto i = 0; i < coeffs.size(); i++) {
        vector<Plaintext> coeff(32);
        for (auto j = 0; j < coeff.size(); j++) {
            vector<uint64_t> mat(slot_count, (j + 1));
            Plaintext pt;
            batch_encoder.encode(mat, pt);
            coeff[j] = pt;
        }
        coeffs[i] = coeff;
    }

    for (auto part = 0; part < 50; part++) {
        for (auto i = 0; i < 32; i++) {
            block[i] = zero;
            evaluator.add_plain_inplace(block[i], coeffs[i][0]);
            threads[i] = std::thread(funcMulPlain, ref(evaluator), ref(relin_keys), ref(powerbasis), ref(coeffs[i]),
                                     ref(big_powerbasis[i * 32]), ref(block[i]));
        }
        for (auto i = 0; i < 32; i++) {
            threads[i].join();
        }
        for (auto i = 1; i < 32; i++) {
            evaluator.add_inplace(block[0], block[i]);
        }
        evaluator.relinearize_inplace(block[0], relin_keys);

        evaluator.rotate_rows(block[0], 1, galois_keys, rotate);
        evaluator.multiply_inplace(block[0], rotate);
        evaluator.relinearize_inplace(block[0], relin_keys);

        evaluator.rotate_rows(block[0], 2, galois_keys, rotate);
        evaluator.multiply_inplace(block[0], rotate);
        evaluator.relinearize_inplace(block[0], relin_keys);

        evaluator.rotate_rows(block[0], 4, galois_keys, rotate);
        evaluator.multiply_inplace(block[0], rotate);
        evaluator.relinearize_inplace(block[0], relin_keys);

        evaluator.rotate_rows(block[0], 8, galois_keys, rotate);
        evaluator.multiply_inplace(block[0], rotate);
        evaluator.relinearize_inplace(block[0], relin_keys);

        evaluator.add_inplace(sum, block[0]);

        // cout << part << "    + Noise budget in result: " <<
        // decryptor.invariant_noise_budget(sum) << " bits" << endl;

        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
    }
    for (auto i = 0; i < 4; i++) {
        evaluator.square_inplace(sum);
        evaluator.relinearize_inplace(sum, relin_keys);
        // cout << i << "    + Noise budget in result: " <<
        // decryptor.invariant_noise_budget(sum) << " bits" << endl;
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    }

    filebuf buf;
    if (buf.open("./cipher_out", ios::out) == nullptr) {
        cerr << "stdout open failed" << endl;
        return -1;
    }
    ostream out(&buf);
    sum.save(out);

    decryptor.decrypt(sum, plain_result);
    batch_encoder.decode(plain_result, mat_res);
    cout << "    + Result plaintext matrix ...... Correct." << endl;
    print_matrix(mat_res, row_size);
}
