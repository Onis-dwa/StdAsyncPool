
#include <iostream>
#include <future>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;
typedef int uint;

static constexpr uint S_begin = 1'000'000;
static constexpr uint S_end   = 2'000'000;
static constexpr uint S_step  = 62'500;
static mutex m;

unsigned long long someF(uint l, uint r) {
	m.lock();
	cout << "thread for range " << l << " " << r << endl;
	m.unlock();

	unsigned long long iters = 0;
	vector<pair<uint, uint>> result = {};
	vector<pair<uint, uint>> resultDiffs = {};
	for (uint i = l; i <= r; ++i) {
		vector<pair<uint, uint>> mults = {};
		uint sqr = sqrt(i);
		uint cnt = 0;
		for (uint a = sqr; a < sqr + 101; ++a) {
			for (uint b = sqr;; --b) {
				iters++;
				uint mod = abs(a - b);
				if (mod >= 100) break;
				if (a * b == i) {
					cnt += mod;
					mults.push_back({ a, b });
				}
			}
		}

		if (mults.size() >= 3) {
			result.push_back({ i, cnt });
			for (const auto& it : mults)
				resultDiffs.push_back(it);
		}
	}

	m.lock();
	for (const auto& it : result) {
		cout << "i: " << it.first << " cnt: " << it.second << " wich [ ";
		for (const auto& it : resultDiffs) {
			cout << "{" << it.first << " " << it.second << "}, ";
		}
		cout << "\b\b ]" << endl;
	}
	m.unlock();
	return iters;
}

int main() {
	steady_clock::time_point begin = steady_clock::now();

	vector<future<unsigned long long>> threads;
	for (uint i = S_begin; i < S_end; i += S_step) {
		threads.emplace_back(async(launch::async, someF, i, i + S_step));
	}

	unsigned long long summary = 0;
	for (auto& it : threads) {
		summary = it.get();
	}
	cout << "summary iterations " << summary << endl;

	m.lock();
	steady_clock::time_point end = steady_clock::now();
	cout << "Total time: " << duration_cast<nanoseconds> (end - begin).count() << "[ns]" << endl;
	m.unlock();

	return 0;
}
