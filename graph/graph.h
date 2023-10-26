#include <bits/stdc++.h>
using namespace std;
struct graph {
  vector<vector<int>> e;
  graph(int n) : e(n + 1) {}
  void adde(int u, int v) { e[u].push_back(v); }
  optional<vector<int>> toposort() {
    int n = e.size() - 1;
    vector<int> d(n + 1);
    queue<int> que;
    for (int u = 1; u <= n; u++)
      for (auto v : e[u]) d[v]++;
    for (int i = 1; i <= n; i++)
      if (!d[i]) que.push(i);
    vector<int> res;
    while (!que.empty()) {
      auto u = que.front();
      res.push_back(u);
      que.pop();
      for (auto v : e[u]) {
        if (!--d[v]) que.push(v);
      }
    }
    if (res.size() != n) return nullopt;
    return res;
  }
};