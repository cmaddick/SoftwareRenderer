#include "Vector3D.h"
#include <math.h>



Vector3D::Vector3D()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vector3D::~Vector3D()
{
}

float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}

float Vector3D::GetMagnitude()
{
	float result;

	result = pow(_x, 2.0) + pow(_y, 2.0) + pow(_z, 2.0);
	result = sqrt(result);

	return result;
}

float Vector3D::DotProduct(const Vector3D& v1, const Vector3D& v2)
{
	float result = 0.0f;

	result = v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY() + v1.GetZ() * v2.GetZ();

	return result;
}

Vector3D Vector3D::CrossProduct(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D result;

	result.SetX(v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY());
	result.SetY(v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ());
	result.SetZ(v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX());

	return result;
}
