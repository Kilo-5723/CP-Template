#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;
struct node {
  node *ls, *rs;
  int l, r, v;
  static node *nnode() {
    static const int buff = 1000;
    static node *ptr = new node[buff], *cur = ptr;
    if (cur == ptr + buff) ptr = new node[buff], cur = ptr;
    return cur++;
  }
  void build(int _l, int _r) {
    l = _l, r = _r, v = 0;
    if (l == r) return;
    int m = l + (r - l) / 2;
    ls = nnode(), ls->build(l, m);
    rs = nnode(), rs->build(m + 1, r);
  }
  int qsum(int _l, int _r) {
    if (_r < l || _l > r) return 0;
    if (_l <= l && _r >= r) return v;
    return ls->qsum(_l, _r) + rs->qsum(_l, _r);
  }
};
auto nnode = node::nnode;
node *add(node *rt, int p, int a) {
  if (p < rt->l || p > rt->r) return rt;
  auto u = nnode();
  *u = *rt;
  u->v += a;
  if (u->l == u->r) return u;
  u->ls = add(u->ls, p, a);
  u->rs = add(u->rs, p, a);
  return u;
}