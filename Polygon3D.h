#pragma once
class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	Polygon3D(const Polygon3D &p);

	~Polygon3D();

	int GetIndex(int i) const;
	void SetIndex(int index, const int value);

	Polygon3D& operator= (const Polygon3D &rhs);

private:
	int _indices[3];
};

