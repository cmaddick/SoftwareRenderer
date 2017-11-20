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
	Vertex result;

	result.SetX(_matrix[0][0] * other.GetX() + _matrix[0][1] * other.GetY() + _matrix[0][2] * other.GetW());
	result.SetY(_matrix[1][0] * other.GetX() + _matrix[1][1] * other.GetY() + _matrix[1][2] * other.GetW());
	result.SetW(_matrix[2][0] * other.GetX() + _matrix[2][1] * other.GetY() + _matrix[2][2] * other.GetW());

	return result;
}