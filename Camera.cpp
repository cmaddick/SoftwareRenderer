#include "Camera.h"
#include "Matrix.h"



Camera::Camera(float xRotation, float yRotation, float zRotation, Vertex& position)
{
	Matrix posMatrix = Matrix::TranslationMatrix(-position.GetX(), -position.GetY(), -position.GetZ());
	_camMatrix = Matrix::XRotationMatrix(xRotation) * Matrix::YRotationMatrix(yRotation) * Matrix::ZRotationMatrix(zRotation) * posMatrix;
}


Camera::~Camera()
{
}

Matrix Camera::GetCamMatrix() const
{
	return _camMatrix;
}
