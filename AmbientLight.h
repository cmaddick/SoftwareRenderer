#pragma once
class AmbientLight
{
public:
	AmbientLight(int r, int g, int b);
	~AmbientLight();

	int GetR() const;
	void SetR(const int r);
	int GetG() const;
	void SetG(const int g);
	int GetB() const;
	void SetB(const int b);

private:
	int _r;
	int _g;
	int _b;
};

