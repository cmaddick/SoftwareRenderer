#pragma once

#include "Vertex.h"
#include "Matrix.h"
class Camera
{
public:
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);
	~Camera();

	Matrix GetCamMatrix() const;
	Vertex GetPosition() const;

private:
	Matrix _camMatrix;
	Vertex _position;
};

