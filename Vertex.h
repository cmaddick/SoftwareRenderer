#pragma once
class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);

	// Copy constructor.  
	Vertex(const Vertex& v);

	// Destructer
	~Vertex();

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float y);
	float GetW() const;
	void SetW(const float w);

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

private:
	float _x;
	float _y;
	float _z;
	float _w;
};

