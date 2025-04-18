template <class val_t, class tag_t, auto unit, auto nil, auto mul, auto add,
          auto comb>
struct segtree {
  segtree() {}
  struct node {
    node *ls, *rs;
    int l, r;
    val_t val;
    tag_t tag;
    node(int l, int r)
        : ls(NULL), rs(NULL), l(l), r(r), val(unit()), tag(nil()) {}
    static void *operator new(size_t count) {
      static node *begin = nullptr, *end = nullptr;
      if (begin == end)
        begin = (node *)malloc(count * 1000), end = begin + 1000;
      return begin++;
    }
  };
  node *rt;
  void init(int n) { init(n + 1, unit()); }
  void init(vector<val_t> a) {
    assert(a.size() > 0);
    auto build = [&](auto &&self, int l, int r) -> node * {
      auto u = new node(l, r);
      if (r - l == 1) {
        u->val = a[l];
      } else {
        u->ls = self(self, l, l + r >> 1);
        u->rs = self(self, l + r >> 1, r);
        update(u);
      }
      return u;
    };
    rt = build(build, 0, a.size());
  }
  void addtag(node *u, tag_t tag) {
    u->val = add(u->val, tag), u->tag = comb(u->tag, tag);
  }
  void pushdown(node *u) {
    if (u->tag != nil())
      addtag(u->ls, u->tag), addtag(u->rs, u->tag), u->tag = nil();
  }
  void update(node *u) { u->val = mul(u->ls->val, u->rs->val); }
  void modify(node *u, int p, val_t v) {
    if (p < u->l || p >= u->r) return;
    if (u->r - u->l == 1) {
      u->val = v;
      return;
    }
    pushdown(u);
    modify(u->ls, p, v), modify(u->rs, p, v);
    update(u);
  }
  void apply(node *u, int l, int r, tag_t tag) {
    if (r <= u->l || u->r <= l) return;
    if (l <= u->l && u->r <= r) {
      addtag(u, tag);
      return;
    }
    pushdown(u);
    apply(u->ls, l, r, tag), apply(u->rs, l, r, tag);
    update(u);
  }
  val_t query(node *u, int l, int r) {
    if (r <= u->l || u->r <= l) return unit();
    if (l <= u->l && u->r <= r) return u->val;
    pushdown(u);
    return mul(query(u->ls, l, r), query(u->rs, l, r));
  }
  void modify(int p, val_t v) { modify(rt, p, v); }
  void apply(int l, int r, tag_t tag) { apply(rt, l, r, tag); }
  val_t query(int l, int r) { return query(rt, l, r); }
};