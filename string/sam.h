#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int alpha = 26;
struct sam {
  struct node {
    array<node *, alpha> next;
    node *link;
    int len;
    int cnt;
    node() {
      fill(next.begin(), next.end(), (node *)NULL);
      link = NULL;
      len = 0;
    }
  };
  node *nd, *end;
  node *nnode() { return new (end++) node(); }
  node *root, *last;
  sam(int n) {
    end = nd = (node *)new char[sizeof(node) * (n << 1)];
    last = root = nnode();
  }
  void add(int c) {
    node *cur = nnode();
    cur->len = last->len + 1;
    cur->cnt = 1;
    node *p = last;
    last = cur;
    while (p && !p->next[c]) {
      p->next[c] = cur;
      p = p->link;
    }
    if (!p) {
      cur->link = root;
      return;
    }
    node *q = p->next[c];
    if (q->len == p->len + 1) {
      cur->link = q;
      return;
    }
    node *clone = nnode();
    clone->next = q->next;
    clone->link = q->link;
    clone->len = p->len + 1;
    while (p && p->next[c] == q) {
      p->next[c] = clone;
      p = p->link;
    }
    cur->link = q->link = clone;
  }
  ll solve() {
    vector<int> deg(end - nd);
    for (auto u = nd + 1; u < end; u++) deg[u->link - nd]++;
    queue<node *> que;
    for (auto u = nd; u < end; u++)
      if (!deg[u - nd]) que.push(u);
    ll res = 0;
    while (que.size()) {
      auto u = que.front();
      que.pop();
      if (auto v = u->link) {
        v->cnt += u->cnt;
        if (!--deg[v - nd]) que.push(v);
      }
      if (u->cnt > 1) res = max(res, 1ll * u->cnt * u->len);
    }
    return res;
  }
};