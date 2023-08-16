#pragma once

#include <vector>

#include "maxflow.h"

namespace match {

std::vector<std::pair<int, int>> max_match(
    const std::vector<std::pair<int, int>> &edges) {
  int n = 1, m = 1;
  for (auto [l, r] : edges) {
    n = std::max(n, l);
    m = std::max(m, r);
  }
  std::vector<maxflow::FlowEdge> fedge;
  int s = n + m + 1, t = n + m + 2;
  for (int i = 1; i <= n; i++) fedge.push_back({.from = s, .to = i, .cap = 1});
  for (int i = 1; i <= m; i++)
    fedge.push_back({.from = i + n, .to = t, .cap = 1});
  for (auto [l, r] : edges) fedge.push_back({.from = l, .to = r + n, .cap = 1});
  maxflow::maximum_flow(fedge, s, t);
  std::vector<std::pair<int, int>> res;
  for (auto e : fedge)
    if (e.from != s && e.to != t && e.flow == 1)
      res.push_back({e.from, e.to - n});
  return res;
}

void dfs(int u, std::vector<bool> &vl, std::vector<bool> &vr,
         std::vector<std::vector<int>> &ltr, std::vector<int> &rtl) {
  if (vl[u]) return;
  vl[u] = true;
  for (auto v : ltr[u]) {
    vr[v] = true;
    dfs(rtl[v], vl, vr, ltr, rtl);
  }
}

std::pair<std::vector<int>, std::vector<int>> min_cover(
    const std::vector<std::pair<int, int>> &edges) {
  int n = 1, m = 1;
  for (auto [l, r] : edges) {
    n = std::max(n, l);
    m = std::max(m, r);
  }
  auto match = max_match(edges);
  std::vector<std::vector<int>> ltr(n + 1);
  std::vector<int> rtl(m + 1);
  std::vector<bool> vis(n + 1);
  for (auto [l, r] : match) {
    rtl[r] = l;
    vis[l] = true;
  }
  for (auto [l, r] : edges) ltr[l].push_back(r);
  std::vector<bool> vl(n + 1), vr(m + 1);
  for (int i = 1; i <= n; i++)
    if (!vis[i]) dfs(i, vl, vr, ltr, rtl);
  std::pair<std::vector<int>, std::vector<int>> res;
  for (int i = 1; i <= n; i++)
    if (!vl[i]) res.first.push_back(i);
  for (int i = 1; i <= m; i++)
    if (vr[i]) res.second.push_back(i);
  return res;
}

}  // namespace match