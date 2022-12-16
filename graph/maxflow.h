#pragma once

#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

namespace maxflow {

typedef long long flow_t;
typedef long long cost_t;
typedef std::pair<flow_t, cost_t> result_t;

const flow_t inf_flow = 1e18, inf_cost = 1e9;

struct FlowEdge {
  int from, to;
  cost_t cost;
  flow_t cap, low, flow;
  FlowEdge(int from, int to, cost_t cost, flow_t cap, flow_t low, flow_t flow)
      : from(from), to(to), cost(cost), cap(cap), low(low), flow(flow) {}
};

struct MaxFlow {
  struct Edge {
    int to;
    cost_t cost;
    flow_t cap;
    Edge(int to, cost_t cost, flow_t cap) : to(to), cost(cost), cap(cap) {}
  };
  int n;
  std::vector<std::vector<int>> ed;
  std::vector<Edge> val;
  void build(const std::vector<FlowEdge> &edge) {
    n = 0;
    for (const auto &e : edge) n = std::max({n, e.from, e.to});
    ed.assign(n + 1, {});
    val.clear();
    int m = 0;
    for (const auto &e : edge) {
      ed[e.from].push_back(m++);
      val.push_back(Edge(e.to, e.cost, e.cap - e.flow));
      ed[e.to].push_back(m++);
      val.push_back(Edge(e.from, e.cost, e.flow));
    }
    vis.assign(n + 1, false);
  }
  std::vector<cost_t> dis;
  std::vector<int> cur;
  std::vector<int> vis;
  bool bfs(int s, int t, bool cost) {
    for (int i = 0; i < val.size(); i += 2)
      // cout << val[i ^ 1].to << ' ' << val[i].to << ' ' << val[i].cap << ' '
      //      << val[i].cost << endl;
      if (s > n || t > n) return false;
    dis.assign(n + 1, inf_cost);
    dis[s] = 0;
    std::queue<int> que;
    que.push(s);
    while (!que.empty()) {
      int u = que.front();
      que.pop();
      for (auto i : ed[u]) {
        auto e = val[i];
        if (e.cap && dis[e.to] > dis[u] + (cost ? e.cost : 1)) {
          // cout << u << ' ' << e.to << endl;
          dis[e.to] = dis[u] + (cost ? e.cost : 1);
          que.push(e.to);
        }
      }
    }
    // for (int i = 1; i <= n; i++) cout << dis[i] << ' ';
    // cout << endl;
    getchar();
    cur.assign(n + 1, 0);
    return dis[t] < inf_cost;
  }
  std::pair<flow_t, cost_t> dfs(int u, int t, flow_t limit, bool cost) {
    // cout << u << ' ' << t << ' ' << limit << endl;
    if (u == t) return {limit, 0};
    vis[u] = true;
    flow_t flow = 0;
    cost_t totc = 0;
    for (int i = 0; i < ed[u].size() && flow < limit; i++) {
      auto &e = val[ed[u][i]];
      if (!vis[e.to] && e.cap && dis[e.to] == dis[u] + (cost ? e.cost : 1)) {
        auto [detf, detc] = dfs(e.to, t, std::min(limit - flow, e.cap), cost);
        flow += detf;
        totc += detc;
        e.cap -= detf;
        val[ed[u][i] ^ 1].cap += detf;
      }
    }
    vis[u] = false;
    return {flow, totc};
  }
  std::vector<FlowEdge> to_edge() {
    std::vector<FlowEdge> res;
    for (int i = 0; i < val.size(); i += 2)
      res.push_back(FlowEdge(val[i ^ 1].to, val[i].to, val[i].cost,
                             val[i].cap + val[i ^ 1].cap, 0, val[i ^ 1].cap));
    return res;
  }
};

std::pair<flow_t, cost_t> maxflow(std::vector<FlowEdge> &edge, int s, int t,
                                  bool cost) {
  MaxFlow mf;
  mf.build(edge);
  flow_t flow = 0;
  cost_t totc = 0;
  while (mf.bfs(s, t, cost)) {
    auto [detf, detc] = mf.dfs(s, t, inf_flow, cost);
    flow += detf;
    totc += detc;
  }
  edge = mf.to_edge();
  return {flow, totc};
}

// st : Feasible Flow <true: from s to t; false: with no s and t>
// cost: <0: Normal; 1: Minimum Cost; -1: Reverse Minimum Cost> Flow
// flag: <1: Maximum; 0: Any; -1: Minimum> Feasible Flow
std::tuple<bool, flow_t, cost_t> feasiflow(std::vector<FlowEdge> &edge,
                                           std::vector<flow_t> excess, int s,
                                           int t, bool st, bool cost,
                                           int flag) {
  int n = 0, m = edge.size();
  flow_t flow = 0;
  cost_t totc = 0;
  for (const auto &e : edge) n = std::max({n, e.from, e.to});
  excess.resize(n + 1);
  for (auto &e : edge) {
    excess[e.from] -= e.low;
    excess[e.to] += e.low;
    totc += e.low * e.cost;
    e.cap -= e.low;
  }
  // for (int i = 0; i < excess.size(); i++) cout << excess[i] << ' ';
  // cout << endl;
  for (int u = 1; u <= n; u++) {
    if (excess[u] > 0) edge.push_back(FlowEdge(n + 1, u, 0, excess[u], 0, 0));
    if (excess[u] < 0) edge.push_back(FlowEdge(u, n + 2, 0, -excess[u], 0, 0));
  }
  if (st) edge.push_back(FlowEdge(t, s, 0, inf_flow, 0, 0));
  auto [detf, detc] = maxflow(edge, n + 1, n + 2, cost);
  flow += detf;
  totc += detc;
  if (st) edge.pop_back();
  bool feasible = true;
  while (edge.size() > m) {
    if (edge.back().flow != edge.back().cap) feasible = false;
    edge.pop_back();
  }
  if (!feasible) {
    for (auto &e : edge) {
      e.cap += e.low;
      e.flow = 0;
    }
    return {false, 0, 0};
  }
  if (flag == 1) {
    auto [detf, detc] = maxflow(edge, s, t, cost);
    flow += detf;
    totc += detc;
  }
  if (flag == -1) {
    for (auto &v : edge) v.cost = -v.cost;
    auto [detf, detc] = maxflow(edge, t, s, cost);
    flow -= detf;
    totc -= detc;
    for (auto &v : edge) v.cost = -v.cost;
  }
  for (auto &e : edge) {
    // cout << e.cap << ' ' << e.flow << ' ' << e.low << endl;
    e.cap += e.low;
    e.flow += e.low;
  }
  return {true, flow, cost ? totc : 0};
}

}  // namespace maxflow