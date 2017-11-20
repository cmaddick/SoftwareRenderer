#include "Rasteriser.h"
#include "Matrix.h"
#include "MD2Loader.h"
#include <math.h>
#include <initializer_list>

Rasteriser app;

bool Rasteriser::Initialise()
{
	if (!MD2Loader::LoadModel("cube.md2", _model, &Model::AddPolygon, &Model::AddVertex))
	{
		return false;
	}
	return true;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	
}

void Rasteriser::Render(Bitmap &bitmap)
{	
	// Clear the window to black
	bitmap.Clear(RGB(0, 0, 0));
}
