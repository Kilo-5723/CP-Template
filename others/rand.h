
namespace myrand {
random_device r;
default_random_engine e(r());
std::uniform_int_distribution<int> g(0, 1e9);
}  // namespace myrand