#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll inf = 1e18;
struct node {
  node *ls, *rs;
  int l, r;
  ll val, sum;
  pair<ll, int> mxm, mnm;
  void update() {
    sum = ls->sum + rs->sum;
    mxm = max(ls->mxm, rs->mxm);
    mnm = min(ls->mnm, rs->mnm);
  }
  void build(int _l, int _r, vector<ll> &a) {
    l = _l, r = _r;
    if (l == r) {
      val = a[l];
      mxm = mnm = {val, l};
      sum = abs(val);
      return;
    }
    int m = (l + r) / 2;
    ls = new node(), ls->build(l, m, a);
    rs = new node(), rs->build(m + 1, r, a);
    update();
  }
  void modify(int p, int a) {
    if (p < l || p > r) return;
    if (l == r) {
      val = val + a;
      mxm = mnm = {val, l};
      sum = abs(val);
      return;
    }
    ls->modify(p, a), rs->modify(p, a);
    update();
  }
  int ppos(int _l, int _r) {
    if (_r < l || _l > r) return -1;
    if (_l <= l && _r >= r) {
      if (mxm.first < 0) return -1;
      if (l == r) return l;
    }
    auto res = ls->ppos(_l, _r);
    if (~res)
      return res;
    else
      return rs->ppos(_l, _r);
  }
  pair<ll, int> qmxm(int _l, int _r) {
    if (_r < l || _l > r) return {-inf, -1};
    if (_l <= l && _r >= r) return mxm;
    return max(ls->qmxm(_l, _r), rs->qmxm(_l, _r));
  }
  pair<ll, int> qmnm(int _l, int _r) {
    if (_r < l || _l > r) return {inf, -1};
    if (_l <= l && _r >= r) return mnm;
    return min(ls->qmnm(_l, _r), rs->qmnm(_l, _r));
  }
  ll qsum(int _l, int _r) {
    if (_r < l || _l > r) return 0;
    if (_l <= l && _r >= r) return sum;
    return ls->qsum(_l, _r) + rs->qsum(_l, _r);
  }
};

const ll inf = 1e18;
int ls(int u) { return u << 1; }
int rs(int u) { return u << 1 | 1; }
struct segtree {
  vector<ll> mnm;
  int l, r;
  void build(int l_, int r_) {
    l = l_, r = r_;
    int k = 1;
    while (k < r - l + 1) k <<= 1;
    mnm.resize(k << 1);
  }
  void update(int u) { mnm[u] = min(mnm[ls(u)], mnm[rs(u)]); }
  void init(int mode, int l = -1, int r = -1, int u = 1) {
    if (!~l) l = this->l, r = this->r;
    if (l == r) {
      mnm[u] = (~mode ? mode ? inf : l : -l);
      return;
    }
    int m = l + (r - l) / 2;
    init(mode, l, m, ls(u));
    init(mode, m + 1, r, rs(u));
    update(u);
  }
  void modify(int p, ll a, int l = -1, int r = -1, int u = 1) {
    if (!~l) l = this->l, r = this->r;
    if (p < l || p > r) return;
    if (l == r) {
      mnm[u] = a;
      return;
    }
    int m = l + (r - l) / 2;
    modify(p, a, l, m, ls(u));
    modify(p, a, m + 1, r, rs(u));
    update(u);
  }
  ll qmnm(int l_, int r_, int l = -1, int r = -1, int u = 1) {
    if (!~l) l = this->l, r = this->r;
    if (r_ < l || l_ > r) return inf;
    if (l_ <= l && r_ >= r) return mnm[u];
    int m = l + (r - l) / 2;
    return min(qmnm(l_, r_, l, m, ls(u)), qmnm(l_, r_, m + 1, r, rs(u)));
  }
};