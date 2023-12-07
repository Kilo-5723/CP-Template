#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct range {
  int l, r;
  bool valid() const { return r >= l; }
  bool cover(range rg) const { return l <= rg.l && r >= rg.r; }
  bool cross(range rg) const { return l <= rg.r && r >= rg.l; }
};
bool operator<(range a, range b) {
  return a.l < b.l || a.l == b.l && a.r > b.r;
}
struct rngseq {
  static const int inf = 1e9 + 7;
  set<range> seq;
  rngseq() : seq({{-inf, -inf}, {inf, inf}}) {}
  void add(range rg) {
    if (!rg.valid()) return;
    seq.insert(rg);
    rt->qadd(rg.l, rg.r, 1);
  }
  void rmv(range rg) {
    if (!rg.valid()) return;
    seq.erase(rg);
    rt->qadd(rg.l, rg.r, -1);
  }
  void insert(range rg) {
    auto it = seq.upper_bound(rg);
    auto tmp = *prev(it);
    if (tmp.cover(rg)) return;
    if (tmp.cross(rg)) {
      rg.l = tmp.l;
      rmv(tmp);
    }
    while (rg.cross(*it)) {
      tmp = *it++;
      rg.r = max(rg.r, tmp.r);
      rmv(tmp);
    }
    add(rg);
  }
  void erase(range rg) {
    auto it = seq.upper_bound(rg);
    auto tmp = *prev(it);
    if (tmp.cover(rg)) {
      rmv(tmp);
      add({tmp.l, rg.l - 1}), add({rg.r + 1, tmp.r});
      return;
    }
    if (tmp.cross(rg)) {
      rmv(tmp);
      add({tmp.l, rg.l - 1});
    }
    while (rg.cross(*it)) {
      tmp = *it++;
      rmv(tmp);
      add({rg.r + 1, tmp.r});
    }
  }
};