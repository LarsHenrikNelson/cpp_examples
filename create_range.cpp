#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <execution>

std::vector<double> genRange(double start, int length, double step)
{
    std::vector<double> range(length);

    std::for_each(range.begin(), range.end(), [start, step, i = 0](double &actual) mutable
                  {
        actual = start + step * i;
        i++; });
    return range;
}

std::vector<double> createRange(double start, int length, double step)
{
    std::vector<double> range(length);
    for (double i{0}; i < length; ++i)
    {
        range[i] = start + step * i;
    }
    return range;
}

int main()
{
    // auto v = genRange(0, 15, 0.1);
    double start{0};
    double step{0.1};
    long long length{};

    std::cin >> length;

    // Fastest way to create a range of numbers.
    std::vector<double> v(length);
    const auto t_start = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par, v.begin(), v.end(), [start, step, i = 0](double &actual) mutable
                  {
        actual = start + step * i;
        ++i; });
    const auto t_end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> mss = t_end - t_start;
    std::cout << mss.count() << std::endl;

    // For loop is slower than for each.
    const auto t_starts = std::chrono::high_resolution_clock::now();
    std::vector<double> range(length);

    for (long long i{}; i < length; ++i)
    {
        range[i] = start + step * i;
    }
    auto t_ends = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> mss1 = t_ends - t_starts;
    std::cout << mss1.count() << std::endl;

    return 0;
}