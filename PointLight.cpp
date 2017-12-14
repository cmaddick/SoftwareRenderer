#include "PointLight.h"



PointLight::PointLight(int r, int g, int b, Vertex& pos)
{
	_r = r;
	_g = g;
	_b = b;
}


PointLight::~PointLight()
{
}

int PointLight::GetR() const
{
	return _r;
}

void PointLight::SetR(const int r)
{
	_r = r;
}

int PointLight::GetG() const
{
	return _g;
}

void PointLight::SetG(const int g)
{
	_g = g;
}

int PointLight::GetB() const
{
	return _b;
}

void PointLight::SetB(const int b)
{
	_b = b;
}

Vertex PointLight::GetPosition() const
{
	return _position;
}

void PointLight::SetPosition(const Vertex pos)
{
	_position = pos;
}
