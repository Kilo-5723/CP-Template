// return the block cut tree of given graph
// if given graph is cactus, edges of square are ordered as the cycle
vector<vector<int>> block_cut(const vector<vector<int>> &e) {
  int n = e.size();
  vector<int> ord(n);  // dfs order, 1-index
  vector<int> low(n);  // lowest ord(u) reachable by at most one backward edge
  vector<int> stk;     // current vertices in dfs stack
  // modified tarjan algorithm for finding bridges
  vector<vector<int>> res(n);
  auto tarjan = [&](auto &&self, int u, auto &&id) -> void {
    ord[u] = low[u] = ++id;
    stk.push_back(u);
    for (auto v : e[u]) {
      if (ord[v]) {  // visited, it's a backward edge
        low[u] = min(low[u], ord[v]);
        continue;
      }
      self(self, v, id);               // not visited, search subtree
      low[u] = min(low[u], low[v]);    // not a backward edge
      if (low[v] != ord[u]) continue;  // not bridge or biconnected component
      if (stk.back() == v) {           // (u, v) is a bridge
        res[u].push_back(v), res[v].push_back(u);  // add (u, v) to result
        stk.pop_back();
        continue;
      }
      res.push_back({});  // a new biconnected component found
      int w;
      do {
        w = stk.back(), stk.pop_back();
        res[n].push_back(w), res[w].push_back(n);  // add (w, n) to result
      } while (w != v);
      res[n].push_back(u), res[u].push_back(n);  // add (u, n) to result
      n++;
    }
  };
  for (int i = 1; i < e.size(); i++)
    if (!ord[i]) tarjan(tarjan, i, 0);
  return res;
}