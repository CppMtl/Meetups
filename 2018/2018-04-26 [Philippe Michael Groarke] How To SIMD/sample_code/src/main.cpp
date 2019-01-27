#include "text.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>

#include <bench_util/bench_util.h> // Timers
#include <emmintrin.h> // SSE2

/**
 * Intel doesn't have multiply instructions for 8bit SIMD vector.
 */
inline __m128i _mm_mullo_epi8(__m128i a, __m128i b) {
	/* Unpack and multiply. */

	/**
	 * Take a = [1, 2]
	 * Take b = [2, 2]
	 *
	 *     1    0
	 * |00000001|00000010| a as 16bit = 18
	 * |00000010|00000010| * b as 16bit = 34
	 * |00000110|00000100| dst_even = 612
	 *
	 */
	__m128i dst_even = _mm_mullo_epi16(a, b);

	/**
	 *     1    0
	 * |00000001|00000010| a as 16bit = 18
	 * |00000000|00000001| a >> 8 = 1
	 * *
	 * |00000000|00000010| b >> 8 = 2
	 * =
	 * |00000000|00000010| dst_odd = 2
	 */
	__m128i dst_odd
			= _mm_mullo_epi16(_mm_srli_epi16(a, 8), _mm_srli_epi16(b, 8));

	/* Repack. */

	/**
	 *     1    0
	 * |00000000|00000010| dst_odd = 2
	 * |00000010|00000000| dst_odd << 8
	 *
	 * |00000110|00000100| dst_even = 612
	 * |00000100|00000000| dst_even << 8 = 64
	 * |00000000|00000100| dst_even >> 8 = 4
	 *
	 * |00000010|00000000| dst_odd
	 * |00000000|00000100| + dst_even
	 * |00000010|00000100| = [2, 4]
	 */
	return _mm_or_si128(_mm_slli_epi16(dst_odd, 8),
			_mm_srli_epi16(_mm_slli_epi16(dst_even, 8), 8));
}

template <size_t N>
void simd_replace(char (&data)[N], char original, char replacement) {
	// Load the searched for character.
	const __m128i xmm_search_char = _mm_set1_epi8(original);
	// ASCII difference of characters.
	const __m128i xmm_diff = _mm_set1_epi8(replacement - original);
	// Mask upper 7bits.
	const __m128i xmm_mask_7 = _mm_set1_epi8(0x01);

	size_t i;
	for (i = 0; i < N / 16 * 16; i += 16) {
		// Load unaligned.
		const __m128i xmm_str = _mm_loadu_si128((const __m128i*)(&data[i]));
		// Find char.
		__m128i xmm_result = _mm_cmpeq_epi8(xmm_str, xmm_search_char);
		// Mask all upper 7bits to get 0x01.
		xmm_result = _mm_and_si128(xmm_result, xmm_mask_7);
		// Multiply 1 by character difference.
		xmm_result = _mm_mullo_epi8(xmm_result, xmm_diff);
		// Add the character difference.
		xmm_result = _mm_add_epi8(xmm_result, xmm_str);
		// Store the results in string.
		_mm_storeu_si128((__m128i*)(&data[i]), xmm_result);
	}

	if (N - i == 0)
		return;

	/**
	 * Final pass with remainder data that doesn't fill 128bits.
	 *
	 * Don't store the full data as we will write in unrelated memory.
	 * Store modified data in appropriately sized array, then only
	 * extract the valid elements.
	 **/
	char temp_storage[16] = {};
	for (size_t j = 0; j < N - i; ++j) {
		temp_storage[j] = data[i + j];
	}

	const __m128i xmm_str = _mm_loadu_si128((const __m128i*)(&temp_storage[0]));
	__m128i xmm_result = _mm_cmpeq_epi8(xmm_str, xmm_search_char);
	xmm_result = _mm_and_si128(xmm_result, xmm_mask_7);
	xmm_result = _mm_mullo_epi8(xmm_result, xmm_diff);
	xmm_result = _mm_add_epi8(xmm_result, xmm_str);

	_mm_storeu_si128((__m128i*)(&temp_storage), xmm_result);
	for (size_t j = 0; j < N - i; ++j) {
		data[i + j] = temp_storage[j];
	}
}

template <size_t N>
void boring_replace(char (&data)[N], char original, char replacement) {
	for (size_t i = 0; i < N; ++i) {
		if (data[i] == original)
			data[i] = replacement;
	}
}

// Output files for diff.
void do_diffs() {
	{
		std::replace(std::begin(text), std::end(text), 'e', '1');
		std::ofstream f{ "std_replace.txt" };
		if (f.is_open()) {
			f << text << std::endl;
		}
		std::replace(std::begin(text), std::end(text), '1', 'e');
	}

	{
		simd_replace(text, 'e', '1');
		std::ofstream f{ "simd_replace.txt" };
		if (f.is_open()) {
			f << text << std::endl;
		}
		simd_replace(text, '1', 'e');
	}
}

void do_tests_bigdata() {
	using namespace std::chrono_literals;
	const size_t num_benchmarks = 200'000;
	fprintf(stderr, "\n");
	bench::title("Big data : First 2 chapters of Moby Dick", stderr);

	// Trivial replace function.
	bench::start();
	for (size_t i = 0; i < num_benchmarks; ++i) {
		boring_replace(text, 'e', '1');
		boring_replace(text, '1', 'e');
	}
	bench::stop("boring replace", stderr);
	printf("%s\n", text);

	// std::replace
	bench::start();
	for (size_t i = 0; i < num_benchmarks; ++i) {
		std::replace(std::begin(text), std::end(text), 'e', '1');
		std::replace(std::begin(text), std::end(text), '1', 'e');
	}
	bench::stop("std::replace", stderr);
	printf("%s\n", text);

	fprintf(stderr, "\ndrum roll...\n");
	std::this_thread::sleep_for(4s);
	fprintf(stderr, "just  kidding :P\n\n");

	// SIMD replace
	bench::start();
	for (size_t i = 0; i < num_benchmarks; ++i) {
		simd_replace(text, '1', 'e');
		simd_replace(text, 'e', '1');
	}
	bench::stop("simd replace", stderr);
	printf("%s\n", text);
}

void do_tests_smalldata() {
	const size_t num_benchmarks = 100'000'000;
	fprintf(stderr, "\n");
	bench::title("Small data : 15 character string", stderr);

	// std::replace
	bench::start();
	for (size_t i = 0; i < num_benchmarks; ++i) {
		std::replace(std::begin(text_small), std::end(text_small), 'e', '1');
		std::replace(std::begin(text_small), std::end(text_small), '1', 'e');
	}
	bench::stop("std::replace", stderr);
	printf("%s\n", text_small);

	// SIMD replace
	bench::start();
	for (size_t i = 0; i < num_benchmarks; ++i) {
		simd_replace(text_small, '1', 'e');
		simd_replace(text_small, 'e', '1');
	}
	bench::stop("simd replace", stderr);
	printf("%s\n", text_small);
}

int main(int, char**) {

	do_diffs();
	do_tests_bigdata();
	do_tests_smalldata();

	fprintf(stderr, "\n");
	return 0;
}
