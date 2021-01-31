#pragma once

struct vec2_t {
	vec2_t() = default;
	vec2_t(float ix, float iy) { x = ix; y = iy; }

	float x = 0.f, y = 0.f;

	vec2_t operator+(const vec2_t& in) const { return vec2_t(x + in.x, y + in.y); }

	vec2_t operator-(const vec2_t& in) const { return vec2_t(x - in.x, y - in.y); }

	vec2_t operator+(float in) const { return vec2_t(x + in, y + in); }

	vec2_t operator-(float in) const { return vec2_t(x - in, y - in); }

	vec2_t operator/(float in) const { return vec2_t(x / in, y / in); }

	vec2_t operator*(float in) const { return vec2_t(x * in, y * in); }

	vec2_t operator-() const { return vec2_t(-x, -y); }

	vec2_t& operator-=(const vec2_t& in) {
		x -= in.x;
		y -= in.y;

		return *this;
	}

	vec2_t& operator+=(const vec2_t& in) {
		x += in.x;
		y += in.y;

		return *this;
	}

	vec2_t& operator/=(float in) {
		x /= in;
		y /= in;

		return *this;
	}

	vec2_t& operator*=(float in) {
		x *= in;
		y *= in;

		return *this;
	}

	void normalize() {
		const auto l = length();

		x /= l;
		y /= l;
	}

	vec2_t normalized() {
		auto ret = *this;

		ret.normalize();

		return ret;
	}

	bool operator==(const vec2_t& in) const { return x == in.x && y == in.y; }

	bool operator!=(const vec2_t& in) const { return !(operator==(in)); }

	float length_sqr() const { return x * x + y * y; }

	float length() const { return fast_sqrt(length_sqr()); }

	float dot_product(const vec2_t& in) const { return x * in.x + y * in.y; }

	float dot_product(const float* in) const { return x * in[0] + y * in[1]; }

	float dist_to(const vec2_t& in) const { return (*this - in).length(); }

	bool is_valid() const { return std::isfinite(x) && std::isfinite(y); }

	bool empty() const { return x == 0.f && y == 0.f; }
};

struct matrix3x4_t;

struct vec3_t {
	vec3_t() = default;
	vec3_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	float x = 0.f, y = 0.f, z = 0.f;

	vec3_t operator+(const vec3_t& in) const { return vec3_t(x + in.x, y + in.y, z + in.z); }

	vec3_t operator-(const vec3_t& in) const { return vec3_t(x - in.x, y - in.y, z - in.z); }

	vec3_t operator-(float in) const{ return vec3_t(x - in, y - in, z - in); }

	vec3_t operator+(float in) const { return vec3_t(x + in, y + in, z + in); }

	vec3_t operator/(float in) const { return vec3_t(x / in, y / in, z / in); }

	vec3_t operator*(float in) const { return vec3_t(x * in, y * in, z * in); }

	vec3_t operator-() const { return vec3_t(-x, -y, -z); }

	vec3_t& operator-=(const vec3_t& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;

		return *this;
	}

	vec3_t& operator+=(const vec3_t& in) {
		x += in.x;
		y += in.y;
		z += in.z;

		return *this;
	}

	vec3_t& operator/=(const vec3_t& in) {
		x /= in.x;
		y /= in.y;
		z /= in.z;

		return *this;
	}

	vec3_t& operator*=(const vec3_t& in) {
		x *= in.x;
		y *= in.y;
		z *= in.z;

		return *this;
	}

	vec3_t& operator/=(float in) {
		x /= in;
		y /= in;
		z /= in;

		return *this;
	}

	vec3_t& operator*=(float in) {
		x *= in;
		y *= in;
		z *= in;

		return *this;
	}

	void normalize() {
		const auto l = length();

		x /= l;
		y /= l;
		z /= l;
	}

	vec3_t normalized() {
		auto ret = *this;

		ret.normalize();

		return ret;
	}

