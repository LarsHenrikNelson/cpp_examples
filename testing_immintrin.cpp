#include <iostream>
#include "immintrin.h"
#include <vector>

int main()
{
    unsigned long long dbs_in_avx = 8;
    std::vector<double> input1{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    std::vector<double> input2{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    unsigned long long i = 0;
    unsigned long long j = 0;
    unsigned long long input1_size = input1.size();
    unsigned long long input2_size = input2.size();
    std::vector<double> result(input1.size() + input2.size() - 1, 0);
    auto input2_size_avx = (input2.size() / dbs_in_avx) * dbs_in_avx;

    for (; i < input1_size; ++i)
    {
        __m512d a = _mm512_set1_pd(input1.at(i));
        j = 0;
        for (; j < input2_size_avx; j += dbs_in_avx)
        {
            __m512d b = _mm512_loadu_pd(input2.data() + j);
            __m512d intermediate = _mm512_mul_pd(a, b);
            __m512d c = _mm512_loadu_pd(result.data() + i + j);
            __m512d in2 = _mm512_add_pd(c, intermediate);
            _mm512_storeu_pd(result.data() + i + j, in2);
        }
        for (auto k = j; k < input2_size; ++k)
        {
            result.at(i + k) += (input1.at(i) * input2.at(k));
        }
    }

    for (auto &r : result)
    {
        std::cout << r << " ";
    }
    std::cout << std::endl;

    std::vector<double> result2(input1_size + input2_size - 1, 0.0);
    for (auto i = 0; i < input1.size(); i++)
    {
        for (auto j = 0; j < input2.size(); j++)
        {
            auto index = i + j;
            double temp = input1.at(i) * input2.at(j);
            result2.at(index) += temp;
        }
    }

    for (auto &m : result2)
    {
        std::cout << m << " ";
    }
    std::cout << std::endl;

    return 0;
}