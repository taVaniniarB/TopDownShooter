#pragma once

// 자료구조 아니고 
// 힘, 크기, 방향 나타내는 벡터 성분으로서의 벡터
struct Vec2
{
	float x;
	float y;
public:
	bool isZero()
	{
		if (x == 0.f && y == 0.f)
			return true;
		return false;
	}

	float Length() // 벡터의 길이
	{
		return sqrt(x * x + y * y);
	}
	Vec2& Nomalize()
	{
		float fLen = Length();

		assert(fLen != 0.f);
		x /= fLen;
		y /= fLen;

		return *this;
	}

public:
	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}

public:
	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;

		return *this;
	}
	Vec2 operator + (Vec2 _vOther)
	{
		return Vec2( x + _vOther.x, y + _vOther.y );
	}
	void operator += (Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}
	void operator -= (Vec2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}
	void operator -= (float _f)
	{
		x -= _f;
		y -= _f;
	}
	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}
	Vec2 operator - (Vec2 _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}
	Vec2 operator * (Vec2 _vOther)
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}
	Vec2 operator / (Vec2 _vOther)
	{
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vec2(x / _vOther.x, y / _vOther.y);
	}
	Vec2 operator * (int _i)
	{
		return Vec2(x * (float)_i, y * (float)_i);
	}
	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}

	Vec2 operator / (float _f)
	{
		assert(! (0.f == _f) );
		return Vec2(x / _f, y / _f);
	}



public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}
	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(POINT _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}
};

//
//
//float VectorToAngle(Vec2 _vec)
//{
//	return atan2(_vec.y, _vec.x) * (180.0f / 3.141592f); // 라디안을 도 단위로 변환
//}