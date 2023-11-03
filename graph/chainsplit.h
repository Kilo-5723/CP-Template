#include <bits/stdc++.h>
using namespace std;
struct graph {
  vector<vector<int>> e;
  vector<int> fa, dep, siz, hvs, top, dfn, rnk;
  vector<bit> seg;
  graph(int n) : seg(maxd, n) {
    e.resize(n + 1);
    fa.resize(n + 1);
    dep.resize(n + 1);
    siz.resize(n + 1);
    hvs.resize(n + 1);
    top.resize(n + 1);
    dfn.resize(n + 1);
    rnk.resize(n + 1);
  }
  void adde(int u, int v) {
    e[u].push_back(v);
    e[v].push_back(u);
  }
  void subts(int u, int f, int d) {
    fa[u] = f, dep[u] = d;
    siz[u] = 1;
    hvs[u] = -1;
    for (auto v : e[u])
      if (v != f) {
        subts(v, u, d + 1);
        siz[u] += siz[v];
        if (!~hvs[u] || siz[v] > siz[hvs[u]]) hvs[u] = v;
      }
  }
  int tot;
  void ordfs(int u, int t) {
    top[u] = t;
    tot++, rnk[tot] = u, dfn[u] = tot;
    if (~hvs[u]) ordfs(hvs[u], t);
    for (auto v : e[u])
      if (v != fa[u] && v != hvs[u]) ordfs(v, v);
  }
  void hvydecomp(int rt) {
    subts(rt, -1, 0);
    tot = 0;
    ordfs(rt, rt);
  }
  void modify(int u, int v, int k, int d) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      seg[d].add(dfn[top[u]], dfn[u], k);
      u = fa[top[u]];
    }
    if (dep[u] < dep[v]) swap(u, v);
    seg[d].add(dfn[v], dfn[u], k);
    u = v;
    while (d--) {
      seg[d].add(dfn[u], dfn[u], k);
      u = fa[u];
      seg[d].add(dfn[u], dfn[u], k);
    }
  }
  int query(int u) {
    int ans = 0;
    for (int i = 0; i < maxd; i++) {
      ans += seg[i].query(dfn[u]);
      u = fa[u];
    }
    return ans;
  }
};