	bool operator==(const vec3_t& in) const { return x == in.x && y == in.y && z == in.z; }

	bool operator!=(const vec3_t& in) const { return !(operator==(in)); }

	float length_sqr() const { return x * x + y * y + z * z; }

	float length() const { return fast_sqrt(length_sqr()); }

	float length_2d() const { return fast_sqrt(x * x + y * y); }

	float dot_product(const vec3_t& in) const { return x * in.x + y * in.y + z * in.z; }

	float dot_product(const float* in) const { return x * in[0] + y * in[1] + z * in[2]; }

	vec3_t cross_product(const vec3_t& in) const { return vec3_t(y * in.z - z * in.y, z * in.x - x * in.z, x * in.y - y * in.x); }

	vec3_t transform(const matrix3x4_t& in) const;

	vec3_t i_transform(const matrix3x4_t& in) const;

	vec3_t rotate(const matrix3x4_t& in) const;

	vec3_t i_rotate(const matrix3x4_t& in) const;

	float dist_to(const vec3_t& in) const { return (*this - in).length(); }

	float dist_to_2d(const vec3_t& in) const { return (*this - in).length_2d(); }

	bool is_valid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

	bool empty() const { return x == 0.f && y == 0.f && z == 0.f; }
};

struct ALIGN16 vector_aligned_t : public vec3_t {
	vector_aligned_t() = default;
	vector_aligned_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }
	vector_aligned_t(const vec3_t& in) { x = in.x; y = in.y; z = in.z; }

	vector_aligned_t& operator=(const vec3_t& in) {
		x = in.x; 
		y = in.y; 
		z = in.z;

		return *this;
	}

	vector_aligned_t& operator=(const vector_aligned_t& in)  {
		x = in.x;
		y = in.y;
		z = in.z;
		w = in.w;

		return *this;
	}

	float w;
};

struct qangle_t {
	qangle_t() = default;
	qangle_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	float x = 0.f, y = 0.f, z = 0.f;

	qangle_t operator+(const qangle_t& in) const { return qangle_t(x + in.x, y + in.y, z + in.z); }

	qangle_t operator-(const qangle_t& in) const { return qangle_t(x - in.x, y - in.y, z - in.z); }

	qangle_t operator-(float in) const { return qangle_t(x - in, y - in, z - in); }

	qangle_t operator+(float in) const { return qangle_t(x + in, y + in, z + in); }

	qangle_t operator/(float in) const { return qangle_t(x / in, y / in, z / in); }

	qangle_t operator*(float in) const { return qangle_t(x * in, y * in, z * in); }

	qangle_t& operator-=(const qangle_t& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;

		return *this;
	}

	qangle_t& operator+=(const qangle_t& in) {
		x += in.x;
		y += in.y;
		z += in.z;

		return *this;
	}

	qangle_t& operator/=(float in) {
		x /= in;
		y /= in;
		z /= in;

		return *this;
	}

	qangle_t& operator*=(float in) {
		x *= in;
		y *= in;
		z *= in;

		return *this;
	}

	bool operator==(const qangle_t& in) const { return x == in.x && y == in.y && z == in.z; }

	bool operator!=(const qangle_t& in) const { return !(operator==(in)); }

	void normalize();

	qangle_t normalized() {
		auto ret = *this;

		ret.normalize();

		return ret;
	}

	float length_sqr() const { return x * x + y * y + z * z; }

	float length() const { return fast_sqrt(length_sqr()); }

	float length_2d() const { return fast_sqrt(x * x + y * y); }

	bool is_valid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

	bool empty() const { return x == 0.f && y == 0.f && z == 0.f; }
};

struct col_t {
	col_t() = default;
	col_t(int r, int g, int b) { set(r, g, b, 255); }
	col_t(int r, int g, int b, int a) { set(r, g, b, a); }
	col_t(const col_t& col, int a) { set(col.r(), col.g(), col.b(), a); }
	col_t(int a) { set(clr[0], clr[1], clr[2], a); }

