#pragma once

#include <algorithm>
#include <optional>
#include <queue>
#include <vector>

namespace costflow {

typedef long long flow_t;
typedef long long cost_t;

const flow_t inf_flow = 1e18;
const cost_t inf_cost = 1e18;

struct CostEdge {
  int from, to;
  cost_t cost;
  flow_t cap, low = 0, flow = 0;
};

int num_node(const std::vector<CostEdge> &edges) {
  int n = 0;
  for (const auto &e : edges) n = std::max({n, e.from, e.to});
  return n;
}
std::pair<flow_t, cost_t> get_flow(const std::vector<CostEdge> &edges, int s) {
  flow_t flow = 0;
  cost_t cost = 0;
  for (const auto &e : edges) {
    if (e.from == s) flow += e.flow;
    cost += e.flow * e.cost;
  }
  return {flow, cost};
}

struct CostFlow {
  struct Edge {
    int from, to;
    cost_t cost;
    flow_t cap;
  };
  int n;
  std::vector<std::vector<int>> eid;
  std::vector<Edge> edge;
  void build(const std::vector<CostEdge> &edges) {
    n = num_node(edges);
    eid.assign(n + 1, {});
    edge.clear();
    int num_edges = 0;
    for (const auto &e : edges) {
      eid[e.from].push_back(num_edges++);
      edge.push_back({e.from, e.to, e.cost, e.cap - e.flow});
      eid[e.to].push_back(num_edges++);
      edge.push_back({e.to, e.from, -e.cost, e.flow});
    }
  }
  std::vector<cost_t> dis;
  std::vector<int> pre;
  bool spfa(int s, int t) {
    if (s > n || t > n) return false;
    dis.assign(n + 1, inf_cost);
    pre.assign(n + 1, 0);
    std::vector<bool> inque(n + 1);
    std::queue<int> que;
    dis[s] = 0;
    que.push(s);
    inque[s] = true;
    while (que.size()) {
      int u = que.front();
      // cerr << 'u' << ' ' << u << endl;
      que.pop();
      inque[u] = false;
      for (auto i : eid[u]) {
        const auto &e = edge[i];
        if (e.cap && dis[e.to] > dis[u] + e.cost) {
          dis[e.to] = dis[u] + e.cost;
          pre[e.to] = i;
          if (!inque[e.to]) {
            que.push(e.to);
            inque[e.to] = true;
          }
        }
      }
    }
    return dis[t] < inf_cost;
  }
  std::pair<flow_t, cost_t> maxflow(int s, int t) {
    flow_t flow = 0;
    cost_t cost = 0;
    while (spfa(s, t)) {
      flow_t detf = inf_flow;
      cost_t detc = 0;
      for (int u = t, i = pre[u]; u != s; u = edge[i].from, i = pre[u]) {
        detf = std::min(detf, edge[i].cap);
        detc += edge[i].cost;
      }
      for (int u = t, i = pre[u]; u != s; u = edge[i].from, i = pre[u]) {
        edge[i].cap -= detf;
        edge[i ^ 1].cap += detf;
      }
      flow += detf;
      cost += detf * detc;
    }
    return {flow, cost};
  }
  std::vector<CostEdge> to_edge() {
    std::vector<CostEdge> edges;
    for (int i = 0; i < edge.size(); i += 2)
      edges.push_back({
          .from = edge[i].from,
          .to = edge[i].to,
          .cost = edge[i].cost,
          .cap = edge[i].cap + edge[i ^ 1].cap,
          .flow = edge[i ^ 1].cap,
      });
    return edges;
  }
};

struct Processor {
  std::vector<bool> neg;
  std::vector<flow_t> low;
  std::vector<flow_t> excess;
  void init(std::vector<CostEdge> &edges) {
    int n = num_node(edges);
    neg.clear();
    neg.reserve(edges.size());
    low.clear();
    low.reserve(edges.size());
    excess.assign(n + 1, 0);
  }
  void rmv_low(std::vector<CostEdge> &edges) {
    for (auto &e : edges) {
      low.push_back(e.low);
      if (e.flow >= e.low) {
        e.flow -= e.low;
      } else {
        excess[e.from] -= e.low - e.flow;
        excess[e.to] += e.low - e.flow;
        e.flow = 0;
      }
      e.cap -= e.low;
      e.low = 0;
    }
  }
  void add_low(std::vector<CostEdge> &edges) {
    reverse(low.begin(), low.end());
    for (auto &e : edges) {
      e.low = low.back();
      e.flow += e.low;
      e.cap += e.low;
      low.pop_back();
    }
  }
  void rmv_neg(std::vector<CostEdge> &edges) {
    for (auto &e : edges) {
      neg.push_back(e.cost < 0);
      if (e.cost < 0) {
        excess[e.from] -= e.cap - e.flow;
        excess[e.to] += e.cap - e.flow;
        e.flow = e.cap;
      }
      if (e.cost > 0) {
        excess[e.from] += e.flow;
        excess[e.to] -= e.flow;
        e.flow = 0;
      }
    }
  }
};

bool excess_flow(std::vector<CostEdge> &edges,
                 const std::vector<flow_t> &excess) {
  int n = num_node(edges), m = edges.size();
  for (int i = 1; i <= n; i++) {
    if (excess[i] > 0)
      edges.push_back({.from = n + 1, .to = i, .cost = 0, .cap = excess[i]});
    if (excess[i] < 0)
      edges.push_back({.from = i, .to = n + 2, .cost = 0, .cap = -excess[i]});
  }
  CostFlow g;
  g.build(edges);
  g.maxflow(n + 1, n + 2);
  edges = g.to_edge();
  for (int i = m; i < edges.size(); i++)
    if (edges[i].flow != edges[i].cap) return false;
  edges.resize(m);
  return true;
}

std::optional<std::pair<flow_t, cost_t>> feasible_flow(
    std::vector<CostEdge> &edges, int s = 0, int t = 0) {
  if (s && t) edges.push_back({.from = t, .to = s, .cost = 0, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  p.rmv_neg(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  if (s && t) edges.pop_back();
  p.add_low(edges);
  return get_flow(edges, s);
}

std::optional<std::pair<flow_t, cost_t>> maximum_flow(
    std::vector<CostEdge> &edges, int s, int t) {
  edges.push_back({.from = t, .to = s, .cost = 0, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  p.rmv_neg(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  edges.pop_back();
  CostFlow g;
  g.build(edges);
  g.maxflow(s, t);
  edges = g.to_edge();
  p.add_low(edges);
  return get_flow(edges, s);
}

std::optional<std::pair<flow_t, cost_t>> minimum_flow(
    std::vector<CostEdge> &edges, int s, int t) {
  edges.push_back({.from = t, .to = s, .cost = 0, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  p.rmv_neg(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  edges.pop_back();
  CostFlow g;
  for (auto &e : edges) e.cost = -e.cost;
  Processor q;
  q.rmv_neg(edges);
  excess_flow(edges, q.excess);
  g.build(edges);
  g.maxflow(t, s);
  edges = g.to_edge();
  for (auto &e : edges) e.cost = -e.cost;
  p.add_low(edges);
  return get_flow(edges, s);
}

}  // namespace costflow