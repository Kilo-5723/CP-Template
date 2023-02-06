#pragma once

#include <vector>

#include "maxflow.h"

std::vector<std::pair<int, int>> bipartite_match(
    const std::vector<const std::pair<int, int>> &pairs) {
  int n = 1, m = 1;
  for (auto [l, r] : pairs) {
    n = max(n, l);
    m = max(m, r);
  }
  std::vector<maxflow::FlowEdge> edges;
  int s = n + m + 1, t = n + m + 2;
  for (int i = 1; i <= n; i++) edges.push_back({.from = s, .to = i, .cap = 1});
  for (int i = 1; i <= m; i++) edges.push_back({.from = i, .to = t, .cap = 1});
  for (auto [l, r] : pairs) edges.push_back({.from = l, .to = r, .cap = 1});
  maxflow::maximum_flow(edges, s, t);
  std::vector<std::pair<int, int>> res;
  for (auto e : edges)
    if (e.from != s && e.to != t && e.flow == 1) res.push_back({e.from, e.to});
  return res;
}