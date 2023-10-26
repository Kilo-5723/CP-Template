#include <bits/stdc++.h>
using namespace std;

vector<int> kmp(string s) {
  vector<int> res = {-1};
  for (auto c : s) {
    int cur = res.back();
    while (~cur && c != s[cur]) cur = res[cur];
    res.push_back(cur + 1);
  }
  return res;
}