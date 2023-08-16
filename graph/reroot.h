
struct graph {
  vector<vector<int>> e;
  ll k, c;
  graph(int n) { e.resize(n + 1); }
  void adde(int u, int v) {
    e[u].push_back(v);
    e[v].push_back(u);
  }
  vector<int> dis, dep;
  ll ans;
  void setup(int u, int f) {
    dep[u] = 0;
    for (auto v : e[u])
      if (v != f) {
        dis[v] = dis[u] + 1;
        setup(v, u);
        dep[u] = max(dep[u], dep[v] + 1);
      }
  }
  void reroot(int u, int f) {
    int mxm1 = -1, mxm2 = -1;
    for (auto v : e[u]) {
      mxm2 = max(mxm2, dep[v]);
      if (mxm2 > mxm1) swap(mxm1, mxm2);
    }
    dep[u] = mxm1 + 1;
    ans = max(ans, dep[u] * k - dis[u] * c);
    for (auto v : e[u])
      if (v != f) {
        dep[u] = (dep[v] == mxm1 ? mxm2 : mxm1) + 1;
        reroot(v, u);
      }
  }
  ll solve(int s) {
    dis.resize(e.size());
    dep.resize(e.size());
    dep[s] = 0;
    setup(s, s);
    ans = 0;
    reroot(s, s);
    return ans;
  }
};