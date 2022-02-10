#pragma once

#include <cmath>

template <typename T>
struct vector_2d
{
	T x, y;

	vector_2d() : x(0), y(0){}
	vector_2d(const T& x, const T& y) : x(x), y(y){}
	vector_2d(const vector_2d&) = default;
	~vector_2d() = default;

	vector_2d operator+(const vector_2d& rsh) const
	{
		return { x + rsh.x, y + rsh.y };
	}

	vector_2d operator-(const vector_2d& rsh) const 
	{
		return { x - rsh.x, y - rsh.y };
	}

	vector_2d operator*(const T& rsh) const
	{
		return { x * rsh, y * rsh };
	}

	vector_2d operator/(const T& rsh) const
	{
		return { x / rsh, y / rsh };
	}

	T operator*(const vector_2d& rsh) const
	{
		return  x * rsh.x + y * rsh.y;
	}

	vector_2d& rotate(const T& angle)
	{
		return  *this = get_rotated(angle);
	}

	[[nodiscard]] vector_2d get_rotated(const T& angle) const
	{
		T sin_const = sin(angle);
		T cos_const = cos(angle);
		
		return { cos_const * x - sin_const * y, sin_const * x + cos_const * y };
	}

	vector_2d& operator+=(const vector_2d& rsh)
	{
		x += rsh.x;
		y += rsh.y;
		return *this;
	}

	vector_2d& operator-=(const vector_2d& rsh)
	{
		x -= rsh.x;
		y -= rsh.y;
		return *this;
	}
	
	vector_2d& operator=(const vector_2d& rsh)
	{
		x = rsh.x;
		y = rsh.y;
		return *this;
	}

	[[nodiscard]] T length() const
	{
		return static_cast<T>(sqrt(x * x + y * y));
	}

	void normalize()
	{
		*this = get_normalized();
	}

	float cross_product(const vector_2d& rsh) const
	{
		return x * rsh.y - rsh.x * y;
	}

	[[nodiscard]] vector_2d get_normalized() const
	{
		return { *this / length() };
	}

	vector_2d get_orthogonal() const
	{
		return { -y, x };
	}

	float angle() const
	{
		return atan2(y, x);
	}
	
};
