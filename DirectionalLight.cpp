#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(int r, int g, int b)
{
	_r = r;
	_g = g;
	_b = b;
	_direction = Vector3D(10.0f, 5.0f, -5.0f);
}


DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::GetR() const
{
	return _r;
}

void DirectionalLight::SetR(const int r)
{
	_r = r;
}

int DirectionalLight::GetG() const
{
	return _g;
}

void DirectionalLight::SetG(const int g)
{
	_g = g;
}

int DirectionalLight::GetB() const
{
	return _b;
}

void DirectionalLight::SetB(const int b)
{
	_b = b;
}

Vector3D DirectionalLight::GetDirection() const
{
	return _direction;
}

void DirectionalLight::SetDirection(const Vector3D dir)
{
	_direction = dir;
}