	std::array<uint8_t, 4> clr = {};

	void set(int r, int g, int b, int a) {
		clr.at(0) = static_cast<uint8_t>(r);
		clr.at(1) = static_cast<uint8_t>(g);
		clr.at(2) = static_cast<uint8_t>(b);
		clr.at(3) = static_cast<uint8_t>(a);
	}

	__forceinline int r() const { return clr.at(0); }

	__forceinline int g() const { return clr.at(1); }

	__forceinline int b() const { return clr.at(2); }

	__forceinline int a() const { return clr.at(3); }

	unsigned int hex(bool rgba = false) const { 
		return rgba
			? ((r() & 0xFF) << 24) + ((g() & 0xFF) << 16) + ((b() & 0xFF) << 8) + (a() & 0xFF)
			: ((a() & 0xFF) << 24) + ((b() & 0xFF) << 16) + ((g() & 0xFF) << 8) + (r() & 0xFF);
	}

	col_t& operator=(const col_t& in) {
		set(in.r(), in.g(), in.b(), in.a());

		return *this;
	}

	col_t& operator-=(uint8_t in) {
		set(r() - in, g() - in, b() - in, a() - in);

		return *this;
	}

	col_t& operator+=(uint8_t in) {
		set(r() + in, g() + in, b() + in, a() + in);

		return *this;
	}

	col_t& operator/=(uint8_t in) {
		set(r() / in, g() / in, b() / in, a() / in);

		return *this;
	}

	col_t& operator*=(uint8_t in) {
		set(r() * in, g() * in, b() * in, a() * in);

		return *this;
	}

	col_t& operator-=(const col_t& in) {
		set(r() - in.r(), g() - in.g(), b() - in.b(), a() - in.a());

		return *this;
	}

	col_t& operator+=(const col_t& in) {
		set(r() + in.r(), g() + in.g(), b() + in.b(), a() + in.a());

		return *this;
	}

	col_t operator-(uint8_t in) const { return col_t(r() - in, g() - in, b() - in, a() - in); }

	col_t operator+(uint8_t in) const { return col_t(r() + in, g() + in, b() + in, a() + in); }

	col_t operator/(uint8_t in) const { return col_t(r() / in, g() / in, b() / in, a() / in); }

	col_t operator*(uint8_t in) const { return col_t(r() * in, g() * in, b() * in, a() * in); }

	col_t operator-(const col_t& in) const { return col_t(r() - in.r(), g() - in.g(), b() - in.b(), a() - in.a()); }

	col_t operator+(const col_t& in) const { return col_t(r() + in.r(), g() + in.g(), b() + in.b(), a() + in.a()); }

	bool operator==(const col_t& in) const { return *const_cast<col_t*>(this) == *const_cast<col_t*>(&in); }

	bool operator!=(const col_t& in) const { return !(operator==(in)); }

		float hue() const {
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = max(red, max(green, blue));
		const auto min = min(red, min(green, blue));

		if (max == min)
			return 0.f;

		const auto delta = max - min;

		auto hue = 0.f;

		if (max == red) {
			hue = (green - blue) / delta;
		}
		else if (max == green) {
			hue = 2.f + (blue - red) / delta;
		}
		else {
			hue = 4.f + (red - green) / delta;
		}

		hue *= 60.f;

		if (hue < 0.f) {
			hue += 360.f;
		}

		return hue / 360.f;
	}

	float saturation() const {
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = max(red, max(green, blue));
		const auto min = min(red, min(green, blue));

		const auto delta = max - min;

		if (max == 0.f)
			return delta;

		return delta / max;
	}

	float brightness() const { return max(r() / 255.f, max(g() / 255.f, b() / 255.f)); }

