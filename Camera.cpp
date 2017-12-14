#include "Camera.h"
#include "Matrix.h"
#include <Math.h>



Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_position = position;

	Matrix posMatrix = Matrix::TranslationMatrix(-position.GetX(), -position.GetY(), -position.GetZ());
	Matrix xRotMat = Matrix({ 1, 0, 0, 0, 0, cos(xRotation), sin(xRotation), 0, 0, -sin(xRotation), cos(xRotation), 0, 0, 0, 0, 1 });
	Matrix yRotMat = Matrix({ cos(yRotation), 0, -sin(yRotation), 0, 0, 1, 0, 0, sin(yRotation), 0, cos(yRotation), 0, 0, 0, 0, 1 });
	Matrix zRotMat = Matrix({ cos(zRotation), sin(zRotation), 0, 0, -sin(zRotation), cos(zRotation), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
	
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
