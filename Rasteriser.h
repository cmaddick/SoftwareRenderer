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
	Camera _camera = Camera(0.0f, 0.0f, 0.0f, Vertex(0.0f, 0.0f, 50.0f, 1.0f));
	Matrix _viewMatrix;
	Matrix _perspectiveMatrix;
	Matrix _transform;
	float _delta = 0.0f;
};

