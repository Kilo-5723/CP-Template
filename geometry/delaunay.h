// ver 2.0
int sgn(ll n) { return (n > 0) - (n < 0); }
int sgn(li n) { return (n > 0) - (n < 0); }
struct cplx {
  ll x, y;
  ll abs2() const { return x * x + y * y; }
};
cplx operator+(const cplx &a, const cplx &b) { return {a.x + b.x, a.y + b.y}; }
cplx operator-(const cplx &a, const cplx &b) { return {a.x - b.x, a.y - b.y}; }
bool operator<(const cplx &a, const cplx &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}
bool operator!=(const cplx &a, const cplx &b) { return a < b || b < a; }
bool operator==(const cplx &a, const cplx &b) { return !(a != b); }
ll det(const cplx &a, const cplx &b) { return a.x * b.y - a.y * b.x; }
li det(li a11, li a12, li a13, li a21, li a22, li a23, li a31, li a32, li a33) {
  return a11 * (a22 * a33 - a23 * a32) + a12 * (a23 * a31 - a21 * a33) +
         a13 * (a21 * a32 - a22 * a31);
}
// check whether o is strictly inside the circle passing a, b, c
// ref: https://zhuanlan.zhihu.com/p/534117002
bool inside(cplx a, cplx b, cplx c, cplx o) {
  if (sgn(det(b - a, c - a)) < 0) swap(b, c);  // guarantee a->c is above a->b
  a = a - o, b = b - o, c = c - o;
  auto d = det(a.x, a.y, a.abs2(), b.x, b.y, b.abs2(), c.x, c.y, c.abs2());
  return sgn(d) > 0;
}
struct edge {
  int v;
  list<edge>::iterator rev;
};
// find DT of a[ord[l]]..a[ord[r-1]] stored in e
// return an arbitrary point (low for speed) of convex a[ord[l]]..a[ord[r-1]]
// edges are sorted counter-clockwise
// begin is the first edge on halfplane for vertex on the border of convex hull
int divconq(const vector<cplx> &a, const vector<int> &ord,
            vector<list<edge>> &e, int l, int r) {
  if (r - l <= 2) {
    if (r - l == 1) return ord[l];  // return the only point of convex
    if (r - l == 2) {
      auto u = ord[l], v = ord[l + 1];
      auto euv = e[u].insert(e[u].end(), edge()),
           evu = e[v].insert(e[v].end(), edge());
      *euv = {v, evu}, *evu = {u, euv};
      return a[u].y < a[v].y ? u : v;
    }  // return low point of convex
  }
  int u = divconq(a, ord, e, l, l + (r - l) / 2),  // low point of left convex
      v = divconq(a, ord, e, l + (r - l) / 2, r),  // low point of right convex
      w = a[u].y < a[v].y ? u : v;                 // low point of convex
  // find neighbour of u lower than line uv (current LR-edge)
  auto lowerl = [&](int u, int v) -> optional<int> {
    for (auto [w, r] : e[u]) {
      auto d = det(a[u] - a[v], a[w] - a[v]);
      if (sgn(d) > 0 ||
          sgn(d) == 0 && (a[u] - a[v]).abs2() > (a[w] - a[v]).abs2())
        return w;
    }
    return nullopt;
  };
  // find neighbour of v lower than line uv (current LR-edge)
  auto lowerr = [&](int u, int v) -> optional<int> {
    for (auto [w, r] : e[v]) {
      auto d = det(a[v] - a[u], a[w] - a[u]);
      if (sgn(d) < 0 ||
          sgn(d) == 0 && (a[v] - a[u]).abs2() > (a[w] - a[u]).abs2())
        return w;
    }
    return nullopt;
  };
  // find base LR-edge
  int flg = 1;  // flg: 1 -> u updated, 0 -> v updated
  // guarantee neighbours of each vertex will be traversed at most twice
  while (true) {
    if (flg) {                           // if u is updated
      if (auto res = lowerl(u, v))       // try update u again
        u = *res;                        // if new u founded, update u
      else if (auto res = lowerr(u, v))  // else, check the other side
        v = *res, flg = 0;               // if new v founded, update v
      else  // else, no newer u, v can be found, break
        break;
    } else {                             // if v is updated
      if (auto res = lowerr(u, v))       // try update v again
        v = *res;                        // if new v founded, update v
      else if (auto res = lowerl(u, v))  // else, check the other side
        u = *res, flg = 1;               // if new u founded, update u
      else  // else, no newer u, v can be found, break
        break;
    }
  }
  // insert base LR-edge, note that uv is the first edge of u and last of v
  auto euv = e[u].insert(e[u].begin(), edge()),
       evu = e[v].insert(e[v].end(), edge());
  *euv = {v, evu}, *evu = {u, euv};
  // next element, guarantee that it points to a element of a
  auto nexte = [](list<edge> &a, list<edge>::iterator it) {
    return ++it == a.end() ? a.begin() : it;
  };
  // prev element, guarantee that it points to a element of a
  auto preve = [](list<edge> &a, list<edge>::iterator it) {
    return it == a.begin() ? --a.end() : --it;
  };
  // next left candidate
  auto nextl = [&](int u, int v,
                   list<edge>::iterator euv) -> optional<list<edge>::iterator> {
    auto p = nexte(e[u], euv);
    for (auto q = nexte(e[u], p); sgn(det(a[v] - a[u], a[p->v] - a[u])) > 0 &&
                                  inside(a[u], a[v], a[p->v], a[q->v]);
         p = q, q = nexte(e[u], p)) {  // while q is in the circle uvp
      int w = p->v;
      e[w].erase(p->rev), e[u].erase(p);  // erase edge p
    }                                     // note that q may be euv
    return sgn(det(a[v] - a[u], a[p->v] - a[u])) > 0 ? optional(p) : nullopt;
  };
  // next right candidate
  auto nextr = [&](int u, int v,
                   list<edge>::iterator evu) -> optional<list<edge>::iterator> {
    auto p = preve(e[v], evu);
    for (auto q = preve(e[v], p); sgn(det(a[u] - a[v], a[p->v] - a[v])) < 0 &&
                                  inside(a[u], a[v], a[p->v], a[q->v]);
         p = q, q = preve(e[v], p)) {  // same with nextl
      int w = p->v;
      e[w].erase(p->rev), e[v].erase(p);
    }
    return sgn(det(a[u] - a[v], a[p->v] - a[v])) < 0 ? optional(p) : nullopt;
  };
  auto updu = [&](int &u, int v, list<edge>::iterator &euv,
                  list<edge>::iterator &evu, list<edge>::iterator &euu) {
    int uu = euu->v;
    auto ruu = euu->rev;
    auto euuv = e[uu].insert(next(ruu), edge()),
         evuu = e[v].insert(evu, edge());
    u = uu, euv = euuv, evu = evuu;
    *euv = {v, evu}, *evu = {u, euv};
  };
  auto updv = [&](int u, int &v, list<edge>::iterator &euv,
                  list<edge>::iterator &evu, list<edge>::iterator &evv) {
    int vv = evv->v;
    auto rvv = evv->rev;
    auto evvu = e[vv].insert(rvv, edge()),
         euvv = e[u].insert(next(euv), edge());
    v = vv, euv = euvv, evu = evvu;
    *euv = {v, evu}, *evu = {u, euv};
  };
  while (true) {
    auto euu = nextl(u, v, euv),
         evv = nextr(u, v, evu);  // find potential candidate
    if (!euu && !evv) break;      // none potential candidate found
    if (!euu) {
      updv(u, v, euv, evu, *evv);  // potential u found
      continue;
    }
    if (!evv) {
      updu(u, v, euv, evu, *euu);  // potential v found
      continue;
    }
    // both found
    if (inside(a[u], a[v], a[(*euu)->v], a[(*evv)->v]))
      updv(u, v, euv, evu, *evv);  // v' inside uvu', choose v'
    else
      updu(u, v, euv, evu, *euu);  // v' not inside uvu', choose u'
  }
  // uv is on the border of convex hull, be the last edge of u and first of v
  e[u].erase(euv), e[v].erase(evu);
  euv = e[u].insert(e[u].end(), edge());
  evu = e[v].insert(e[v].begin(), edge());
  *euv = {v, evu}, *evu = {u, euv};
  return w;  // return low point of convex
}
// find the deuaunay tranagulation of a vertex set with no repeat nodes
// edges are sorted counter-clockwise
// ref: http://www.geom.uiuc.edu/~samuelp/del_project.html
vector<vector<int>> delaunay(const vector<cplx> &a) {
  int n = a.size();
  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) { return a[i] < a[j]; });
  vector<list<edge>> e(n);
  divconq(a, ord, e, 0, n);
  vector<vector<int>> res(n);
  for (int i = 0; i < n; i++)
    for (auto &[v, r] : e[i]) res[i].push_back(v);
  return res;
}