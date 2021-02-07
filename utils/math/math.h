#pragma once

namespace math {
	#define Assert( _exp ) ((void)0)

	typedef __declspec( align( 16 ) ) union {
		float f[ 4 ];
		__m128 v;
	} m128;

	inline __m128 sqrt_ps( const __m128 squared )
	{
		return _mm_sqrt_ps( squared );
	}

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	void  VectorAngles(const vec3_t& forward, qangle_t& angles, vec3_t* up = nullptr);
	void  AngleVectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);
	float GetFOV(const qangle_t& view_angles, const vec3_t& start, const vec3_t& end);

	float segment_to_segment(const vec3_t s1, const vec3_t s2, const vec3_t k1, const vec3_t k2);

	void ang_vec_mult(const vec3_t angles, vec3_t& forward, vec3_t& right, vec3_t& up);

	void AngleMatrix(const vec3_t& angles, matrix3x4_t& matrix);

	bool intersect_line_with_bb(vec3_t& start, vec3_t& end, vec3_t& min, vec3_t& max);

	void vector_i_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out);

	void vector_i_rotate(vec3_t in1, matrix3x4_t in2, vec3_t& out);

	vec3_t vector_rotate(vec3_t& in1, matrix3x4_t& in2);

	vec3_t vector_rotate(vec3_t& in1, vec3_t& in2);

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);

	void vector_angles(const vec3_t& forward, qangle_t& angles);

	float fast_asin(float x);

	float fast_atan2(const float y, const float x);

	float fast_atan(const float x);

	float fast_sin(float x);

	float fast_cos(const float x);

	float fast_acos(float x);

	void fast_rsqrt(float a, float* out);

	void vector_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out);

	void angle_vectors(const qangle_t& angles, vec3_t& forward);

	void clamp_angle(qangle_t& angles);

	void clamp_angles(vec3_t& angles);

	float get_fov(vec3_t view_angle, vec3_t aim_angle);

	void fix_angle(qangle_t& angles);

	vec3_t calculate_angle(vec3_t src, vec3_t dst);

	float product(const float* v1, const float* v2);

	void vec_transform(const float* in1, const matrix3x4_t& in2, float* out);

	void vec_transform_wrapper(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out);

	inline float fastsqrt(float x) {
		unsigned int i = *(unsigned int*)&x;
		i += 127 << 23;
		i >>= 1;
		return *(float*)&i;
	}


	void ang_vec(const vec3_t& angles, vec3_t& forward);
	
	float normalize_yaw(float yaw);

	float interpolate(const float from, const float to, const float percent);

	qangle_t interpolate(const qangle_t from, const qangle_t to, const float percent);

	void normalize_angle(float& angle);

	__forceinline float normalize_angles(float angle) {
			normalize_angle(angle);
		return angle;
	}
	
	void vec_angles(const vec3_t& forward, vec3_t& angles);

	template<class T>
	void normalize(T& vec)
	{
		for (auto i = 0; i < 2; i++) {

			if (i == 0)
			{
				while (vec.x < -180.0f) vec.x += 360.0f;
				while (vec.x > 180.0f) vec.x -= 360.0f;
			}
			else if (i == 1)
			{
				while (vec.y < -180.0f) vec.y += 360.0f;
				while (vec.y > 180.0f) vec.y -= 360.0f;
			}
			else if (i == 2)
			{
				while (vec.z < -180.0f) vec.z += 360.0f;
				while (vec.z > 180.0f) vec.z -= 360.0f;
			}
		}
		vec.z = 0.f;
	}
}