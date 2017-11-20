#include "Rasteriser.h"
#include "Matrix.h"
#include <math.h>
#include <initializer_list>

Rasteriser app;

bool Rasteriser::Initialise()
{
	return true;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	testMat = Matrix({(float)cos(20), -((float)sin(20)), 0, 0, (float)sin(20), (float)cos(20), 0, 0, 0, 0, 0, 1});
}

void Rasteriser::Render(Bitmap &bitmap)
{
	for (int i = 0; i < 4; i++)
	{
		square[i] = testMat * square[i];
	}
	
	// Clear the window to black
	bitmap.Clear(RGB(0, 0, 0));

	DrawSquare(square, bitmap.GetDC());
}

void Rasteriser::DrawSquare(Vertex square[4], HDC hDC)
{
	HBRUSH brush;
	RECT rect;
	
	brush = CreateSolidBrush(RGB(255, 0, 0));
	
	rect.left = square[0].GetX();
	rect.top = square[0].GetY();
	rect.right = square[3].GetX();
	rect.bottom = square[3].GetY();

	FillRect(hDC, &rect, brush);
}
