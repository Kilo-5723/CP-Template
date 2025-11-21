#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;

optional<vector<int>> euler(vector<pair<int, int>> edges, int s = -1) {
  int n = 0;
  for (auto [u, v] : edges) n = max({n, u, v});
  vector<int> e;
  vector<vector<int>> g(n + 1);
  for (auto [u, v] : edges) {
    g[u].push_back(e.size()), e.push_back(v);
    g[v].push_back(e.size()), e.push_back(u);
  }
  if (s == -1) {
    for (int u = 0; u <= n; u++)
      if (g[u].size() & 1) s = u;
    if (s == -1)
      for (int u = 0; u <= n; u++)
        if (g[u].size()) s = u;
    if (s == -1) return {};
  }
  int cnt = 0;
  for (int u = 0; u <= n; u++)
    if (g[u].size() & 1) cnt++;
  if (cnt > 2 || cnt && ~g[s].size() & 1) return nullopt;
  vector<bool> vis(edges.size());
  vector<int> res;
  auto dfs = [&](auto &&self, int u) -> void {
    while (g[u].size()) {
      auto i = g[u].back();
      g[u].pop_back();
      if (vis[i / 2]) continue;
      vis[i / 2] = true;
      self(self, e[i]);
      res.push_back(i);
    }
  };
  dfs(dfs, s);
  if (res.size() != edges.size()) return nullopt;
  return res;
}

// euler path fron any s https://codeforces.com/contest/2110/problem/E