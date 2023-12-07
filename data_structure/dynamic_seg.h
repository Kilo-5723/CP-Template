#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
const int k = 18;
typedef array<int, k> mtrx;
mtrx flip(mtrx a) {
  mtrx b;
  b.fill(0);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < k; j++) b[j] |= (a[i] >> j & 1) << i;
  return b;
}
mtrx operator*(mtrx a, mtrx b) {
  b = flip(b);
  mtrx c;
  c.fill(0);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < k; j++) c[i] |= ((a[i] & b[j]) > 0) << j;
  return c;
}
mtrx init(int n) {
  if (n > 6) n = 6 + n % 3;
  static unordered_map<int, mtrx> mem;
  if (mem.count(n)) return mem[n];
  auto &res = mem[n];
  res.fill(0);
  for (auto e : {0, 1})
    for (auto a : {0, 1, 2})
      for (auto b : {0, 1, 2}) {
        int l = e * 9 + a * 3 + b;
        int m = n - a - b;
        if (m < 0) continue;
        int r = e * 9 + b * 3 + m % 3;
        res[l] |= 1 << r;
        if (!e && m >= 2) {
          r = 9 + b * 3 + (m - 2) % 3;
          res[l] |= 1 << r;
        }
      }
  return res;
}
struct node {
  node *ls, *rs;
  int l, r, v;
  mtrx m;
  node(int l, int r) : l(l), r(r), v(0), m(init(0)), ls(NULL), rs(NULL) {}
  static node *nnode(int l, int r) {
    static const int buff = 1000;
    static node *ptr = NULL, *cur = NULL;
    if (cur == ptr)
      ptr = (node *)new char[sizeof(node) * buff], cur = ptr + buff;
    return new (--cur) node(l, r);
  }
  void update() { m = ls->m * rs->m; }
  void pushdown() {
    if (ls || rs) return;
    int m = l + (r - l) / 2;
    ls = nnode(l, m);
    rs = nnode(m + 1, r);
  }
  mtrx qsum(int _l, int _r) {
    if (_r < l || _l > r) return init(0);
    if (_l <= l && _r >= r) return m;
    pushdown();
    return ls->qsum(_l, _r) * rs->qsum(_l, _r);
  }
  void add(int p, int a) {
    if (p < l || p > r) return;
    if (l == r) {
      v += a;
      m = init(v);
      return;
    }
    pushdown();
    ls->add(p, a);
    rs->add(p, a);
    update();
  }
};
auto nnode = node::nnode;