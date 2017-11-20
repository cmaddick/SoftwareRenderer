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
	_transform = Matrix::YRotationMatrix(1);
	GeneratePerspectiveMatrix(1, (float)bitmap.GetWidth() / bitmap.GetHeight());
	GenerateViewMatrix(1, bitmap.GetHeight(), bitmap.GetWidth());
}

void Rasteriser::Render(Bitmap &bitmap)
{	
	_model.ApplyTransformToOriginalVertices(_transform);
	_model.ApplyTransformToTransformedVertices(_camera.GetCamMatrix());
	_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
	_model.DehomogenizeVertices();
	_model.ApplyTransformToTransformedVertices(_viewMatrix);
	DrawWireFrame(bitmap);

	// Clear the window to black
	bitmap.Clear(RGB(0, 0, 0));
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_perspectiveMatrix = Matrix({ d / aspectRatio, 0, 0, 0, 0, d, 0, 0, 0, 0, d, 0, 0, 0, 1, 0 });
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	_viewMatrix = Matrix({(float)width / 2, 0, 0, (float)width / 2, 0, (float)-height / 2, 0, (float)height / 2, 0, 0, d / 2, d / 2, 0, 0, 0, 1});
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	std::vector<Polygon3D> polygons = _model.GetPolygons();
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	int ind0, ind1, ind2;
	Vertex vert0, vert1, vert2;
	std::vector<Vertex> vertices;
	HDC hDC;

	hDC = bitmap.GetDC();
	
	vertices = _model.GetVertices();

	SelectObject(hDC, pen);

	for (std::vector<Polygon3D>::iterator it = polygons.begin(); it != polygons.end(); it++)
	{
		ind0 = it->GetIndex(0);
		ind1 = it->GetIndex(1);
		ind2 = it->GetIndex(2);

		vert0 = vertices[ind0];
		vert1 = vertices[ind1];
		vert2 = vertices[ind2];

		MoveToEx(hDC, vert0.GetX(), vert0.GetY(), nullptr);
		LineTo(hDC, vert1.GetX(), vert1.GetY());
		LineTo(hDC, vert2.GetX(), vert2.GetY());
		LineTo(hDC, vert0.GetX(), vert0.GetY());
	}
}
