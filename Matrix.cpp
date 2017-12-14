#include "Matrix.h"



Matrix::Matrix()
{
	// Initialse the matrix to 0.0
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			_matrix[i][j] = 0.0f;
		}
	}
}

Matrix::Matrix(const Matrix &other)
{
	// Copy constructor

	cpyMatrix(other);
}

Matrix::Matrix(std::initializer_list<float> list)
{
	const float* iterator = list.begin();
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			_matrix[i][j] = *iterator++;
		}
	}
}

Matrix::~Matrix()
{
}

float Matrix::GetM(const int row, const int column) const
{
	return _matrix[row][column];
}

void Matrix::SetM(const int row, const int column, const float value)
{
	_matrix[row][column] = value;
}

void Matrix::cpyMatrix(const Matrix &other)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			_matrix[i][j] = other.GetM(i, j);
		}
	}
}

Matrix& Matrix::operator= (const Matrix &rhs)
{
	if (this != &rhs)
	{
		cpyMatrix(rhs);
	}
	return *this;
}

const Matrix Matrix::operator*(const Matrix &other) const
{
	// Matrix by matrix multiplication
	Matrix tmpMat;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			float tmpRes = 0.0f;
			for (int k = 0; k < ROWS; k++)
			{
				tmpRes += _matrix[i][k] * other.GetM(k, j);
			}
			tmpMat.SetM(i, j, tmpRes);
		}
	}

	return tmpMat;
}

const Vertex Matrix::operator*(const Vertex &other) const
{
	// Matrix by vertex multiplication
	Vertex result;

	result.SetX(_matrix[0][0] * other.GetX() + _matrix[0][1] * other.GetY() + _matrix[0][2] * other.GetZ() + _matrix[0][3] * other.GetW());
	result.SetY(_matrix[1][0] * other.GetX() + _matrix[1][1] * other.GetY() + _matrix[1][2] * other.GetZ() + _matrix[1][3] * other.GetW());
	result.SetZ(_matrix[2][0] * other.GetX() + _matrix[2][1] * other.GetY() + _matrix[2][2] * other.GetZ() + _matrix[2][3] * other.GetW());
	result.SetW(_matrix[3][0] * other.GetX() + _matrix[3][1] * other.GetY() + _matrix[3][2] * other.GetZ() + _matrix[3][3] * other.GetW());

	return result;
}

Matrix Matrix::TranslationMatrix(float x, float y, float z)
{
	Matrix translation;

	translation = Matrix({1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1});

	return translation;
}

Matrix Matrix::ScalingMatrix(float x, float y, float z)
{
	Matrix scaling;

	scaling = Matrix({ x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 });

	return scaling;
}

Matrix Matrix::XRotationMatrix(float angle)
{
	Matrix rotation;

	rotation = Matrix({ 1, 0, 0, 0, 0, cos(angle), -(sin(angle)), 0, 0, sin(angle), cos(angle), 0, 0, 0, 0, 1 });

	return rotation;
}

Matrix Matrix::YRotationMatrix(float angle)
{
	Matrix rotation;

	rotation = Matrix({ cos(angle), 0, sin(angle), 0, 0, 1, 0, 0, -(sin(angle)), 0, cos(angle), 0, 0, 0, 0, 1 });

	return rotation;
}

Matrix Matrix::ZRotationMatrix(float angle)
{
	Matrix rotation;

	rotation = Matrix({ cos(angle), -(sin(angle)), 0, 0, sin(angle), cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });

	return rotation;
}

Matrix Matrix::XYZRotationMatrix(float angleX, float angleY, float angleZ)
{
	Matrix rotation;

	rotation = XRotationMatrix(angleX) * YRotationMatrix(angleY) * ZRotationMatrix(angleZ);

	return rotation;
}
