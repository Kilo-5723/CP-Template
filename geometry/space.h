#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;
typedef long double ld;
// typedef __int128 double;
struct cplx {
  ld x, y, z;
  ld abs() { return sqrt(1.0 * x * x + 1.0 * y * y + 1.0 * z * z); }
};
cplx operator+(cplx a, cplx b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
cplx operator-(cplx a, cplx b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
cplx operator*(cplx a, ld b) { return {a.x * b, a.y * b, a.z * b}; }
cplx operator*(ld b, cplx a) { return {a.x * b, a.y * b, a.z * b}; }
cplx det(cplx a, cplx b) {
  return {a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}
ld dot(cplx a, cplx b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
const ld inf = 1e30, eps = 1e-8;
ld dist(array<cplx, 3> a, cplx b) {
  cplx h = det(a[0] - a[1], a[0] - a[2]);
  ld dt = dot(b, h);
  if (dt < eps) return inf;
  ld cp = dot(a[0], h);
  cplx o = cp / dt * b;
  ld sa = det(a[0] - o, a[1] - o).abs() + det(a[1] - o, a[2] - o).abs() +
          det(a[2] - o, a[0] - o).abs(),
     sb = h.abs();
  if (sa / sb > 1 + eps) return inf;
  return o.abs();
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(3);
  int n, m;
  cin >> n >> m;
  vector a(n, array<cplx, 3>());
  for (auto &t : a) {
    for (auto &[x, y, z] : t) cin >> x >> y >> z;
    if (dot(det(t[0] - t[1], t[0] - t[2]), t[0]) < 0) swap(t[1], t[2]);
  }
  while (m--) {
    cplx b;
    cin >> b.x >> b.y >> b.z;
    int p = -1;
    ld val = inf / 2;
    for (int i = 0; i < n; i++) {
      auto tmp = dist(a[i], b);
      if (tmp < val) {
        p = i, val = tmp;
      }
    }
    cout << p + 1 << el;
  }
  return 0;
}