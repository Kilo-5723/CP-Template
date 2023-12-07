#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef unsigned long long ull;
const int maxn = 1 << 25;
const int inf = 1e9;
const vector<pair<int, int>> dk = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                                   {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
const vector<pair<int, int>> dn = {{2, 1},   {1, 2},   {-1, 2}, {-2, 1},
                                   {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
const vector<vector<pair<int, int>>> db4 = {
    {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},                //
    {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},         //
    {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},  //
    {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}}};        //
tuple<int, int, int, int, int, int, int, int, int> its(int idx) {
  return {idx >> 24 & 1, idx >> 21 & 7, idx >> 18 & 7,
          idx >> 15 & 7, idx >> 12 & 7, idx >> 9 & 7,
          idx >> 6 & 7,  idx >> 3 & 7,  idx >> 0 & 7};
}
int sti(int mv, int kx, int ky, int wx, int wy, int nx, int ny, int bx,
        int by) {
  return mv << 24 | kx << 21 | ky << 18 | wx << 15 | wy << 12 | nx << 9 |
         ny << 6 | bx << 3 | by;
}
int state(int mv, int kx, int ky, int wx, int wy, int nx, int ny, int bx,
          int by) {  // -1: illegal; 0 ongoing; 1 dead; 2 checkmate
  static vector<int> mem(maxn, -2);
  if (max({kx, ky, wx, wy, nx, ny, bx, by}) > 7 ||
      min({kx, ky, wx, wy, nx, ny, bx, by}) < 0)
    return -1;
  auto &res = mem[sti(mv, kx, ky, wx, wy, nx, ny, bx, by)];
  if (res != -2) return res;
  if (kx == wx && ky == wy || wx == nx && wy == ny || nx == bx && ny == by ||
      bx == wx && by == wy)
    return res = -1;
  if (mv) {  // black to go
    for (const auto &[i, j] : dk)
      if (kx + i == wx && ky + j == wy) return res = -1;
    bool flg = false;
    for (const auto &[i, j] : dk)
      flg = flg || ~state(!mv, kx + i, ky + j, wx, wy, nx, ny, bx, by);
    if (!flg) return res = 2 - ~state(!mv, kx, ky, wx, wy, nx, ny, bx, by);
  } else {  // white to go
    for (const auto &[i, j] : dk)
      if (wx + i == kx && wy + j == ky) return res = -1;
    for (const auto &[i, j] : dn)
      if (nx + i == kx && ny + j == ky) return res = -1;
    for (const auto &db : db4)
      for (const auto &[i, j] : db) {
        if (bx + i < 0 || bx + i > 7 || by + j < 0 || by + j > 7) break;
        if (bx + i == kx && by + j == ky) return res = -1;
        if (bx + i == wx && by + j == wy || bx + i == nx && by + j == ny) break;
      }
    if (kx == nx && ky == ny || kx == bx && ky == by) return res = 1;
  }
  return res = 0;
}
ull prvmv(int idx) {
  ull res = 0;
  const auto &[mv, kx, ky, wx, wy, nx, ny, bx, by] = its(idx);
  if (!~state(mv, kx, ky, wx, wy, nx, ny, bx, by)) return 0;
  if (mv) {  // black to go
    for (int i = 0; i < 8; i++) {
      auto [dx, dy] = dk[i];
      if (!state(!mv, kx, ky, wx + dx, wy + dy, nx, ny, bx, by))
        res |= (ull)1 << i + 0;
    }
    for (int i = 0; i < 8; i++) {
      auto [dx, dy] = dn[i];
      if (!state(!mv, kx, ky, wx, wy, nx + dx, ny + dy, bx, by))
        res |= (ull)1 << i + 8;
    }
    for (int i = 1; bx + i < 8 && by + i < 8; i++) {
      if (bx + i == wx && by + i == wy || bx + i == nx && by + i == ny) break;
      if (!state(!mv, kx, ky, wx, wy, nx, ny, bx + i, by + i))
        res |= (ull)1 << i + 16;
    }
    for (int i = 1; bx + i < 8 && by - i >= 0; i++) {
      if (bx + i == wx && by - i == wy || bx + i == nx && by - i == ny) break;
      if (!state(!mv, kx, ky, wx, wy, nx, ny, bx + i, by - i))
        res |= (ull)1 << i + 24;
    }
    for (int i = 1; bx - i >= 0 && by + i < 8; i++) {
      if (bx - i == wx && by + i == wy || bx - i == nx && by + i == ny) break;
      if (!state(!mv, kx, ky, wx, wy, nx, ny, bx - i, by + i))
        res |= (ull)1 << i + 32;
    }
    for (int i = 1; bx - i >= 0 && by - i >= 0; i++) {
      if (bx - i == wx && by - i == wy || bx - i == nx && by - i == ny) break;
      if (!state(!mv, kx, ky, wx, wy, nx, ny, bx - i, by - i))
        res |= (ull)1 << i + 40;
    }
  } else {  // white to go
    for (int i = 0; i < 8; i++) {
      auto [dx, dy] = dk[i];
      if (!state(!mv, kx + dx, ky + dy, wx, wy, nx, ny, bx, by))
        res |= (ull)1 << i + 48;
    }
  }
  return res;
}
vector<int> det;
void init() {
  det.resize(56);
  for (int i = 0; i < 8; i++) {
    auto [dx, dy] = dk[i];
    det[i + 0] = (-1 << 24) + (dx << 15) + (dy << 12);
  }
  for (int i = 0; i < 8; i++) {
    auto [dx, dy] = dn[i];
    det[i + 8] = (-1 << 24) + (dx << 9) + (dy << 6);
  }
  for (int i = 0; i < 8; i++) {
    int dx = i, dy = i;
    det[i + 16] = (-1 << 24) + (dx << 3) + (dy << 0);
  }
  for (int i = 0; i < 8; i++) {
    int dx = i, dy = -i;
    det[i + 24] = (-1 << 24) + (dx << 3) + (dy << 0);
  }
  for (int i = 0; i < 8; i++) {
    int dx = -i, dy = i;
    det[i + 32] = (-1 << 24) + (dx << 3) + (dy << 0);
  }
  for (int i = 0; i < 8; i++) {
    int dx = -i, dy = -i;
    det[i + 40] = (-1 << 24) + (dx << 3) + (dy << 0);
  }
  for (int i = 0; i < 8; i++) {
    auto [dx, dy] = dk[i];
    det[i + 48] = (1 << 24) + (dx << 21) + (dy << 18);
  }
  // for (auto v : det) cout << v << ' ';
  // cout << el;
}
vector<int> solve(int n) {
  vector<int> a(n, inf), d(n, 0);
  vector<ull> e(n);
  for (int u = 0; u < n; u++) {
    e[u] = prvmv(u);
    for (int i = 0; i < 56; i++)
      if (e[u] >> i & 1) d[u + det[i]]++;
  }
  queue<int> que;
  for (int i = n / 2; i < n; i++) {
    const auto &[mv, kx, ky, wx, wy, nx, ny, bx, by] = its(i);
    if (state(mv, kx, ky, wx, wy, nx, ny, bx, by) == 2) {
      a[i] = 0;
      que.push(i);
    }
  }
  while (que.size()) {
    int u = que.front(), v;
    que.pop();
    if (u >> 24) {
      for (int i = 0; i < 56; i++)
        if (e[u] >> i & 1) {
          int v = u + det[i];
          if (a[v] > a[u] + 1) a[v] = a[u] + 1, que.push(v);
        }
    } else {
      for (int i = 0; i < 56; i++)
        if (e[u] >> i & 1) {
          int v = u + det[i];
          if (!--d[v]) a[v] = a[u], que.push(v);
        }
    }
  }
  return a;
}
int read() {
  string k, w, n, b;
  cin >> k >> w >> n >> b;
  return k[0] - 'a' << 21 | k[1] - '1' << 18 | w[0] - 'a' << 15 |
         w[1] - '1' << 12 | n[0] - 'a' << 9 | n[1] - '1' << 6 |
         b[0] - 'a' << 3 | b[1] - '1';
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  init();
  auto a = solve(maxn);
  int tt;
  cin >> tt;
  while (tt--) {
    int n = read();
    auto res = a[n];
    if (res < inf)
      cout << "win" << el << res << el;
    else
      cout << "draw" << el;
  }
  return 0;
}