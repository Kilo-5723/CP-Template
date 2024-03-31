// ver 2.0
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
  if ((det(b - a, c - a)) < 0) swap(b, c);  // guarantee a->c is above a->b
  a = a - o, b = b - o, c = c - o;
  auto d = det(a.x, a.y, a.abs2(), b.x, b.y, b.abs2(), c.x, c.y, c.abs2());
  return d > 0;
}
//         +-------------------------------+
//         |                               |
//         v                               v
// head = nil <--> edge1 <--> edge2 <--> edge3
struct edge {             // cyclic chain with a nil node indicating the head
  int to;                 // -1 for nil node
  edge *rev;              // rev: reverse edge
  array<edge *, 2> next;  // next[0]: counter-clockwise; next[1]: clockwise
  edge(int to = -1) : to(to), rev(NULL), next({NULL, NULL}) {}
  static void *operator new(size_t count) {
    static edge *begin = NULL, *end = NULL;
    if (begin == end) begin = (edge *)malloc(count * 100), end = begin + 100;
    return begin++;
  }
  // insert this in direction d of u
  void insert(edge *u, bool d) {
    auto v = u->next[d];  // u -> this -> v
    u->next[d] = this, this->next[!d] = u;
    this->next[d] = v, v->next[!d] = this;
  }
  // erase this
  void erase() {
    auto u = this->next[1], v = this->next[0];  // v -> u -> w
    u->next[0] = v, v->next[1] = u;
  }
  // next non-nil element in direction d of this (nil if not found)
  edge *nextelm(bool d) {
    auto u = this->next[d];
    return ~u->to ? u : u->next[d];
  }
};
// generate a new edge uv and reverse edge vu
pair<edge *, edge *> newedge(int u, int v) {
  auto uv = new edge(v), vu = new edge(u);
  uv->rev = vu, vu->rev = uv;
  return {uv, vu};
}
// find DT of a[ord[l]]..a[ord[r-1]] stored in h
// return the lowest point of convex hull a[ord[l]]..a[ord[r-1]] for speed
// neighbours of a vertex are sorted in a cyclic chain heading by h[u]
// the begin and end of the chain are both borders of convex hull if applicable
int divconq(const vector<cplx> &a, const vector<int> &ord, vector<edge *> &h,
            int l, int r) {
  if (r - l <= 2) {
    if (r - l == 1) return ord[l];  // return the only point of convex
    auto u = ord[l], v = ord[l + 1];
    auto [uv, vu] = newedge(u, v);
    uv->insert(h[u], 0), vu->insert(h[v], 0);
    return a[u].y < a[v].y ? u : v;  // return low point of convex
  }
  int u = divconq(a, ord, h, l, l + (r - l) / 2),  // low point of left convex
      v = divconq(a, ord, h, l + (r - l) / 2, r),  // low point of right convex
      w = a[u].y < a[v].y ? u : v;                 // low point of convex
  // find neighbour of u lower than line uv (current LR-edge)
  // f = 0: not flipped, 1: fliped by y-axis to handle mirror situation
  auto lower = [&](int u, int v, bool f) -> optional<int> {
    for (auto e = h[u]->next[0]; e != h[u]; e = e->next[0]) {
      auto w = e->to;
      auto d = det(a[u] - a[v], a[w] - a[v]) * (!f - f);  // * -1 if flipped
      if (d > 0 || d == 0 && (a[u] - a[v]).abs2() > (a[w] - a[v]).abs2())
        return w;
    }
    return nullopt;
  };
  // find base LR-edge
  int flg = 1;  // flg: 1 -> u updated, 0 -> v updated
  // guarantee neighbours of each vertex will be traversed at most twice
  while (true) {
    if (flg) {                             // if u is updated
      if (auto res = lower(u, v, 0))       // try update u again
        u = *res;                          // if new u founded, update u
      else if (auto res = lower(v, u, 1))  // else, check the other side
        v = *res, flg = 0;                 // if new v founded, update v
      else  // else, no newer u, v can be found, break
        break;
    } else {                               // if v is updated
      if (auto res = lower(v, u, 1))       // try update v again
        v = *res;                          // if new v founded, update v
      else if (auto res = lower(u, v, 0))  // else, check the other side
        u = *res, flg = 1;                 // if new u founded, update u
      else  // else, no newer u, v can be found, break
        break;
    }
  }
  // insert base LR-edge, note that uv is the first edge of u and last of v
  auto [uv, vu] = newedge(u, v);
  uv->insert(h[u], 0);  // insert uv in the front of h[u]
  vu->insert(h[v], 1);  // insert vu in the back of h[v]
  // next candidate, flipped by y-axis if f = 1
  auto candidate = [&](int u, int v, edge *uv, bool f) {
    auto e1 = uv->nextelm(f);       // e1 is the candidate
    for (auto e2 = e1->nextelm(f);  // e2 is the next candidate
         det(a[v] - a[u], a[e1->to] - a[u]) * (!f - f) > 0 &&  // < 180 degree
         inside(a[u], a[v], a[e1->to], a[e2->to]);  // e2 inside circle u,v,e1
         e1 = e2, e2 = e1->nextelm(f))              // note that e2 maybe uv
      e1->rev->erase(), e1->erase();                // erase e1
    return det(a[v] - a[u], a[e1->to] - a[u]) * (!f - f) > 0 ? e1 : NULL;
  };
  // update next LR edge as wv, flipped by y-axis if f=1
  auto updatelr = [&](int &u, int v, edge *&uv, edge *&vu, edge *uw, bool f) {
    int w = uw->to;
    auto wu = uw->rev;
    auto [wv, vw] = newedge(w, v);
    wv->insert(wu, f);
    vw->insert(vu, !f);
    u = w, uv = wv, vu = vw;
  };
  while (true) {
    auto uu2 = candidate(u, v, uv, 0),
         vv2 = candidate(v, u, vu, 1);  // find potential candidate
    if (!uu2 && !vv2)
      break;  // no candidate found
    else if (!vv2)
      updatelr(u, v, uv, vu, uu2, 0);  // candidate u found
    else if (!uu2)
      updatelr(v, u, vu, uv, vv2, 1);  // candidate v found
    else                               // both found
      if (inside(a[u], a[v], a[uu2->to], a[vv2->to]))
        updatelr(v, u, vu, uv, vv2, 1);  // v' inside uvu', choose v'
      else
        updatelr(u, v, uv, vu, uu2, 0);  // v' not inside uvu', choose u'
  }
  // uv is on the border of convex hull, be the last edge of u and first of v
  uv->erase(), vu->erase();
  uv->insert(h[u], 1);
  vu->insert(h[v], 0);
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
  vector<edge *> h(n);
  for (auto &e : h) e = new edge(), e->next = {e, e};
  divconq(a, ord, h, 0, n);
  vector<vector<int>> res(n);
  for (int i = 0; i < n; i++)
    for (auto e = h[i]->next[0]; e != h[i]; e = e->next[0])
      res[i].push_back(e->to);
  return res;
}
// verified by:
// https://www.luogu.com.cn/problem/P6362
// 