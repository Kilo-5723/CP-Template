struct node {
  node *ls, *rs;
  int l, r, v;
  void build(int _l, int _r) {
    l = _l, r = _r, v = 0;
    if (l == r) return;
    int m = l + (r - l) / 2;
    ls = new node(), ls->build(l, m);
    rs = new node(), rs->build(m + 1, r);
  }
  int query(int _l, int _r) {
    if (_r < l || _l > r) return 0;
    if (_l <= l && _r >= r) return v;
    return ls->query(_l, _r) + rs->query(_l, _r);
  }
};
node *add(node *rt, int p, int a) {
  if (p < rt->l || p > rt->r) return rt;
  auto u = new node();
  *u = *rt;
  u->v += a;
  if (u->l == u->r) return u;
  u->ls = add(u->ls, p, a);
  u->rs = add(u->rs, p, a);
  return u;
}