#pragma once
#include <vector>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);
	void DrawSquare(Vertex square[4], HDC hDC);
	Vertex square[4] { Vertex(200, 200, 0), Vertex(250, 200, 0), Vertex(200, 150, 0), Vertex(250, 150, 0) };
	Vertex newSquare[4];
	Matrix testMat;
};

