#ifndef TEMPLATE_UTILITY
#define TEMPLATE_UTILITY

#include <chrono>
#include <iostream>

template<typename T, typename U, typename V>
void process_data(T function, U& object, V& data) {
	auto start = std::chrono::high_resolution_clock::now();


	(object.*function)(data);

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	std::cout << "The process completed in "<< microseconds/1000 << " milliseconds " << std::endl;
}

#endif
