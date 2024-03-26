// ver 1.5
struct node {
  int l, r;
  node *ls, *rs;
  int tag;
  node() : l(0), r(0), ls(nullptr), rs(nullptr), tag(0) {}
  static void* operator new(size_t count) {
    static node *begin = nullptr, *end = nullptr;
    if (begin == end) begin = (node*)malloc(count * 1000), end = begin + 1000;
    return begin++;
  }
};
node* build(int l, int r) {
  node* u = new node();
  u->l = l, u->r = r;
  if (l == r) return u;
  int m = l + (r - l) / 2;
  u->ls = build(l, m);
  u->rs = build(m + 1, r);
  return u;
}
node* addtag(node* u, int val) {
  u = new node(*u);
  u->tag += val;
  return u;
}
node* pushdown(node* u) {
  u = new node(*u);
  if (u->tag) {
    u->ls = addtag(u->ls, u->tag);
    u->rs = addtag(u->rs, u->tag);
    u->tag = 0;
  }
  return u;
}
node* add(node* u, int l, int r, int val) {
  if (u->l > r || u->r < l) return u;
  if (l <= u->l && u->r <= r) return addtag(u, val);
  u = pushdown(u);
  u->ls = add(u->ls, l, r, val);
  u->rs = add(u->rs, l, r, val);
  return u;
}
node* merge(node* u, node* v, int p) {
  if (u->r <= p) return u;
  if (v->l > p) return v;
  u = pushdown(u), v = pushdown(v);
  node* w = new node(*u);
  w->ls = merge(u->ls, v->ls, p);
  w->rs = merge(u->rs, v->rs, p);
  return w;
}
int val(node* u, int p) {
  if (u->r < p || u->l > p) return 0;
  if (u->l == u->r) return u->tag;
  return u->tag + val(u->ls, p) + val(u->rs, p);
}