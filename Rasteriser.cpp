#include "Rasteriser.h"
#include "Matrix.h"
#include "MD2Loader.h"
#include <math.h>
#include <initializer_list>

Rasteriser app;

bool Rasteriser::Initialise()
{
	// Initalise and load the model into a model instance
	if (!MD2Loader::LoadModel("cartman.md2", _model, &Model::AddPolygon, &Model::AddVertex))
	{
		return false;
	}

	// Set up directional and point lights
	_dLights.push_back(DirectionalLight(200, 0, 0));
	_pLights.push_back(PointLight(0, 200, 250, Vertex(20, 50, -50, 0.0f)));

	return true;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	_transform = Matrix::YRotationMatrix(_delta);
	GeneratePerspectiveMatrix(1, (float)bitmap.GetWidth() / (float)bitmap.GetHeight());
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
	_delta = _delta + 0.01f;
}

void Rasteriser::Render(Bitmap &bitmap)
{	
	// Model transformations
	if (_frameNo >= 600 && _frameNo < 1200)
	{
		_message.assign(L"Wireframe X rotation");
		_model.ApplyTransformToOriginalVertices(Matrix::XRotationMatrix(_delta));
	}
	else if (_frameNo >= 1200 && _frameNo < 1800)
	{
		_message.assign(L"Wireframe Z rotation");
		_model.ApplyTransformToOriginalVertices(Matrix::ZRotationMatrix(_delta));
	}
	else if (_frameNo >= 1800 && _frameNo < 2000)
	{
		if (_frameNo == 1800)
		{
			_delta = 0.0f;
		}

		_message.assign(L"Wireframe scaling");
		_model.ApplyTransformToOriginalVertices(Matrix::ScalingMatrix(_delta, _delta, _delta));
	}
	else if (_frameNo >= 2000 && _frameNo < 2600)
	{
		if (_frameNo == 1800)
		{
			_delta = 0.0f;
		}

		_message.assign(L"Wireframe translation");
		_model.ApplyTransformToOriginalVertices(Matrix::TranslationMatrix(_delta, _delta, _delta));
	}
	else
	{
		_message.assign(L"Wireframe Y rotation");
		_model.ApplyTransformToOriginalVertices(_transform);
	}

	if (_frameNo >= 2600)
	{
		_message.assign(L"Wireframe backface cull");
		_model.CalculateBackfaces(_camera.GetPosition());
	}

	if (_frameNo >= 3800)
	{
		_message.assign(L"Flat shading + ambient");
		_model.CalculateLightingAmbient(AmbientLight(100, 100, 100));
	}

	if (_frameNo >= 4400)
	{
		_message.assign(L"Flat shading + ambient + directional");
		_model.CalculateLightingDirectional(_dLights);
	}

	if (_frameNo >= 5000)
	{
		_message.assign(L"Flat shading + ambient + directional + point light");
		_model.CalculateLightingPoint(_pLights);
	}
	_model.ApplyTransformToTransformedVertices(_camera.GetCamMatrix());
	
	if (_frameNo >= 3200)
	{
		if (_frameNo < 3800)
		{
			_message.assign(L"Wireframe backface cull + z depth");
		}
		_model.Sort();
	}
	_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
	_model.DehomogenizeVertices();
	_model.ApplyTransformToTransformedVertices(_viewMatrix);

	// Clear the window to black
	bitmap.Clear(RGB(0, 0, 0));

	if (_frameNo < 3800)
	{
		DrawWireFrame(bitmap);
	}
	else
	{
		DrawSolidFlat(bitmap);
	}
	
	// Increment frame number
	_frameNo++;
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
	// Draws wire frame of all polygons in the model /w backface culling

	std::vector<Polygon3D> polygons = _model.GetPolygons();
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	int ind0, ind1, ind2;
	Vertex vert0, vert1, vert2;
	std::vector<Vertex> vertices;
	HDC hDC;

	hDC = bitmap.GetDC();
	
	DrawString(bitmap, (LPCTSTR)_message.c_str());
	
	vertices = _model.GetTransformedVertices();

	SelectObject(hDC, pen);

	for (std::vector<Polygon3D>::iterator it = polygons.begin(); it != polygons.end(); it++)
	{
		ind0 = it->GetIndex(0);
		ind1 = it->GetIndex(1);
		ind2 = it->GetIndex(2);

		vert0 = vertices[ind0];
		vert1 = vertices[ind1];
		vert2 = vertices[ind2];

		if (!it->IsMarkedForCulling())
		{
			MoveToEx(hDC, (int)vert0.GetX(), (int)vert0.GetY(), nullptr);
			LineTo(hDC, (int)vert1.GetX(), (int)vert1.GetY());
			LineTo(hDC, (int)vert2.GetX(), (int)vert2.GetY());
			LineTo(hDC, (int)vert0.GetX(), (int)vert0.GetY());
		}
	}
}

void Rasteriser::DrawSolidFlat(Bitmap &bitmap)
{
	// Draws and fills polygons with the appropriate colour that make up the model

	std::vector<Polygon3D> polygons = _model.GetPolygons();
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	int ind0, ind1, ind2;
	Vertex vert0, vert1, vert2;
	std::vector<Vertex> vertices;
	HDC hDC;

	hDC = bitmap.GetDC();

	DrawString(bitmap, (LPCTSTR)_message.c_str());

	vertices = _model.GetTransformedVertices();

	for (std::vector<Polygon3D>::iterator it = polygons.begin(); it != polygons.end(); it++)
	{
		HPEN pen = CreatePen(PS_SOLID, 1, it->GetColour());
		HBRUSH brush = CreateSolidBrush(it->GetColour());

		SelectObject(hDC, pen);
		SelectObject(hDC, brush);

		ind0 = it->GetIndex(0);
		ind1 = it->GetIndex(1);
		ind2 = it->GetIndex(2);

		vert0 = vertices[ind0];
		vert1 = vertices[ind1];
		vert2 = vertices[ind2];

		POINT points[3];

		points[0].x = (LONG)vert0.GetX();
		points[1].x = (LONG)vert1.GetX();
		points[2].x = (LONG)vert2.GetX();

		points[0].y = (LONG)vert0.GetY();
		points[1].y = (LONG)vert1.GetY();
		points[2].y = (LONG)vert2.GetY();

		if (!it->IsMarkedForCulling())
		{
			Polygon(hDC, points, 3);
		}

		// Delete pen and brush because of resource issues
		DeleteObject(pen);
		DeleteObject(brush);
	}
}

void Rasteriser::DrawString(Bitmap &bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}
