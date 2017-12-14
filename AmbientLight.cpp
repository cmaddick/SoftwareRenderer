#include "AmbientLight.h"



AmbientLight::AmbientLight(int r, int g, int b)
{
	_r = r;
	_g = g;
	_b = b;
}


AmbientLight::~AmbientLight()
{
}

int AmbientLight::GetR() const
{
	return _r;
}

void AmbientLight::SetR(const int r)
{
	_r = r;
}

int AmbientLight::GetG() const
{
	return _g;
}

void AmbientLight::SetG(const int g)
{
	_g = g;
}

int AmbientLight::GetB() const
{
	return _b;
}

void AmbientLight::SetB(const int b)
{
	_b = b;
}
