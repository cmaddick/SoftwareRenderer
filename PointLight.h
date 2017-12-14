#pragma once
#include "Vertex.h"
class PointLight
{
public:
	PointLight(int r, int g, int b, Vertex& pos);
	~PointLight();

	// Getters/Setters
	int GetR() const;
	void SetR(const int r);
	int GetG() const;
	void SetG(const int g);
	int GetB() const;
	void SetB(const int b);
	Vertex GetPosition() const;
	void SetPosition(const Vertex pos);

private:
	// Colours
	int _r;
	int _g;
	int _b;

	// Direction
	Vertex _position;
};

