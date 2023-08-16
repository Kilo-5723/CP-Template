#include <optional>

namespace maxflow {

typedef long long flow_t;
const flow_t inf_flow = 1e18;
const int inf_dep = 1e9;

struct edge {
  int from, to;
  flow_t cap, low = 0, flow = 0;
  cost_t cost;
};

optional<flow_t> feasible_flow(vector<edge> &edges, int s, int t) {
  edges.push_back({.from = t, .to = s, .cap = inf_flow});
}

}  // namespace maxflow