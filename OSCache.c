int main() {
	cache_misses();
	cache_delays();
	sequential_memory_access();
	random_memory_access();

	volatile_cache();
	atomic_cache();

	race_condition();

	return 0;
}
