#ifndef USER_CODE_H
#define USER_CODE_H

// Feel free to include more standard library header files
// based on your requirements
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <stack>

using namespace std;

// Feel free to introduce more helper functions

int question_one(const vector<int>& delta) {
    // TODO: Implement function
    int sum = delta[0];
    int sum_global = delta[0];
    
    for(int i = 1; i< delta.size() ;i++){
            sum_global=max(sum_global,max(delta[i],delta[i]+sum));
            // if(sum > sum_global)
                // sum_global=sum;
    }
    return sum_global;
}
void dfs(
    int i,
    int depth,
    const vector<int>& values,
    const vector<bool>& present,
    vector<int>& path,
    map<pair<int, vector<int>>, vector<int>>& groups,
    int& max_depth,
    const vector<int>& leftChild,
    const vector<int>& rightChild
) {
    if (i == -1 || !present[i])
        return;

    path.push_back(values[i]);

    int left = leftChild[i];
    int right = rightChild[i];

    if (left == -1 && right == -1) {

        vector<int> sorted_path = path;
        sort(sorted_path.begin(), sorted_path.end());

        auto key = make_pair(depth, sorted_path);

        auto it = groups.find(key);

        if (it == groups.end()) {
            // First path with this depth + multiset
            groups[key] = path;
        }
        else if (it->second != path) {
            // Same multiset, different ordering
            max_depth = max(max_depth, depth);
        }
    }

    dfs(left, depth + 1,
        values, present, path, groups,
        max_depth, leftChild, rightChild);

    dfs(right, depth + 1,
        values, present, path, groups,
        max_depth, leftChild, rightChild);

    path.pop_back();
}

int question_two(const vector<int>& values, const vector<bool>& present) {
    // TODO: Implement function
    // vector<vector<vector<int>>> buckets;
    //vector<int> path;
    // for(int x: values){
    //     cout << x << " ";
    // }
    // cout << endl;
    // for(int y: present){
    //     cout << y << " ";
    // }
    // cout << endl;

    int n = values.size();

    // Store the actual children of every node.
    vector<int> leftChild(n, -1);
    vector<int> rightChild(n, -1);

    // Build tree from compact level-order representation.
    if (n == 0 || !present[0])
        return -1;

    queue<int> q;
    q.push(0);

    int next = 1;

    while (!q.empty() && next < n) {
        int parent = q.front();
        q.pop();

        // Next token is left child
        if (next < n) {
            if (present[next]) {
                leftChild[parent] = next;
                q.push(next);
            }
            next++;
        }

        // Next token is right child
        if (next < n) {
            if (present[next]) {
                rightChild[parent] = next;
                q.push(next);
            }
            next++;
        }
    }

// Groups:
    // (depth, sorted path) -> set of original paths
    map<pair<int, vector<int>>, vector<int>> groups;

    vector<int> path;
    int max_depth = -1;
    dfs(0, 0, values, present, path, groups,
        max_depth, leftChild, rightChild);

   return max_depth;
//    return 0;
}

// vector<int> dijkstra(int n, vector<vector<pair<int, int>>>& graph, int source) {
//     const int INF = INT_MAX;

//     vector<int> dist(n, INF);

//     // {distance, node}
//     priority_queue<
//         pair<int, int>,
//         vector<pair<int, int>>,
//         greater<pair<int, int>>
//     > pq;

//     dist[source] = 0;
//     pq.push({0, source});

//     while (!pq.empty()) {
//         auto [currentDist, node] = pq.top();
//         pq.pop();

//         // Ignore outdated entries
//         if (currentDist != dist[node])
//             continue;

//         for (auto [neighbour, weight] : graph[node]) {
//             int newDist = currentDist + weight;

//             if (newDist < dist[neighbour]) {
//                 dist[neighbour] = newDist;
//                 pq.push({newDist, neighbour});
//             }
//         }
//     }

//     return dist;
// }


const long long INF = 1e18;


