#include "Polygon3D.h"



Polygon3D::Polygon3D()
{
	// Initialse the polygon indices to 0
	for (int i = 0; i < 3; i++)
	{
		_indices[i] = 0;
	}
}

Polygon3D::Polygon3D(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
}

Polygon3D::Polygon3D(const Polygon3D &p)
{
	// Initialse the polygon indices to 0
	for (int i = 0; i < 3; i++)
	{
		_indices[i] = p.GetIndex(i);
	}
}

Polygon3D::~Polygon3D()
{
}

int Polygon3D::GetIndex(int i) const
{
	return _indices[i];
}

void Polygon3D::SetIndex(int index, const int value)
{
	_indices[index] = value;
}

Polygon3D& Polygon3D::operator=(const Polygon3D &rhs)
{
	for (int i = 0; i < 2; i++)
	{
		_indices[i] = rhs.GetIndex(i);
	}
}
