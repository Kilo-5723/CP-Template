#include <bits/stdc++.h>
typedef long long ll;
using namespace std;
const ll inf = 1e18;

template <class val_t, class tag_t, auto unit, auto nil, auto mul, auto comb,
          auto add>
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

// history max https://www.luogu.com.cn/problem/P4314
const ll inf = 1e18;
struct value {
  ll max, hist_max;
};
struct tag {
  ll add, add_max, cvr, cvr_max;
};
value unit() { return {-inf, -inf}; }
value init(ll v) { return {v, v}; }
tag nil() { return {0, 0, inf, -inf}; }
value mul(value a, value b) {
  value c;
  c.max = max(a.max, b.max);
  c.hist_max = max(a.hist_max, b.hist_max);
  return c;
}
tag comb(tag a, tag b) {
  tag c;
  c.cvr = b.cvr < inf ? b.cvr : a.cvr < inf ? a.cvr + b.add : inf;
  c.add = a.cvr < inf ? a.add : a.add + b.add;
  c.add_max = a.add_max;
  if (a.cvr == inf) c.add_max = max(c.add_max, a.add + b.add_max);
  c.cvr_max = max(a.cvr_max, b.cvr_max);
  if (a.cvr < inf) c.cvr_max = max(c.cvr_max, a.cvr + b.add_max);
  return c;
}
value add(value a, tag b) {
  value c;
  c.max = b.cvr < inf ? b.cvr : a.max + b.add;
  c.hist_max = max({a.hist_max, a.max + b.add_max, b.cvr_max});
  return c;
}