// ------------------------------------------------------------
// Dijkstra
// Returns shortest distance from source to every node.
// ------------------------------------------------------------
vector<long long> dijkstra(
    int source,
    int N,
    const vector<vector<pair<int, int>>>& graph
) {
    vector<long long> dist(N, INF);

    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        auto [d, u] = pq.top();
        pq.pop();

        // Outdated entry
        if (d != dist[u])
            continue;

        for (auto [v, w] : graph[u]) {

            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}


// ------------------------------------------------------------
// Bitmask DP after sorting.
//
// remaining = indices of packages that have not yet been
// delivered.
//
// start = current node (the Sorting Room)
//
// Finds the minimum travel time to deliver all remaining
// packages in any order.
// ------------------------------------------------------------
long long optimalAfterSorting(
    int start,
    const vector<int>& remaining,
    const vector<int>& destinations,
    const vector<vector<long long>>& dist
) {
    int m = remaining.size();

    if (m == 0)
        return 0;

    /*
        dp[mask][i]

        mask:
            Which packages from "remaining" have been delivered.

        i:
            The last package delivered.

        Value:
            Minimum travel time to reach that state.
    */

    vector<vector<long long>> dp(
        1 << m,
        vector<long long>(m, INF)
    );


    // --------------------------------------------------------
    // Choose the first package to deliver.
    // --------------------------------------------------------
    for (int i = 0; i < m; i++) {

        int packageIndex = remaining[i];

        int destination = destinations[packageIndex];

        dp[1 << i][i] =
            dist[start][destination];
    }


    // --------------------------------------------------------
    // Add packages one by one.
    // --------------------------------------------------------
    for (int mask = 0; mask < (1 << m); mask++) {

        for (int i = 0; i < m; i++) {

            // Package i is not in this mask
            if (!(mask & (1 << i)))
                continue;

            if (dp[mask][i] == INF)
                continue;

            int currentPackage = remaining[i];

            int currentNode =
                destinations[currentPackage];


            // Try every package that hasn't been delivered.
            for (int j = 0; j < m; j++) {

                if (mask & (1 << j))
                    continue;

                int nextPackage = remaining[j];

                int nextNode =
                    destinations[nextPackage];

                int newMask =
                    mask | (1 << j);

                dp[newMask][j] =
                    min(
                        dp[newMask][j],
                        dp[mask][i]
                        + dist[currentNode][nextNode]
                    );
            }
        }
    }


    // --------------------------------------------------------
    // All packages delivered.
    // Find the cheapest ending position.
    // --------------------------------------------------------
    int fullMask = (1 << m) - 1;

    long long answer = INF;

    for (int i = 0; i < m; i++) {
        answer = min(answer, dp[fullMask][i]);
    }

    return answer;
}


// ------------------------------------------------------------
// Question 3
// ------------------------------------------------------------
int question_three(
    int N,
    const vector<vector<int>>& edges,
    int S,
    int K,
    const vector<int>& destinations
) {
    int C = destinations.size();


    // ========================================================
    // 1. Build graph
    // ========================================================

    vector<vector<pair<int, int>>> graph(N);

    for (const auto& edge : edges) {

        int u = edge[0];
        int v = edge[1];
        int w = edge[2];

        // Undirected graph
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }


    // ========================================================
    // 2. Find the important nodes
    //
    // We only care about:
    //
    //     0       = Arrival Room
    //     K       = Sorting Room
    //     package destinations
    //
    // ========================================================

    vector<int> important;

    important.push_back(0);
    important.push_back(K);

    for (int x : destinations)
        important.push_back(x);


    // Remove duplicates
    sort(important.begin(), important.end());

    important.erase(
        unique(important.begin(), important.end()),
        important.end()
    );


    // ========================================================
    // 3. Calculate shortest distances
    //
    // dist[u][v] = shortest distance between u and v
    //
    // We run Dijkstra only from important nodes.
    // ========================================================

    vector<vector<long long>> dist(
        N,
        vector<long long>(N, INF)
    );

    for (int source : important) {

        vector<long long> d =
            dijkstra(source, N, graph);

        for (int target : important) {
            dist[source][target] = d[target];
        }
    }


    // ========================================================
    // 4. Loading
    //
    // Every package takes 1 unit.
    // ========================================================

    long long loadingTime = C;


    // ========================================================
    // 5. Try every possible point where sorting happens.
    //
    // Initial queue:
    //
    // destinations:
    //
    //     P0 P1 P2 P3
    //
    // After loading into stack:
    //
    //     P3  <- TOP
    //     P2
    //     P1
    //     P0
    //
    // Therefore, before sorting, the delivery order is forced:
    //
    //     P3 -> P2 -> P1 -> P0
    //
    // We try:
    //
    // 0 packages before sorting
    // 1 package before sorting
    // 2 packages before sorting
    // ...
    // C packages before sorting
    //
    // ========================================================

    long long answer = INF;


    // Travel cost accumulated while following
    // the original LIFO stack.
    long long directTravel = 0;

    // Current position of robot.
    int currentNode = 0;


    for (int delivered = 0;
         delivered <= C;
         delivered++) {


        // ====================================================
        // CASE 1:
        // All packages have already been delivered.
        //
        // No sorting required.
        // ====================================================

        if (delivered == C) {

            long long total =
                loadingTime
                + directTravel;

            answer = min(answer, total);

            break;
        }


        // ====================================================
        // CASE 2:
        // Some packages remain.
        //
        // Go to sorting room and optimally arrange the
        // remaining packages.
        // ====================================================


        // ----------------------------------------------------
        // Packages that remain are:
        //
        // destinations[0]
        // destinations[1]
        // ...
        // destinations[C-delivered-1]
        //
        // ----------------------------------------------------

        vector<int> remaining;

        for (int i = 0;
             i < C - delivered;
             i++) {

            remaining.push_back(i);
        }


        // ----------------------------------------------------
        // Travel from current position to Sorting Room.
        // ----------------------------------------------------

        long long toSorting =
            dist[currentNode][K];


        // ----------------------------------------------------
        // After sorting, packages can be delivered in ANY
        // order.
        // ----------------------------------------------------

        long long afterSorting =
            optimalAfterSorting(
                K,
                remaining,
                destinations,
                dist
            );


        // ----------------------------------------------------
        // Total:
        //
        // loading
        // + direct deliveries
        // + travel to sorting room
        // + sorting penalty
        // + optimal remaining deliveries
        // ----------------------------------------------------

        long long total =
            loadingTime
            + directTravel
            + toSorting
            + S
            + afterSorting;


        answer = min(answer, total);


        // ====================================================
        // Prepare next possibility.
        //
        // Deliver one more package directly from the original
        // stack.
        // ====================================================

        int packageIndex =
            C - 1 - delivered;

        int nextNode =
            destinations[packageIndex];


        directTravel +=
            dist[currentNode][nextNode];


        currentNode = nextNode;
    }


    return (int)answer;
}

#endif // USER_CODE_H
