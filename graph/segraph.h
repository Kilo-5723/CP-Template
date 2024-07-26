// ver 2.0
// segment tree graph that initial with node n
struct segraph {
  vector<int> a;
  int n, rt;  // in case of |a| = 1
  vector<int> ls, rs;
  segraph(vector<int> a, int n) : a(a), n(n) {
    ls.reserve(2 * a.size()), rs.reserve(2 * a.size());
    rt = build(0, a.size());
  }
  vector<pair<int, int>> e;
  int vsize() { return ls.size(); }
  int nnode() {
    ls.push_back(0), rs.push_back(0);
    return ls.size() - 1;
  }
  static int mid(int l, int r) { return (l + r) >> 1; }
  int build(int l, int r) {
    if (l + 1 == r) return a[l] - n;
    int u = nnode();
    ls[u] = build(l, mid(l, r)), rs[u] = build(mid(l, r), r);
    return u;
  }
  vector<pair<int, int>> edge() {
    vector<pair<int, int>> res;
    for (int u = 0; u < vsize(); u++)
      res.push_back({u + n, ls[u] + n}), res.push_back({u + n, rs[u] + n});
    return res;
  }
  vector<int> covers(int l, int r) {
    vector<int> res;
    auto dfs = [&](auto &&self, int u, int vl, int vr, int ql, int qr) -> void {
      if (qr <= vl || vr <= ql) return;
      if (ql <= vl && vr <= qr) {
        res.push_back(u + n);
        return;
      }
      self(self, ls[u], vl, mid(vl, vr), ql, qr);
      self(self, rs[u], mid(vl, vr), vr, ql, qr);
    };
    dfs(dfs, rt, 0, a.size(), l, r);
    return res;
  }
};