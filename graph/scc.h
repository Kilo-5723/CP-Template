// ver 2.0
const int maxn = 5e5;
// return sccs for given edges
vector<vector<int>> scc_decomp(vector<pair<int, int>> ed) {
  static vector<vector<int>> e(maxn);
  static vector<int> dfn(maxn, 0), low(maxn, 0);
  vector<vector<int>> res;
  for (auto [u, v] : ed) e[u].push_back(v);
  vector<int> stk;
  static vector<bool> ins(maxn);
  int k = 0;
  auto dfs = [&](auto &&self, int u) -> void {
    stk.push_back(u), ins[u] = true, dfn[u] = low[u] = ++k;
    for (auto v : e[u])
      if (!dfn[v]) {
        self(self, v);
        low[u] = min(low[u], low[v]);
      } else if (ins[v]) {
        low[u] = min(low[u], dfn[v]);
      }
    if (dfn[u] == low[u]) {
      res.push_back({});
      int v;
      do {
        v = stk.back(), res.back().push_back(v);
        stk.pop_back(), ins[v] = false;
      } while (v != u);
    }
  };
  for (auto [u, v] : ed)
    if (!dfn[u]) dfs(dfs, u);
  // clear
  for (auto [u, v] : ed) {
    e[u].clear();
    dfn[u] = dfn[v] = low[u] = low[v] = 0;
  }
  return res;
}