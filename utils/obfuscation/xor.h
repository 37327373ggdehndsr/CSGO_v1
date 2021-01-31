#pragma once

template<int L, uint8_t K>
struct xor_str_t {
private:
	__forceinline uint8_t dec(uint8_t value) const { return value ^ K; }
	constexpr __forceinline uint8_t enc(uint8_t value) const { return value ^ K; }
public:
	template <int... S>
	constexpr __forceinline xor_str_t(const char* txt, std::index_sequence<S...>) { m_encrypted = { enc(txt[S])... }; }

	__forceinline std::string dec() const {
		auto ret = std::string(L, 0);

		for (int i = 0; i < L; i++) {
			ret.at(i) = dec(m_encrypted.at(i));
		}

		return ret;
	}

	__forceinline std::string ot(bool decrypt = true) {
		auto ret = std::string(L, 0);

		for (int i = 0; i < L; i++) {
			ret.at(i) = decrypt ? dec(m_encrypted.at(i)) : m_encrypted.at(i);

			m_encrypted.at(i) = 0;
		}

		return ret;
	}

	std::array<uint8_t, L> m_encrypted = {};
};

#define XOR_KEY ((FNV1A(__FILE__ __TIME__) + __LINE__) % (std::numeric_limits<uint8_t>::max)())

#define _(txt) xor_str_t<sizeof(txt), XOR_KEY>(txt, std::make_index_sequence<sizeof(txt)>()).dec().c_str()
#define _OT(txt) xor_str_t<sizeof(txt), XOR_KEY>(txt, std::make_index_sequence<sizeof(txt)>()).ot().c_str()

#define __(txt) []() -> std::pair<std::string, char> { \
	constexpr auto key = XOR_KEY; \
	return std::make_pair(xor_str_t<sizeof(txt), key>(txt, std::make_index_sequence<sizeof(txt)>()).ot(false), key); \
}()

#define _RT(n, txt) auto (n) = reinterpret_cast<char*>(alloca(((txt).first.size() + 1) * sizeof(char))); \
	for (int i = 0; i < (txt).first.size(); i++) \
        (n)[i] = (txt).first[i] ^ (txt).second; \
    (n)[(txt).first.size()] = 0