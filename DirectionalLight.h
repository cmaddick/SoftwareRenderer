#pragma once
#include "Vector3D.h"

class DirectionalLight
{
public:
	DirectionalLight(int r, int g, int b);
	~DirectionalLight();

	int GetR() const;
	void SetR(const int r);
	int GetG() const;
	void SetG(const int g);
	int GetB() const;
	void SetB(const int b);
	Vector3D GetDirection() const;
	void SetDirection(Vector3D dir);

private:
	// Colours
	int _r;
	int _g;
	int _b;

	// Direction
	Vector3D _direction;
};

