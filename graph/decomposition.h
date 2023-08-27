#include <bits/stdc++.h>
using namespace std;
struct graph {
  vector<vector<int>> e, r;
  graph(int n) {
    e.resize(n + 1);
    r.resize(n + 1);
  }
  void adde(int u, int v) {
    e[u].push_back(v);
    r[v].push_back(u);
  }
  vector<bool> vis;
  vector<int> ord;
  vector<int> col;
  void travel(int u, int f) {
    vis[u] = true;
    for (auto v : e[u])
      if (!vis[v]) travel(v, u);
    ord.push_back(u);
  }
  void color(int u, int f, int c) {
    col[u] = c;
    for (auto v : r[u])
      if (!col[v]) color(v, u, c);
  }
  vector<int> decomp() {
    vis.assign(e.size(), false);
    ord.clear();
    for (int i = 1; i < e.size(); i++)
      if (!vis[i]) travel(i, i);
    reverse(ord.begin(), ord.end());
    col.assign(e.size(), 0);
    for (auto v : ord)
      if (!col[v]) color(v, v, v);
    return col;
  }
};