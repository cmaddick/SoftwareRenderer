#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	~Vector3D();

	// Getters/Setters
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);

	// Vector magnitude
	float GetMagnitude();

	// Vector multiplication statics
	static float DotProduct(const Vector3D& v1, const Vector3D& v2);
	static Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2);

	Vector3D GetNormalisedVector();

private:
	float _x;
	float _y;
	float _z;
};

