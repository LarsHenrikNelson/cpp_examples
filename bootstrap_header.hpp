#ifndef BOOTSTRAP_TEST_HPP
#define BOOTSTRAP_TEST_HPP

#include <vector>
#include <numeric>
#include <random>
#include <pcg_random.hpp>

std::vector<double> norm_dist(double mu, double std, int length, int seed = 42)
{
    std::normal_distribution<double> norm_dist(mu, std);

    pcg64 rng(seed);

    std::vector<double> dist(length);
    for (auto &i : dist)
    {
        i = norm_dist(rng);
    };
    return dist;
};

std::vector<double> random_choice(const std::vector<double> &vals, int length, pcg64 &rng)
{
    std::vector<double> choices(length);
    std::uniform_int_distribution<> distr(0, vals.size() - 1);

    for (auto &i : choices)
    {
        i = vals.at(distr(rng));
    }
    return choices;
}

std::vector<double> bootstrap_diffs(const std::vector<double> &dist_1, const std::vector<double> &dist_2, int repeats, pcg64 &rng)
{
    std::vector<double> means(repeats);
    for (auto &i : means)
    {
        auto choices_1 = random_choice(dist_1, dist_1.size() - 1, rng);
        auto choices_2 = random_choice(dist_2, dist_2.size() - 2, rng);
        double mean_1 = std::reduce(choices_1.cbegin(), choices_1.cend()) / choices_1.size();
        double mean_2 = std::reduce(choices_2.cbegin(), choices_2.cend()) / choices_2.size();
        i = mean_2 - mean_1;
    }
    return means;
}

double percentile(const std::vector<double> &vals, double val)
{
    double left_count{0.0};
    double right_count{0.0};

    for (auto &i : vals)
    {
        if (i <= val)
            right_count += 1.0;
        if (i < val)
            left_count += 1.0;
    }

    double total = left_count + right_count;

    if (left_count < right_count)
        total += 1.0;

    total = total * 50.0 / vals.size();

    return total;
}

double bootstrap_test(const std::vector<double> &dist_1, const std::vector<double> &dist_2, int repeats, int seed)
{
    pcg64 rng(seed);
    auto bootstrapped_means = bootstrap_diffs(dist_1, dist_2, repeats, rng);
    double mean_1 = std::reduce(dist_1.cbegin(), dist_1.cend()) / dist_1.size();
    double mean_2 = std::reduce(dist_2.cbegin(), dist_2.cend()) / dist_2.size();
    double observed_mean = mean_2 - mean_1;

    for (auto &i : bootstrapped_means)
    {
        i -= observed_mean;
    }

    auto percen = percentile(bootstrapped_means, observed_mean);
    double auc_right = percen / 100.0;
    double auc_left = 1.0 - percen / 100.0;
    double auc_tail = (auc_right < auc_left) ? auc_right : auc_left;
    double p_value = auc_tail * 2.0;
    return p_value;
}

#endif
