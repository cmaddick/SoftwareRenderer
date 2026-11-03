#include "Rasteriser.h"
#include "Matrix.h"
#include "MD2Loader.h"
#include <math.h>
#include <float.h>
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
		if (_frameNo == 2000)
		{
			_delta = 0.0f;
		}

		_message.assign(L"Wireframe XYZ simulateous translation");
		_model.ApplyTransformToOriginalVertices(Matrix::TranslationMatrix(_delta, _delta, _delta));
	}
	else
	{
		_message.assign(L"Wireframe Y rotation");
		_model.ApplyTransformToOriginalVertices(_transform);
	}

	// Backface culling
	if (_frameNo >= 2600)
	{
		_message.assign(L"Wireframe backface cull");
		_model.CalculateBackfaces(_camera.GetPosition());
	}

	// Ambient light
	if (_frameNo >= 3800)
	{
		_message.assign(L"Flat shading + ambient");
		_model.CalculateLightingAmbient(AmbientLight(100, 100, 100));
	}

	// Directional light
	if (_frameNo >= 4400)
	{
		_message.assign(L"Flat shading + ambient + directional");
		_model.CalculateLightingDirectional(_dLights);
	}

	// Point light
	if (_frameNo >= 5000)
	{
		_message.assign(L"Flat shading + ambient + directional + point light");
		_model.CalculateLightingPoint(_pLights);
	}
	_model.ApplyTransformToTransformedVertices(_camera.GetCamMatrix());
	
	// Z depth sort
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
	// The third row outputs w (always 1) rather than d * z, so after dehomogenising
	// each vertex's z holds 1 / camera-space z. Unlike camera-space z itself, 1 / z
	// interpolates linearly across the screen, which is what the depth buffer needs.
	_perspectiveMatrix = Matrix({ d / aspectRatio, 0, 0, 0, 0, d, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 });
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

	HGDIOBJ oldPen = SelectObject(hDC, pen);

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

	// Restore the original pen before deleting ours so it isn't leaked
	SelectObject(hDC, oldPen);
	DeleteObject(pen);
}

void Rasteriser::DrawSolidFlat(Bitmap &bitmap)
{
	// Draws and fills polygons with the appropriate colour that make up the model,
	// using a per-pixel depth buffer so overlapping polygons are always resolved
	// correctly, whatever order they are drawn in

	std::vector<Polygon3D> polygons = _model.GetPolygons();
	int ind0, ind1, ind2;
	std::vector<Vertex> vertices;

	DrawString(bitmap, (LPCTSTR)_message.c_str());

	// Make sure GDI has finished drawing before we write pixels directly
	GdiFlush();

	// Reset the depth buffer to "infinitely far away"
	_depthBuffer.assign(bitmap.GetWidth() * bitmap.GetHeight(), FLT_MAX);

	vertices = _model.GetTransformedVertices();

	for (std::vector<Polygon3D>::iterator it = polygons.begin(); it != polygons.end(); it++)
	{
		if (it->IsMarkedForCulling())
		{
			continue;
		}

		ind0 = it->GetIndex(0);
		ind1 = it->GetIndex(1);
		ind2 = it->GetIndex(2);

		FillTriangle(bitmap, vertices[ind0], vertices[ind1], vertices[ind2], it->GetColour());
	}
}

void Rasteriser::FillTriangle(Bitmap &bitmap, const Vertex &v0, const Vertex &v1, const Vertex &v2, COLORREF colour)
{
	// Fills a triangle in screen space, writing only the pixels that are nearer
	// than what is already in the depth buffer. Vertex z holds 1 / camera-space z,
	// which gets smaller as polygons get nearer the camera.

	DWORD* pixels = bitmap.GetPixels();
	int width = (int)bitmap.GetWidth();
	int height = (int)bitmap.GetHeight();

	if (pixels == nullptr)
	{
		return;
	}

	// Twice the signed area of the triangle; zero means it has no area to fill
	float area = (v1.GetX() - v0.GetX()) * (v2.GetY() - v0.GetY()) - (v1.GetY() - v0.GetY()) * (v2.GetX() - v0.GetX());
	if (area == 0.0f)
	{
		return;
	}

	// Bounding box of the triangle, clipped to the bitmap
	int minX = max(0, (int)floorf(min(v0.GetX(), min(v1.GetX(), v2.GetX()))));
	int maxX = min(width - 1, (int)ceilf(max(v0.GetX(), max(v1.GetX(), v2.GetX()))));
	int minY = max(0, (int)floorf(min(v0.GetY(), min(v1.GetY(), v2.GetY()))));
	int maxY = min(height - 1, (int)ceilf(max(v0.GetY(), max(v1.GetY(), v2.GetY()))));

	// DIB pixels are 0x00RRGGBB, whereas a COLORREF is 0x00BBGGRR
	DWORD pixel = (GetRValue(colour) << 16) | (GetGValue(colour) << 8) | GetBValue(colour);

	for (int y = minY; y <= maxY; y++)
	{
		float py = y + 0.5f;

		for (int x = minX; x <= maxX; x++)
		{
			float px = x + 0.5f;

			// Barycentric weights of the pixel centre. Dividing by the signed area
			// makes them all non-negative inside the triangle, whatever its winding.
			float w0 = ((v2.GetX() - v1.GetX()) * (py - v1.GetY()) - (v2.GetY() - v1.GetY()) * (px - v1.GetX())) / area;
			float w1 = ((v0.GetX() - v2.GetX()) * (py - v2.GetY()) - (v0.GetY() - v2.GetY()) * (px - v2.GetX())) / area;
			float w2 = 1.0f - w0 - w1;

			if (w0 < 0.0f || w1 < 0.0f || w2 < 0.0f)
			{
				continue;
			}

			float depth = w0 * v0.GetZ() + w1 * v1.GetZ() + w2 * v2.GetZ();
			int index = y * width + x;

			if (depth < _depthBuffer[index])
			{
				_depthBuffer[index] = depth;
				pixels[index] = pixel;
			}
		}
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
