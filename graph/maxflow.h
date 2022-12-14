#pragma once

#include <vector>

namespace maxflow {

typedef long long flow_t;
typedef long long cost_t;
typedef std::pair<flow_t, cost_t> result_t;

struct FlowEdge {
  int from, to;
  cost_t cost;
  flow_t cap, low, flow;
};

struct MaxFlow {};
struct CostFlow {};

result_t add(result_t a, result_t b) {
  return {a.first + b.first, a.second + b.second};
}

result_t maxflow(std::vector<FlowEdge> &edge, int s, int t, bool cap) {
  build();
  result_t res = {0, 0};
  while (cap ? spfa() : bfs()) res += dfs();
  return res;
}

// cap = true  : Minimum Cost Flow
//       false : Flow
// flg = 0  : Feasible Flow
//       1  : Maximum Feasible Flow
//       -1 : Minimum Feasible Flow

result_t feasiflow(std::vector<FlowEdge> &edge, int s, int t, bool cap,
                   int flg) {}

}  // namespace maxflow