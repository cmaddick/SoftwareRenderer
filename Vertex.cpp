#include "Vertex.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;
}

Vertex::Vertex(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

// Copy constructor implementation
// Note that the parameter v is passed by reference, rather than value (that is what
// '&' indicates).  Note also the use of 'const' to indicate that the parameter v cannot
// be changed.

Vertex::Vertex(const Vertex& v)
{
	*this = v;
}

// Destructor does not need to do anything

Vertex::~Vertex()
{
}

float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}


float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

void Vertex::DehomogenizeVertex()
{
	_x = _x / _w;
	_y = _y / _w;
	_z = _z / _w;
	_w = _w / _w;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
		_w = rhs.GetW();
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ() && _w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ(), _w + rhs.GetW());
}

const Vector3D Vertex::operator- (const Vertex& rhs) const
{
	Vector3D result;

	result.SetX(_x - rhs.GetX());
	result.SetY(_y - rhs.GetY());
	result.SetZ(_z - rhs.GetZ());

	return result;
}
