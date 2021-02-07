#include "../utils.h"

namespace math {
	void sin_cos(float radian, float& sin, float& cos) {
		sin = std::sin(radian);
		cos = std::cos(radian);
	}

	float normalize_yaw(float yaw)
	{
		while (yaw < -180.f)
			yaw += 360.f;

		while (yaw > 180.f)
			yaw -= 360.f;

		return yaw;
	}

	void AngleVectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		float cp = std::cos(DEG2RAD(angles.x)), sp = std::sin(DEG2RAD(angles.x));
		float cy = std::cos(DEG2RAD(angles.y)), sy = std::sin(DEG2RAD(angles.y));
		float cr = std::cos(DEG2RAD(angles.z)), sr = std::sin(DEG2RAD(angles.z));

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = -1.f * sr * sp * cy + -1.f * cr * -sy;
			right->y = -1.f * sr * sp * sy + -1.f * cr * cy;
			right->z = -1.f * sr * cp;
		}

		if (up) {
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	float GetFOV(const qangle_t& view_angles, const vec3_t& start, const vec3_t& end) {
		vec3_t dir, fw;

		// get direction and normalize.
		dir = (end - start).normalized();

		// get the forward direction vector of the view angles.
		AngleVectors(view_angles, &fw);

		// get the angle between the view angles forward directional vector and the target location.
		return max(DEG2RAD(std::acos(fw.dot_product(dir))), 0.f);
	}



	void VectorAngles(const vec3_t& forward, qangle_t& angles, vec3_t* up) {
		vec3_t  left;
		float   len, up_z, pitch, yaw, roll;

		// get 2d length.
		len = forward.length_2d();

		if (up && len > 0.001f) {
			pitch = RAD2DEG(std::atan2(-forward.z, len));
			yaw = RAD2DEG(std::atan2(forward.y, forward.x));

			// get left direction vector using cross product.
			left = (*up).cross_product(forward).normalized();

			// calculate up_z.
			up_z = (left.y * forward.x) - (left.x * forward.y);

			// calculate roll.
			roll = RAD2DEG(std::atan2(left.z, up_z));
		}

		else {
			if (len > 0.f) {
				// calculate pitch and yaw.
				pitch = RAD2DEG(std::atan2(-forward.z, len));
				yaw = RAD2DEG(std::atan2(forward.y, forward.x));
				roll = 0.f;
			}

			else {
				pitch = (forward.z > 0.f) ? -90.f : 90.f;
				yaw = 0.f;
				roll = 0.f;
			}
		}

		// set out angles.
		angles = { pitch, yaw, roll };
	}

	qangle_t interpolate(const qangle_t from, const qangle_t to, const float percent) {
		return to * percent + from * (1.f - percent);
	}

	float interpolate(const float from, const float to, const float percent) {
		return to * percent + from * (1.f - percent);
	}

	void normalize_angle(float& angle) {
		float rot;

		// bad number.
		if (!std::isfinite(angle)) {
			angle = 0.f;
			return;
		}

		// no need to normalize this angle.
		if (angle >= -180.f && angle <= 180.f)
			return;

		// get amount of rotations needed.
		rot = std::round(std::abs(angle / 360.f));

		// normalize.
		angle = (angle < 0.f) ? angle + (360.f * rot) : angle - (360.f * rot);
	}



	void ang_vec(const vec3_t& angles, vec3_t& forward) {
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles.y));
		cy = cos(DEG2RAD(angles.y));

		sp = sin(DEG2RAD(angles.x));
		cp = cos(DEG2RAD(angles.x));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	void vec_angles(const vec3_t& forward, vec3_t& angles) {
		float tmp, yaw, pitch;

		if (forward.y == 0 && forward.x == 0) {
			yaw = 0;
			if (forward.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward.y, forward.x) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
			pitch = (atan2(-forward.z, tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles.x = pitch;
		angles.y = yaw;
		angles.z = 0;
	}

	void ang_vec_mult(const vec3_t angles, vec3_t& forward, vec3_t& right, vec3_t& up) {
		float angle;
		static float sp, sy, cp, cy;

		angle = angles.x * (M_PI / 180.f);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles.y * (M_PI / 180.f);
		sy = sin(angle);
		cy = cos(angle);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		static float sr, cr;

		angle = angles.z * (M_PI / 180.f);
		sr = sin(angle);
		cr = cos(angle);

		right.x = -1 * sr * sp * cy + -1 * cr * -sy;
		right.y = -1 * sr * sp * sy + -1 * cr * cy;
		right.z = -1 * sr * cp;

		up.x = cr * sp * cy + -sr * -sy;
		up.y = cr * sp * sy + -sr * cy;
		up.z = cr * cp;
	}

	void AngleMatrix(const vec3_t& angles, matrix3x4_t& matrix) {
		float sr, sp, sy, cr, cp, cy;

		sin_cos(DEG2RAD(angles.y), sy, cy);
		sin_cos(DEG2RAD(angles.x), sp, cp);
		sin_cos(DEG2RAD(angles.z), sr, cr);

		// matrix = (YAW * PITCH) * ROLL
		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}

	bool intersect_line_with_bb(vec3_t& start, vec3_t& end, vec3_t& min, vec3_t& max) {
		float d1, d2, f;
		auto start_solid = true;
		auto t1 = -1.0f, t2 = 1.0f;

		const float s[3] = { start.x, start.y, start.z };
		const float e[3] = { end.x, end.y, end.z };
		const float mi[3] = { min.x, min.y, min.z };
		const float ma[3] = { max.x, max.y, max.z };

		for (auto i = 0; i < 6; i++) {
			if (i >= 3) {
				const auto j = i - 3;

				d1 = s[j] - ma[j];
				d2 = d1 + e[j];
			}
			else {
				d1 = -s[i] + mi[i];
				d2 = d1 - e[i];
			}

			if (d1 > 0.0f && d2 > 0.0f)
				return false;

			if (d1 <= 0.0f && d2 <= 0.0f)
				continue;

			if (d1 > 0)
				start_solid = false;

			if (d1 > d2) {
				f = d1;
				if (f < 0.0f)
					f = 0.0f;

				f /= d1 - d2;
				if (f > t1)
					t1 = f;
			}
			else {
				f = d1 / (d1 - d2);
				if (f < t2)
					t2 = f;
			}
		}

		return start_solid || (t1 < t2&& t1 >= 0.0f);
	}

	void vector_i_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out) {
		out.x = (in1.x - in2[0][3]) * in2[0][0] + (in1.y - in2[1][3]) * in2[1][0] + (in1.z - in2[2][3]) * in2[2][0];
		out.y = (in1.x - in2[0][3]) * in2[0][1] + (in1.y - in2[1][3]) * in2[1][1] + (in1.z - in2[2][3]) * in2[2][1];
		out.z = (in1.x - in2[0][3]) * in2[0][2] + (in1.y - in2[1][3]) * in2[1][2] + (in1.z - in2[2][3]) * in2[2][2];
	}

	void vector_i_rotate(vec3_t in1, matrix3x4_t in2, vec3_t& out) {
		out.x = in1.x * in2[0][0] + in1.y * in2[1][0] + in1.z * in2[2][0];
		out.y = in1.x * in2[0][1] + in1.y * in2[1][1] + in1.z * in2[2][1];
		out.z = in1.x * in2[0][2] + in1.y * in2[1][2] + in1.z * in2[2][2];
	}

	vec3_t vector_rotate(vec3_t& in1, matrix3x4_t& in2) {
		return vec3_t(in1.dot_product(in2[0]), in1.dot_product(in2[1]), in1.dot_product(in2[2]));
	}

	vec3_t vector_rotate(vec3_t& in1, vec3_t& in2) {
		matrix3x4_t m;
		AngleMatrix(in2, m);
		return vector_rotate(in1, m);
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		vec3_t cos, sin;

		sin_cos(DEG2RAD(angles.x), sin.x, cos.x);
		sin_cos(DEG2RAD(angles.y), sin.y, cos.y);
		sin_cos(DEG2RAD(angles.z), sin.z, cos.z);

		if (forward) {
			forward->x = cos.x * cos.y;
			forward->y = cos.x * sin.y;
			forward->z = -sin.x;
		}

		if (right) {
			right->x = -sin.z * sin.x * cos.y + -cos.z * -sin.y;
			right->y = -sin.z * sin.x * sin.y + -cos.z * cos.y;
			right->z = -sin.z * cos.x;
		}

		if (up) {
			up->x = cos.z * sin.x * cos.y + -sin.z * -sin.y;
			up->y = cos.z * sin.x * sin.y + -sin.z * cos.y;
			up->z = cos.z * cos.x;
		}
	}

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst) {
		const auto delta = src - dst;
		if (delta.empty())
			return qangle_t();

		const auto length = delta.length();

		if (delta.z == 0.f && length == 0.f
			|| delta.y == 0.f && delta.x == 0.f)
			return qangle_t();

		auto angles = qangle_t(fast_asin(delta.z / length) * M_RADPI, fast_atan(delta.y / delta.x) * M_RADPI, 0.f);

		if (delta.x >= 0.f) {
			angles.y += 180.f;
		}

		return angles.normalized();
	}

	void vector_angles(const vec3_t& forward, qangle_t& angles) {
		auto yaw = 0.f, pitch = 0.f;

		if (forward.z == 0.f
			&& forward.x == 0.f) {
			yaw = 0.f;
			pitch = forward.z > 0.f ? 90.f : 270.f;
		}
		else {
			yaw = fast_atan2(forward.y, forward.x) * 180.f / M_PI;

			if (yaw < 0.f) {
				yaw += 360.f;
			}

			pitch = fast_atan2(-forward.z, forward.length_2d()) * 180.f / M_PI;

			if (pitch < 0.f) {
				pitch += 360.f;
			}
		}

		angles.x = clamp(remainderf(pitch, 360.f), -89.f, 89.f);
		angles.y = clamp(remainderf(yaw, 360.f), -180.f, 180.f);
		angles.z = 0.f;
	}

	float fast_asin(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = -0.0187293;
		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x) * ret;
		return float(ret - 2 * negate * ret);
	}

	float fast_atan2(const float y, const float x) {
		const auto xabs = fabs(x);
		const auto yabs = fabs(y);
		double t3 = xabs;
		double t1 = yabs;
		auto t0 = max(t3, t1);
		t1 = min(t3, t1);
		t3 = 1 / t0;
		t3 = t1 * t3;

		const auto t4 = t3 * t3;
		t0 = -0.013480470;
		t0 = t0 * t4 + 0.057477314;
		t0 = t0 * t4 - 0.121239071;
		t0 = t0 * t4 + 0.195635925;
		t0 = t0 * t4 - 0.332994597;
		t0 = t0 * t4 + 0.999995630;
		t3 = t0 * t3;

		t3 = (yabs > xabs) ? 1.570796327 - t3 : t3;
		t3 = (x < 0) ? 3.141592654 - t3 : t3;
		t3 = (y < 0) ? -t3 : t3;

		return float(t3);
	}

	float fast_atan(const float x) {
		return fast_atan2(x, float(1));
	}

	float fast_sin(float x) {
		x *= float(0.159155);
		x -= floor(x);
		const auto xx = x * x;
		auto y = -6.87897;
		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;
		return float(x * y);
	}

	float fast_cos(const float x) {
		return fast_sin(x + 1.5708f);
	}

	float fast_acos(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = float(-0.0187293);
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}

	void fast_rsqrt(float a, float* out) {
		const auto xx = _mm_load_ss(&a);
		auto xr = _mm_rsqrt_ss(xx);
		auto xt = _mm_mul_ss(xr, xr);
		xt = _mm_mul_ss(xt, xx);
		xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
		xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
		xr = _mm_mul_ss(xr, xt);
		_mm_store_ss(out, xr);
	}

	void vector_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out) {
		auto DotProduct = [](const vec3_t& a, const vec3_t& b) {
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		};

		out.x = DotProduct(in1, vec3_t(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out.y = DotProduct(in1, vec3_t(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out.z = DotProduct(in1, vec3_t(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}

	void angle_vectors(const qangle_t& angles, vec3_t& forward) {
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles.x));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles.y));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	void clamp_angle(qangle_t& angles) {
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}

	void clamp_angles(vec3_t& angles){
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}

	float get_fov(vec3_t view_angle, vec3_t aim_angle){
		const float MaxDegrees = 360.0f;
		vec3_t Delta(0, 0, 0);
		vec3_t Forward(0, 0, 0);
		vec3_t delta = aim_angle - view_angle;
		clamp_angles(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}

	void fix_angle(qangle_t& angles) {
		normalize(angles);
		clamp_angle(angles);
	}

	vec3_t calculate_angle(vec3_t src, vec3_t dst) {
		vec3_t angles;

		vec3_t delta = src - dst;
		float hyp = delta.length_2d();

		angles.y = std::atanf(delta.y / delta.x) * 57.2957795131f;
		angles.x = std::atanf(-delta.z / hyp) * -57.2957795131f;
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}

	float product(const float* v1, const float* v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	};

	void vec_transform(const float* in1, const matrix3x4_t& in2, float* out) {
		out[0] = product(in1, in2[0]) + in2[0][3];
		out[1] = product(in1, in2[1]) + in2[1][3];
		out[2] = product(in1, in2[2]) + in2[2][3];
	};

	void vec_transform_wrapper(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out) {
		vec_transform(&in1.x, in2, &out.x);
	};

	float segment_to_segment(const vec3_t s1, const vec3_t s2, const vec3_t k1, const vec3_t k2) {
		static auto constexpr epsilon = 0.00000001;

		auto u = s2 - s1;
		auto v = k2 - k1;
		const auto w = s1 - k1;

		const auto a = u.dot_product(u);
		const auto b = u.dot_product(v);
		const auto c = v.dot_product(v);
		const auto d = u.dot_product(w);
		const auto e = v.dot_product(w);
		const auto D = a * c - b * b;
		float sn, sd = D;
		float tn, td = D;

		if (D < epsilon) {
			sn = 0.0;
			sd = 1.0;
			tn = e;
			td = c;
		}
		else {
			sn = b * e - c * d;
			tn = a * e - b * d;

			if (sn < 0.0) {
				sn = 0.0;
				tn = e;
				td = c;
			}
			else if (sn > sd) {
				sn = sd;
				tn = e + b;
				td = c;
			}
		}

		if (tn < 0.0) {
			tn = 0.0;

			if (-d < 0.0)
				sn = 0.0;
			else if (-d > a)
				sn = sd;
			else {
				sn = -d;
				sd = a;
			}
		}
		else if (tn > td) {
			tn = td;

			if (-d + b < 0.0)
				sn = 0;
			else if (-d + b > a)
				sn = sd;
			else {
				sn = -d + b;
				sd = a;
			}
		}

		const float sc = abs(sn) < epsilon ? 0.0 : sn / sd;
		const float tc = abs(tn) < epsilon ? 0.0 : tn / td;

		m128 n;
		auto dp = w + u * sc - v * tc;
		n.f[0] = dp.dot_product(dp);
		const auto calc = sqrt_ps(n.v);
		return reinterpret_cast<const m128*>(&calc)->f[0];
	}
}