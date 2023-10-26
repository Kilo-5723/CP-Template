#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
const int alpha = 26;
struct node {
  array<node *, alpha> ch;
  int cnt;
  node() : cnt(0) { ch.fill(NULL); }
};
void add(node *u, const string &s) {
  for (auto c : s) {
    c -= 'a';
    if (!u->ch[c]) u->ch[c] = new node();
    u = u->ch[c];
    u->cnt++;
  }
}
pair<node *, ll> run(node *u, const string &s) {
  ll res = 0;
  for (auto c : s) {
    c -= 'a';
    if (!u->ch[c]) return {NULL, res};
    u = u->ch[c];
    res += u->cnt;
  }
  return {u, res};
}