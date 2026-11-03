#include "Camera.h"
#include "Matrix.h"
#include <Math.h>



Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_position = position;

	Matrix posMatrix = Matrix::TranslationMatrix(-position.GetX(), -position.GetY(), -position.GetZ());
	Matrix xRotMat = Matrix({ 1, 0, 0, 0, 0, cosf(xRotation), sinf(xRotation), 0, 0, -sinf(xRotation), cosf(xRotation), 0, 0, 0, 0, 1 });
	Matrix yRotMat = Matrix({ cosf(yRotation), 0, -sinf(yRotation), 0, 0, 1, 0, 0, sinf(yRotation), 0, cosf(yRotation), 0, 0, 0, 0, 1 });
	Matrix zRotMat = Matrix({ cosf(zRotation), sinf(zRotation), 0, 0, -sinf(zRotation), cosf(zRotation), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
	
	_camMatrix = xRotMat * yRotMat * zRotMat * posMatrix;
}


Camera::~Camera()
{
}

Matrix Camera::GetCamMatrix() const
{
	return _camMatrix;
}

Vertex Camera::GetPosition() const
{
	return _position;
}
