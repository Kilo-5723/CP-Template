
  auto cmp = [&](int u, int v) {
    if (g.e[u].size() != g.e[v].size()) return g.e[u].size() < g.e[v].size();
    return u < v;
  };
  set<int, decltype(cmp)> st(cmp);