
struct node {
  array<node *, alpha> ch;
  array<node *, sizf> fa;
  int dep;
  ll val;
  node() {
    for (auto &v : ch) v = NULL;
    for (auto &v : fa) v = NULL;
    dep = 0;
    val = 0;
  }
};
node *rt;
struct cmp {
  bool operator()(node *a, node *b) {
    if (a == b) return false;
    if (!a) return true;
    if (!b) return false;
    if (a->dep != b->dep) return a->dep > b->dep;
    for (int i = sizf - 1; i >= 0; i--)
      if (a->fa[i] != b->fa[i]) a = a->fa[i], b = b->fa[i];
    node *u = a->fa[0];
    for (int i = 0; i < alpha; i++) {
      if (a == u->ch[i]) return false;
      if (b == u->ch[i]) return true;
    }
  }
  bool operator()(pair<node *, int> a, pair<node *, int> b) {
    return cmp()(a.first, b.first);
  }
};
node *add(node *u, int n) {
  if (n / 10) u = add(u, n / 10);
  n %= 10;
  if (u->ch[n]) return u->ch[n];
  u->ch[n] = new node();
  auto v = u->ch[n];
  v->fa[0] = u;
  for (int i = 1; i < sizf; i++) v->fa[i] = v->fa[i - 1]->fa[i - 1];
  v->dep = u->dep + 1;
  v->val = (u->val * 10 + n) % mod;
  return v;
}