#pragma once
#include "Vector3D.h"
class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);

	// Copy constructor.  
	Vertex(const Vertex& v);

	// Destructor
	~Vertex();

	void DehomogenizeVertex();

	// Getters/Setters
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);

	// Equality operator
	bool operator== (const Vertex& rhs) const;

	// Addition/Subtraction operators
	const Vertex operator+ (const Vertex& rhs) const;
	const Vector3D operator- (const Vertex& rhs) const;

private:
	float _x;
	float _y;
	float _z;
	float _w;
};