	static col_t from_hsb(float hue, float saturation, float brightness) {
		const auto h = hue == 1.f ? 0 : hue * 6.f;
		const auto f = h - static_cast<int>(h);
		const auto p = brightness * (1.f - saturation);
		const auto q = brightness * (1.f - saturation * f);
		const auto t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f) {
			return col_t(
				static_cast<uint8_t>(brightness * 255.f),
				static_cast<uint8_t>(t * 255.f),
				static_cast<uint8_t>(p * 255.f)
			);
		}

		if (h < 2.f) {
			return col_t(
				static_cast<uint8_t>(q * 255),
				static_cast<uint8_t>(brightness * 255),
				static_cast<uint8_t>(p * 255)
			);
		}

		if (h < 3.f) {
			return col_t(
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(brightness * 255.f),
				static_cast<uint8_t>(t * 255.f)
			);
		}

		if (h < 4.f) {
			return col_t(
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(q * 255.f),
				static_cast<uint8_t>(brightness * 255.f)
			);
		}

		if (h < 5.f) {
			return col_t(
				static_cast<uint8_t>(t * 255.f),
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(brightness * 255.f)
			);
		}

		return col_t(
			static_cast<uint8_t>(brightness * 255.f),
			static_cast<uint8_t>(p * 255.f),
			static_cast<uint8_t>(q * 255.f)
		);
	}

	struct palette_t {
		static col_t red(int alpha = 255) { return col_t(255, 0, 0, alpha); }
		static col_t green(int alpha = 255) { return col_t(0, 255, 0, alpha); }
		static col_t blue(int alpha = 255) { return col_t(0, 0, 255, alpha); }

		static col_t white(int alpha = 255) { return col_t(255, 255, 255, alpha); }
		static col_t black(int alpha = 255) { return col_t(13, 13, 13, alpha); }
		static col_t grey(int alpha = 255) { return col_t(60, 60, 60, alpha); }
		static col_t orange(int alpha = 255) { return col_t(245, 171, 53, alpha); }

		static col_t light_black(int alpha = 255) { return col_t(32, 32, 32, alpha); }
		static col_t light_grey(int alpha = 255) { return col_t(77, 77, 77, alpha); }
		static col_t light_red(int alpha = 255) { return col_t(236, 98, 95, alpha); }
		static col_t light_blue(int alpha = 255) { return col_t(0, 102, 204, alpha); }
		static col_t light_violet(int alpha = 255) { return col_t(153, 153, 255, alpha); }

		static col_t dark_grey(int alpha = 255) { return col_t(44, 44, 44, alpha); }

		static col_t purple(int alpha = 255) { return col_t(220, 0, 220, alpha); }
		static col_t pink(int alpha = 255) { return col_t(255, 105, 180, alpha); }
	};
};

struct matrix3x4_t {
	matrix3x4_t() = default;
	matrix3x4_t(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin) { 
		m_matrix[0][0] = x_axis.x;
		m_matrix[0][1] = y_axis.x;
		m_matrix[0][2] = z_axis.x;
		m_matrix[0][3] = vec_origin.x;
		m_matrix[1][0] = x_axis.y;
		m_matrix[1][1] = y_axis.y;
		m_matrix[1][2] = z_axis.y;
		m_matrix[1][3] = vec_origin.y;
		m_matrix[2][0] = x_axis.z;
		m_matrix[2][1] = y_axis.z;
		m_matrix[2][2] = z_axis.z;
		m_matrix[2][3] = vec_origin.z;
	}

	void set_column(const vec3_t& in, int column) {
		m_matrix[0][column] = in.x;
		m_matrix[1][column] = in.y;
		m_matrix[2][column] = in.z;
	}

	float* operator[](int i) { return m_matrix[i]; }

	const float* operator[](int i) const { return m_matrix[i]; }

	float m_matrix[3][4] = {};
};

struct ALIGN16 matrix3x4a_t : public matrix3x4_t {

};

struct v_matrix {
	float* operator[](int i) { return m_matrix[i]; }

	const float* operator[](int i) const { return m_matrix[i]; }

	float m_matrix[4][4] = {};
};