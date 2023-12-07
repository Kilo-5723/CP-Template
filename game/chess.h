#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef unsigned long long ull;
array<ull, 64> rook, bishop, knight, queen, arch, chan, maha;
void write(ull t, ull p) {
  for (int i = 0; i < 8; i++, cout << el)
    for (int j = 0; j < 8; j++) {
      int x = i << 3 | j;
      cout << (p >> x & 1 ? "o " : t >> x & 1 ? "  " : "x ");
    }
  cout << el;
}
void init() {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      auto &t = rook[i << 3 | j];
      t = 0;
      for (int x = 7; x >= 0; x--)
        for (int y = 7; y >= 0; y--) t = t << 1 | (x == i || y == j);
    }
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      auto &t = bishop[i << 3 | j];
      t = 0;
      for (int x = 7; x >= 0; x--)
        for (int y = 7; y >= 0; y--)
          t = t << 1 | (i + j == x + y || i - j == x - y);
    }
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      auto &t = knight[i << 3 | j];
      t = 0;
      for (int x = 7; x >= 0; x--)
        for (int y = 7; y >= 0; y--)
          t = t << 1 | ((x != i && y != j && abs(x - i) + abs(y - j) == 3) ||
                        x == i && y == j);
    }
  for (int i = 0; i < 64; i++) queen[i] = rook[i] | bishop[i];
  for (int i = 0; i < 64; i++) arch[i] = bishop[i] | knight[i];
  for (int i = 0; i < 64; i++) chan[i] = rook[i] | knight[i];
  for (int i = 0; i < 64; i++) maha[i] = queen[i] | knight[i];
}
array<ull, 64> piece(char ch) {
  switch (ch) {
    case 'R':
      return rook;
    case 'B':
      return bishop;
    case 'Q':
      return queen;
    case 'A':
      return arch;
    case 'C':
      return chan;
    case 'M':
      return maha;
  }
}
int cnt = 100;
bool win(string s, int p, ull safe, ull used) {
  if (p == s.size()) return false;
  bool res = false;
  const auto &pce = piece(s[p]);
  for (int i = 0; i < 64 && !res; i++)
    if ((safe >> i & 1) && !(used & pce[i]))
      res = res || !win(s, p + 1, safe & ~pce[i], used | (1ull << i));
  return res;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  init();
  string s;
  cin >> s;
  cout << (win(s, 0, -1ull, 0) ? "Alice" : "Bob") << el;
  return 0;
}
// https://contest.ucup.ac/contest/1399/problem/7635