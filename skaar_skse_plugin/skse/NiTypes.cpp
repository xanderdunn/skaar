#include "NiTypes.h"

NiPoint3::NiPoint3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

NiPoint3 NiPoint3::operator- () const
{
	return NiPoint3(-x, -y, -z);
}

NiPoint3 NiPoint3::operator+ (const NiPoint3& pt) const
{
	return NiPoint3(x + pt.x, y + pt.y, z + pt.z);
}

NiPoint3 NiPoint3::operator- (const NiPoint3& pt) const
{
	return NiPoint3(x - pt.x, y - pt.y, z - pt.z);
}

NiPoint3& NiPoint3::operator+= (const NiPoint3& pt)
{
	x += pt.x;
	y += pt.y;
	z += pt.z;
	return *this;
}
NiPoint3& NiPoint3::operator-= (const NiPoint3& pt)
{
	x -= pt.x;
	y -= pt.y;
	z -= pt.z;
	return *this;
}

// Scalar operations
NiPoint3 NiPoint3::operator* (float scalar) const
{
	return NiPoint3(scalar * x, scalar * y, scalar * z);
}
NiPoint3 NiPoint3::operator/ (float scalar) const
{
	float invScalar = 1.0f / scalar;
	return NiPoint3(invScalar * x, invScalar * y, invScalar * z);
}

NiPoint3& NiPoint3::operator*= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
NiPoint3& NiPoint3::operator/= (float scalar)
{
	float invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	z *= invScalar;
	return *this;
}

void NiMatrix33::Identity()
{
	data[0][0] = 1.0f;
	data[0][1] = 0.0f;
	data[0][2] = 0.0f;
	data[1][0] = 0.0f;
	data[1][1] = 1.0f;
	data[1][2] = 0.0f;
	data[2][0] = 0.0f;
	data[2][1] = 0.0f;
	data[2][2] = 1.0f;
}

NiMatrix33 NiMatrix33::operator* (const NiMatrix33& rhs) const
{
	NiMatrix33 tmp;
	tmp.data[0][0] =
		data[0][0]*rhs.data[0][0]+
		data[0][1]*rhs.data[1][0]+
		data[0][2]*rhs.data[2][0];
	tmp.data[1][0] =
		data[1][0]*rhs.data[0][0]+
		data[1][1]*rhs.data[1][0]+
		data[1][2]*rhs.data[2][0];
	tmp.data[2][0] =
		data[2][0]*rhs.data[0][0]+
		data[2][1]*rhs.data[1][0]+
		data[2][2]*rhs.data[2][0];
	tmp.data[0][1] =
		data[0][0]*rhs.data[0][1]+
		data[0][1]*rhs.data[1][1]+
		data[0][2]*rhs.data[2][1];
	tmp.data[1][1] =
		data[1][0]*rhs.data[0][1]+
		data[1][1]*rhs.data[1][1]+
		data[1][2]*rhs.data[2][1];
	tmp.data[2][1] =
		data[2][0]*rhs.data[0][1]+
		data[2][1]*rhs.data[1][1]+
		data[2][2]*rhs.data[2][1];
	tmp.data[0][2] =
		data[0][0]*rhs.data[0][2]+
		data[0][1]*rhs.data[1][2]+
		data[0][2]*rhs.data[2][2];
	tmp.data[1][2] =
		data[1][0]*rhs.data[0][2]+
		data[1][1]*rhs.data[1][2]+
		data[1][2]*rhs.data[2][2];
	tmp.data[2][2] =
		data[2][0]*rhs.data[0][2]+
		data[2][1]*rhs.data[1][2]+
		data[2][2]*rhs.data[2][2];
	return tmp;
}

NiMatrix33 NiMatrix33::operator* (float scalar) const
{
	NiMatrix33 result;
	result.data[0][0] = data[0][0] * scalar;
	result.data[0][1] = data[0][1] * scalar;
	result.data[0][2] = data[0][2] * scalar;
	result.data[1][0] = data[1][0] * scalar;
	result.data[1][1] = data[1][1] * scalar;
	result.data[1][2] = data[1][2] * scalar;
	result.data[2][0] = data[2][0] * scalar;
	result.data[2][1] = data[2][1] * scalar;
	result.data[2][2] = data[2][2] * scalar;
	return result;
}

NiPoint3 NiMatrix33::operator* (const NiPoint3& pt) const
{
	return NiPoint3
		(
		data[0][0]*pt.x+data[0][1]*pt.y+data[0][2]*pt.z,
		data[1][0]*pt.x+data[1][1]*pt.y+data[1][2]*pt.z,
		data[2][0]*pt.x+data[2][1]*pt.y+data[2][2]*pt.z
		);
}

NiTransform::NiTransform()
{
	rot.Identity();
	scale = 1.0f;
}

NiTransform NiTransform::operator*(const NiTransform &rhs) const
{
	NiTransform tmp;
	tmp.scale = scale * rhs.scale;
	tmp.rot = rot * rhs.rot;
	tmp.pos = pos + (rot * rhs.pos) * scale;
	return tmp;
}

NiPoint3 NiTransform::operator*(const NiPoint3 & pt) const
{
	return (((rot * pt) * scale) + pos);
}
