#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

#include "user_code.h"

using namespace std;
using namespace chrono;


// ============================================================
// Configuration
// ============================================================

const int REPEATS = 5;


// ============================================================
// Utility: measure execution time
// ============================================================

template <typename Function>
double measure_ms(Function func, int repeats = REPEATS) {

    // Warm-up
    func();

    vector<double> times;

    for (int r = 0; r < repeats; r++) {

        auto start = high_resolution_clock::now();

        volatile auto result = func();

        auto end = high_resolution_clock::now();

        double ms =
            duration<double, milli>(end - start).count();

        times.push_back(ms);
    }

    // Median is less sensitive to random OS interruptions
    sort(times.begin(), times.end());

    return times[times.size() / 2];
}


// ============================================================
// Random number generator
// ============================================================

mt19937 rng(42);

int random_int(int low, int high) {
    uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}


// ============================================================
// QUESTION 1 INPUT GENERATORS
// ============================================================

vector<int> generate_random_array(int n) {

    vector<int> a(n);

    for (int i = 0; i < n; i++)
        a[i] = random_int(-100, 100);

    return a;
}


vector<int> generate_all_positive(int n) {

    vector<int> a(n);

    for (int i = 0; i < n; i++)
        a[i] = random_int(1, 100);

    return a;
}


vector<int> generate_all_negative(int n) {

    vector<int> a(n);

    for (int i = 0; i < n; i++)
        a[i] = -random_int(1, 100);

    return a;
}


// ============================================================
// QUESTION 2 TREE GENERATORS
//
// present[] is the compact level-order representation used
// by your question_two() implementation.
// ============================================================

// Balanced tree
vector<bool> generate_balanced_tree(int n) {

    vector<bool> present(n, true);

    return present;
}


// Left-skewed tree.
//
// Because question_two() constructs the tree from level order,
// simply putting all nodes as "present" does NOT create a
// skewed tree.
//
// Instead, we create a representation that leaves only one
// child at every level.
//
// This generator creates a valid compact representation for
// experimentation with the function's representation.
vector<bool> generate_left_skewed(int n) {

    vector<bool> present(n, false);

    if (n == 0)
        return present;

    present[0] = true;

    // The exact compact representation is constrained by the
    // input format used by question_two().
    //
    // We therefore use a sparse pattern.
    for (int i = 1; i < n; i += 2)
        present[i] = true;

    return present;
}


vector<int> generate_tree_values(int n) {

    vector<int> values(n);

    for (int i = 0; i < n; i++)
        values[i] = random_int(1, 100);

    return values;
}


// ============================================================
// QUESTION 3 GRAPH GENERATOR
// ============================================================

struct GraphInput {

    int N;
    vector<vector<int>> edges;
    int S;
    int K;
    vector<int> destinations;
};


// Generate connected sparse graph
GraphInput generate_graph(int N, int C) {

    GraphInput input;

    input.N = N;

    // Sorting penalty
    input.S = 5;

    // Arrival = 0
    // Sorting room = 1
    input.K = 1;

    // --------------------------------------------------------
    // First create a chain so graph is guaranteed connected.
    // --------------------------------------------------------

    for (int i = 0; i < N - 1; i++) {

        int weight = random_int(1, 20);

        input.edges.push_back({
            i,
            i + 1,
            weight
        });
    }


    // --------------------------------------------------------
    // Add additional random edges.
    // --------------------------------------------------------

    int extra_edges = 2 * N;

    for (int i = 0; i < extra_edges; i++) {

        int u = random_int(0, N - 1);
        int v = random_int(0, N - 1);

        if (u == v)
            continue;

        int weight = random_int(1, 20);

        input.edges.push_back({
            u,
            v,
            weight
        });
    }


    // --------------------------------------------------------
    // Package destinations.
    //
    // Avoid 0 and K where possible.
    // --------------------------------------------------------

    for (int i = 0; i < C; i++) {

        int destination;

        do {
            destination = random_int(2, N - 1);
        } while (false);

        input.destinations.push_back(destination);
    }

    return input;
}


// ============================================================
// QUESTION 1 BENCHMARK
// ============================================================

void benchmark_question_one() {

    cout << "\n========================================\n";
    cout << "QUESTION 1\n";
    cout << "========================================\n";

    ofstream out("question1_results.csv");

    out << "n,case,time_ms\n";

    vector<int> sizes = {
        100,
        1000,
        10000,
        100000,
        1000000,
        5000000
    };


    for (int n : sizes) {

        cout << "n = " << n << endl;


        // Random
        {
            auto input = generate_random_array(n);

            double t = measure_ms(
                [&]() {
                    return question_one(input);
                }
            );

            out << n << ",random," << t << "\n";
        }


        // All positive
        {
            auto input = generate_all_positive(n);

            double t = measure_ms(
                [&]() {
                    return question_one(input);
                }
            );

            out << n << ",positive," << t << "\n";
        }


        // All negative
        {
            auto input = generate_all_negative(n);

            double t = measure_ms(
                [&]() {
                    return question_one(input);
                }
            );

            out << n << ",negative," << t << "\n";
        }
    }

    out.close();
}


