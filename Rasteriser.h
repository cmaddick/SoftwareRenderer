#pragma once
#include <vector>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "Camera.h"

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

	void DrawWireFrame(Bitmap &bitmap);

private:
	Model _model;
	Camera _camera = Camera(0, 0, 0, Vertex(0, 0, -50, 0));
	Matrix _viewMatrix;
	Matrix _perspectiveMatrix;
	Matrix _transform;
};

