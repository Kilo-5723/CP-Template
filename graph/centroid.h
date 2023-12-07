#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;
const ll inf = 1e18;
struct graph {
  vector<vector<pair<int, ll>>> e;
  vector<ll> r;
  graph(int n) : e(n + 1), r(n + 1) {}
  void adde(int u, int v, ll w) {
    e[u].push_back({v, w});
    e[v].push_back({u, w});
  }
  vector<int> siz;
  int calcsiz(int u, int f) {
    siz[u] = 1;
    for (auto [v, w] : e[u])
      if (v != f && ~siz[v]) siz[u] += calcsiz(v, u);
    return siz[u];
  }
  vector<vector<pair<int, ll>>> fa;
  vector<vector<pair<ll, int>>> ch;
  void init(int u, int f, int rt, ll d) {
    fa[u].push_back({rt, d});
    ch[rt].push_back({r[u] - d, u});
    for (auto [v, w] : e[u])
      if (v != f && ~siz[v]) init(v, u, rt, d + w);
  }
  int split(int u) {
    int n = calcsiz(u, u) / 2;
    while (true) {
      bool flg = true;
      for (auto [v, w] : e[u])
        if (siz[v] < siz[u] && siz[v] > n) {
          u = v;
          flg = false;
          break;
        }
      if (flg) break;
    }
    siz[u] = -1;
    init(u, u, u, 0);
    sort(ch[u].begin(), ch[u].end());
    for (auto [v, w] : e[u])
      if (~siz[v]) v = split(v);
    return u;
  }
  int rt;
  void ctrdecomp() {
    int n = e.size();
    siz.assign(n, 0);
    fa.assign(n, {});
    ch.assign(n, {});
    rt = split(1);
  }
  vector<int> solve() {
    vector<int> dis(e.size(), -1);
    dis[1] = 0;
    queue<int> que;
    que.push(1);
    while (que.size()) {
      int u = que.front();
      que.pop();
      for (auto [f, d] : fa[u]) {
        while (ch[f].size()) {
          auto [w, v] = ch[f].back();
          if (d > w) break;
          ch[f].pop_back();
          if (~dis[v]) continue;
          que.push(v);
          dis[v] = dis[u] + 1;
        }
      }
    }
    return dis;
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int tt;
  cin >> tt;
  while (tt--) {
    int n;
    cin >> n;
    graph g(n);
    for (int i = 2; i <= n; i++) cin >> g.r[i];
    for (int i = 1; i < n; i++) {
      ll u, v, w;
      cin >> u >> v >> w;
      g.adde(u, v, w);
    }
    g.ctrdecomp();
    auto res = g.solve();
    for (int i = 2; i <= n; i++) cout << res[i] << ' ';
    cout << el;
  }
  return 0;
}