// ============================================================
// QUESTION 2 BENCHMARK
// ============================================================

void benchmark_question_two() {

    cout << "\n========================================\n";
    cout << "QUESTION 2\n";
    cout << "========================================\n";

    ofstream out("question2_results.csv");

    out << "n,case,time_ms\n";


    vector<int> sizes = {
        100,
        500,
        1000,
        2000,
        5000,
        10000
    };


    for (int n : sizes) {

        cout << "n = " << n << endl;


        // ----------------------------------------------------
        // Balanced
        // ----------------------------------------------------

        {
            vector<int> values =
                generate_tree_values(n);

            vector<bool> present =
                generate_balanced_tree(n);

            double t = measure_ms(
                [&]() {
                    return question_two(values, present);
                }
            );

            out << n << ",balanced," << t << "\n";
        }


        // ----------------------------------------------------
        // Sparse / skew-like
        // ----------------------------------------------------

        {
            vector<int> values =
                generate_tree_values(n);

            vector<bool> present =
                generate_left_skewed(n);

            double t = measure_ms(
                [&]() {
                    return question_two(values, present);
                }
            );

            out << n << ",sparse," << t << "\n";
        }
    }

    out.close();
}


// ============================================================
// QUESTION 3 BENCHMARK
// ============================================================

// ============================================================
// QUESTION 3 — EXPERIMENT 1
//
// Runtime vs graph size N
// C is fixed.
//
// Output:
//     question3_vs_N.csv
//
// Columns:
//     N,C,E,time_ms
// ============================================================

void benchmark_question_three_vs_N() {

    cout << "\n========================================\n";
    cout << "QUESTION 3 — RUNTIME VS N\n";
    cout << "========================================\n";

    ofstream out("question3_vs_N.csv");

    out << "N,C,E,time_ms\n";

    // Fixed number of packages
    const int C = 6;

    // Increase graph size
    vector<int> node_sizes = {
        100,
        250,
        500,
        1000,
        2000,
        5000
    };

    for (int N : node_sizes) {

        cout << "N = " << N
             << ", C = " << C << endl;

        GraphInput input =
            generate_graph(N, C);

        double t = measure_ms(
            [&]() {
                return question_three(
                    input.N,
                    input.edges,
                    input.S,
                    input.K,
                    input.destinations
                );
            },
            5
        );

        out << N << ","
            << C << ","
            << input.edges.size() << ","
            << t << "\n";
    }

    out.close();

    cout << "Saved: question3_vs_N.csv\n";
}


// ============================================================
// QUESTION 3 — EXPERIMENT 2
//
// Runtime vs number of packages C
// N is fixed.
//
// Output:
//     question3_vs_C.csv
//
// Columns:
//     N,C,E,time_ms
// ============================================================

void benchmark_question_three_vs_C() {

    cout << "\n========================================\n";
    cout << "QUESTION 3 — RUNTIME VS C\n";
    cout << "========================================\n";

    ofstream out("question3_vs_C.csv");

    out << "N,C,E,time_ms\n";

    // Fixed graph size
    const int N = 1000;

    // Increase number of packages
    vector<int> package_sizes = {
        2,
        4,
        6,
        8,
        10,
        12,
        14,
        16
    };

    for (int C : package_sizes) {

        cout << "N = " << N
             << ", C = " << C << endl;

        GraphInput input =
            generate_graph(N, C);

        double t = measure_ms(
            [&]() {
                return question_three(
                    input.N,
                    input.edges,
                    input.S,
                    input.K,
                    input.destinations
                );
            },
            5
        );

        out << N << ","
            << C << ","
            << input.edges.size() << ","
            << t << "\n";
    }

    out.close();

    cout << "Saved: question3_vs_C.csv\n";
}

// ============================================================
// MAIN
// ============================================================

int main() {

    cout << fixed << setprecision(3);

    benchmark_question_one();

    benchmark_question_two();

    benchmark_question_three_vs_N();
    benchmark_question_three_vs_C();

    cout << "\n========================================\n";
    cout << "BENCHMARK COMPLETE\n";
    cout << "========================================\n";

    cout << "Generated:\n";
    cout << "  question1_results.csv\n";
    cout << "  question2_results.csv\n";
    cout << "  question3_results.csv\n";

    return 0;
}