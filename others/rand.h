// ver 2.0
random_device rnd;
default_random_engine eng(rnd());
uniform_int_distribution<int> gen(0, 1e9);
// gen(eng)