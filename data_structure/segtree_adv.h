#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
template <class val_t, class tag_t, auto unit, auto nil, auto mul, auto comb,
          auto check, auto add>
struct segtree {
  void addtag(node *u, tag_t tag) {
    u->tag = comb(u->tag, tag);
    if (check(u->val, tag))
      u->val = add(u->val, tag);
    else
      pushdown(u), update(u);
  }
};

// 吉司机线段树 https://loj.ac/p/6565
const ll inf = 1e18;
struct value {
  ll max, max_cnt, max2, min, min_cnt, min2, sum, size;
};
struct tag {
  ll add, low, up;
};
value unit() { return {-inf, 0ll, -inf, inf, 0ll, inf, 0ll, 0ll}; }
value init(ll v) { return {v, 1ll, -inf, v, 1ll, inf, v, 1ll}; }
tag nil() { return {0ll, -inf, inf}; }
value mul(value a, value b) {
  value c;
  c.max = max(a.max, b.max);
  c.max_cnt = (c.max == a.max) * a.max_cnt + (c.max == b.max) * b.max_cnt;
  c.max2 = a.max == b.max  ? max(a.max2, b.max2)
           : a.max < b.max ? max(a.max, b.max2)
                           : max(a.max2, b.max);
  c.min = min(a.min, b.min);
  c.min_cnt = (c.min == a.min) * a.min_cnt + (c.min == b.min) * b.min_cnt;
  c.min2 = a.min == b.min  ? min(a.min2, b.min2)
           : a.min > b.min ? min(a.min, b.min2)
                           : min(a.min2, b.min);
  c.sum = a.sum + b.sum, c.size = a.size + b.size;
  return c;
}
tag comb(tag a, tag b) {
  assert(b.low <= b.up);
  tag c;
  c.add = a.add + b.add;
  c.up = max(min(b.up, a.up + b.add), b.low);
  c.low = min(max(b.low, a.low + b.add), b.up);
  return c;
}
bool check(const value &a, const tag &b) {
  assert(b.low <= b.up);
  return a.max2 + b.add < b.up && a.min2 + b.add > b.low;
}
value add(value a, tag b) {
  assert(b.low <= b.up);
  value c;
  c.max = a.max + b.add, c.max_cnt = a.max_cnt, c.max2 = a.max2 + b.add;
  c.min = a.min + b.add, c.min_cnt = a.min_cnt, c.min2 = a.min2 + b.add;
  c.sum = a.sum + a.size * b.add, c.size = a.size;
  if (c.max > b.up) {
    c.sum -= c.max_cnt * (c.max - b.up);
    if (c.max == c.min) c.min = b.up;
    if (c.max == c.min2) c.min2 = b.up;
    c.max = b.up;
  }
  if (c.min < b.low) {
    c.sum -= c.min_cnt * (c.min - b.low);
    if (c.min == c.max) c.max = b.low;
    if (c.min == c.max2) c.max2 = b.low;
    c.min = b.low;
  }
  return c;
}