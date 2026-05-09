
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "flat_bst.hpp"
#include "veb_bst.hpp"

namespace {
struct BenchResult {
	std::size_t n;
	double flat_ns_per_query;
	double veb_ns_per_query;
};

double data_size_mb(std::size_t n) {
	const double bytes = static_cast<double>(n) * static_cast<double>(sizeof(int));
	return bytes / (1024.0 * 1024.0);
}

std::vector<int> make_keys(std::size_t n) {
	std::vector<int> keys(n);
	for (std::size_t i = 0; i < n; ++i) {
		keys[i] = static_cast<int>(i * 2 + 1);
	}
	return keys;
}

std::vector<int> make_queries(const std::vector<int>& keys,
							  std::size_t count,
							  std::mt19937& rng) {
	std::vector<int> queries(count);
	std::uniform_int_distribution<std::size_t> hit_dist(0, keys.size() - 1);
	std::uniform_int_distribution<int> miss_dist(0, static_cast<int>(keys.size() * 4));

	for (std::size_t i = 0; i < count; ++i) {
		if ((i & 1U) == 0) {
			queries[i] = keys[hit_dist(rng)];
		} else {
			int candidate = miss_dist(rng) * 2;
			queries[i] = candidate;
		}
	}
	return queries;
}

template <typename Tree>
double run_queries(Tree& tree, const std::vector<int>& queries) {
	volatile std::uint64_t hits = 0;
	auto start = std::chrono::steady_clock::now();
	for (int q : queries) {
		hits += static_cast<std::uint64_t>(tree.search(q));
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::nano> elapsed = end - start;
	(void)hits;
	return elapsed.count() / static_cast<double>(queries.size());
}

BenchResult run_benchmark(std::size_t n,
						  std::size_t query_count,
						  std::mt19937& rng) {
	std::vector<int> keys = make_keys(n);
	std::vector<int> queries = make_queries(keys, query_count, rng);

	flat_bst flat(keys);
	veb_best veb(keys);

	run_queries(flat, queries);
	run_queries(veb, queries);

	double flat_ns = run_queries(flat, queries);
	double veb_ns = run_queries(veb, queries);
	return BenchResult{n, flat_ns, veb_ns};
}

BenchResult run_benchmark_avg(std::size_t n,
							  std::size_t query_count,
							  int trials,
							  std::mt19937& rng) {
	double flat_total = 0.0;
	double veb_total = 0.0;
	for (int t = 0; t < trials; ++t) {
		BenchResult result = run_benchmark(n, query_count, rng);
		flat_total += result.flat_ns_per_query;
		veb_total += result.veb_ns_per_query;
	}
	return BenchResult{n, flat_total / trials, veb_total / trials};
}
} // namespace

int main() {
	std::mt19937 rng(12345);

	std::vector<std::size_t> sizes = {
        1'000'000,
        2'000'000,
        4'000'000,
        8'000'000,
        16'000'000,
        32'000'000,
        64'000'000,
        255'000'000LL,
        512'000'000LL,
        1'024'000'000LL,

	};
	std::size_t query_count = 500'000;
	int trials = 3;

	std::ofstream csv("benchmark_results.csv");
	csv << "n,data_mb,flat_ns_per_query,veb_ns_per_query,speedup\n";

	std::cout << "Cache-aware BST query benchmark\n";
	std::cout << "Queries: " << query_count << " (50% hits, 50% misses)\n";
	std::cout << "Trials: " << trials << "\n\n";
	std::cout << std::left << std::setw(12) << "N"
			  << std::setw(12) << "Data MB"
			  << std::setw(18) << "Flat ns/query"
			  << std::setw(18) << "vEB ns/query"
			  << std::setw(12) << "Speedup" << "\n";

	for (std::size_t n : sizes) {
		BenchResult result = run_benchmark_avg(n, query_count, trials, rng);
		double speedup = result.flat_ns_per_query / result.veb_ns_per_query;

		double mb = data_size_mb(result.n);

		std::cout << std::left << std::setw(12) << result.n
				  << std::setw(12) << std::fixed << std::setprecision(2) << mb
				  << std::setw(18) << result.flat_ns_per_query
				  << std::setw(18) << result.veb_ns_per_query
				  << std::setw(12) << speedup << "\n";

		csv << result.n << "," << std::fixed << std::setprecision(2)
			<< mb << "," << result.flat_ns_per_query
			<< "," << result.veb_ns_per_query
			<< "," << speedup << "\n";
	}

	return 0;
}
