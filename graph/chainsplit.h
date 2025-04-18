// ver 2?
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct tree {
  vector<vector<int>> e;
  vector<int> fa, size, dep, anc, ord;
  node *rt;
  tree(int n)
      : e(n + 1), fa(n + 1), size(n + 1), dep(n + 1), anc(n + 1), ord(n + 1) {}
  void adde(int u, int v) { e[u].push_back(v), e[v].push_back(u); }
  void init(vector<ll> x) {
    vector<ll> y = x;
    for (int u = 1; u < x.size(); u++)
      for (auto v : e[u]) y[u] = min(y[u], x[v]);
    auto prep = [&](auto &&self, int u, int f) -> void {
      fa[u] = f, size[u] = 1, dep[u] = dep[f] + 1;
      for (int i = 0; i < e[u].size(); i++)
        if (e[u][i] == f) e[u][i] = e[u].back(), e[u].pop_back();
      for (auto v : e[u]) self(self, v, u), size[u] += size[v];
      for (auto &v : e[u])
        if (size[v] > size[e[u][0]]) swap(v, e[u][0]);
    };
    prep(prep, 1, 1);
    vector<int> seq;
    auto dfs = [&](auto &&self, int u, bool hvy) -> void {
      anc[u] = hvy ? anc[fa[u]] : u;
      ord[u] = seq.size(), seq.push_back(u);
      for (auto v : e[u]) self(self, v, v == e[u][0]);
    };
    dfs(dfs, 1, false);
    rt = build(0, seq.size() - 1, seq, x, y);
  }
  ll qlink(int u, int v) {
    dist mu, mv;
    while (anc[u] != anc[v]) {
      if (dep[anc[u]] < dep[anc[v]]) swap(u, v), swap(mu, mv);
      mu = qrange(rt, ord[anc[u]], ord[u]) + mu;
      u = fa[anc[u]];
    }
    if (dep[u] < dep[v]) swap(u, v), swap(mu, mv);
    auto m = qrange(rt, ord[v], ord[u]);
    return (-mv + qrange(rt, ord[v], ord[u]) + mu).a[0][0];
  }
};