#pragma once
#include "Vector3D.h"
class Polygon3D
{
public:
	// Constructors
	Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	Polygon3D(const Polygon3D &p);

	// Destructor
	~Polygon3D();

	// Getters/Setters
	int GetIndex(int i) const;
	void SetIndex(int index, const int value);
	bool IsMarkedForCulling() const;
	void MarkForCulling();
	void UnMarkForCulling();
	Vector3D GetNormal() const;
	void SetNormal(const Vector3D normal);

	// Operators
	Polygon3D& operator= (const Polygon3D &rhs);

private:
	int _indices[3];
	bool _markedForCulling = false;
	Vector3D _normal;
};

