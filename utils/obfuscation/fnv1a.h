#pragma once
#include <array>

template<typename T, T value>
struct constant_holder_t {
	enum class e_value_holder : T {
		m_value = value
	};
};

#define CONSTANT(value) ((decltype(value))constant_holder_t<decltype(value), value>::e_value_holder::m_value)

constexpr auto seed = 0x45C3370D;
constexpr auto prime = 0x1000193;

__forceinline uint32_t fnv1a_fl_rt(const char* txt, uint32_t length) {
	auto hash = seed;

	for (int i = 0; i < length; i++) {
		hash ^= txt[i];
		hash *= prime;
	}

	return hash;
}

__forceinline uint32_t fnv1a_rt(const char* txt) { return fnv1a_fl_rt(txt, strlen(txt)); }

constexpr uint32_t fnv1a_ct(const char* txt, uint32_t value = seed) noexcept { return !*txt ? value : fnv1a_ct(txt + 1, static_cast<unsigned>(1ull * (value ^ static_cast<uint8_t>(*txt)) * prime)); }

#define FNV1A(txt) CONSTANT(fnv1a_ct(txt))
