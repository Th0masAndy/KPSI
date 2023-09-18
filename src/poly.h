#include <chrono>
#include <emmintrin.h>
#include <iostream>
#include <vector>

void add_vectors(const int* a, const int* b, int* c, int n) {
    long long i;
    for (i = 0; i <= n - 4; i += 4) {
        __m128i va = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&a[i]));
        __m128i vb = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&b[i]));
        __m128i vc = _mm_add_epi32(va, vb);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(&c[i]), vc);
    }
}

class LagrangeInterpolation {
  private:
    const long long MOD;

    int mod_add(long long a, long long b) const { return (a + b) % MOD; }

    int mod_sub(long long a, long long b) const { return (a - b + MOD) % MOD; }

    int mod_mul(long long a, long long b) const {
        std::uint64_t tmp = a * b;
        long long res = tmp % MOD;
        return res;
    }

    int mod_pow(long long base, long long exp) const {
        long long result = 1;
        while (exp) {
            if (exp & 1) result = mod_mul(result, base);
            base = mod_mul(base, base);
            exp >>= 1;
        }
        return result;
    }

    long long mod_inv(long long a) const { return mod_pow(a, MOD - 2); }

  public:
    std::vector<long long> x_values;
    std::vector<std::vector<int>> L;

    LagrangeInterpolation(long long mod = 65537) : MOD(mod) {
        x_values.resize(1024);
        for (auto i = 0; i < 1024; i++) {
            x_values[i] = i;
        }
        L.resize(x_values.size());
        long long n = x_values.size();
        std::vector<long long> G(n + 1, 0);
        G[1] = 1;
        G[0] = MOD - x_values[0];
        for (auto i = 1; i < x_values.size(); i++) {
            G[0] = mod_mul(G[0], MOD - x_values[i]);
            for (auto k = i + 1; k > 0; k--) {
                G[k] = (G[k - 1] + mod_mul(G[k], (MOD - x_values[i]))) % MOD;
            }
        }
        for (auto i = 0; i < x_values.size(); i++) {
            std::vector<int> Li(n);
            long long s = 1;
            for (auto j = 0; j < x_values.size(); j++) {
                if (i == j) {
                    continue;
                }
                s = mod_mul(s, (x_values[i] + MOD - x_values[j]) % MOD);
            }
            s = mod_inv(s);
            if (x_values[i] == 0) {
                for (auto j = 0; j < Li.size(); j++) {
                    std::uint64_t tmp = G[j + 1];
                    tmp = tmp * s % MOD;
                    Li[j] = tmp & 0xffffffff;
                }
                L[i] = Li;
            } else {
                Li[0] = mod_mul(G[0], mod_inv(MOD - x_values[i]));
                for (auto k = 1; k < Li.size(); k++) {
                    std::uint64_t tmp = (G[k] + MOD - Li[k - 1]) * mod_inv(MOD - x_values[i] % MOD);
                    Li[k] = tmp % MOD;
                }
                for (auto j = 0; j < Li.size(); j++) {
                    std::uint64_t tmp = Li[j];
                    tmp = tmp * s % MOD;
                    Li[j] = tmp;
                }
                L[i] = Li;
            }
        }
    }

    std::vector<int> Interpolation(std::vector<int>& y) {
        std::vector<int> res(y.size(), 0);
        for (auto k = 0; k < L.size(); k++) {
            auto Lk = L[k];
            if (y[k] == 0) {
                continue;
            }
            add_vectors(res.data(), Lk.data(), res.data(), res.size()); // optimized
            // for (auto i = 0; i < Lk.size(); i++) {
            //     res[i] = (res[i] + Lk[i]) % MOD;
            // }
        }
        for (auto k = 0; k < L.size(); k++) {
            res[k] = res[k] % MOD;
        }
        return res;
    }

    unsigned long long Evaluation(std::vector<int> coeff, unsigned long long x) {
        long long res = coeff[0];
        long long base = x;
        for (auto i = 1; i < coeff.size(); i++) {
            res = res + coeff[i] * base;
            res = res % MOD;
            base = (base * x) % MOD;
        }
        return res;
    }
};

// int main() {
//     size_t n = 1024;
//     std::vector<long long> x(n);
//     std::vector<int> y(n);

//     for (auto i = 0; i < x.size(); i++) {
//         x[i] = i;
//         if (i % 6 == 0) {
//             y[i] = 1;
//         } else {
//             y[i] = 0;
//         }
//     }

//     LagrangeInterpolation interpolator;

//     auto begin = std::chrono::high_resolution_clock::now();

//     for (auto k = 0; k < 1024 * 80; k++) {
//         auto res = interpolator.Interpolation(y);
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//     printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

//     return 0;
// }
