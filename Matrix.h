#pragma once
#include "Vertex.h"
#include <initializer_list>
#include <vector>

const int ROWS = 3;
const int COLUMNS = 3;

class Matrix
{
public:
	// Default constructor
	Matrix();
	// Constructor that initialises all elements. Note that an initialiser
	// list is used to pass in the values. This is a new C++ feature.
	Matrix(std::initializer_list<float> list);
	// Copy constructor
	Matrix(const Matrix& other);
	// Destructor
	~Matrix();
	// Get matrix grid
	float GetMatrix() const;
	// Retrieve value in matrix at specified row and column
	float GetM(const int row, const int column) const;
	// Set value in matrix at specified row and column
	void SetM(const int row, const int column, const float value);
	// Copy assignment operator
	Matrix& operator= (const Matrix &rhs);
	// Multiply two matrices together
	const Matrix operator*(const Matrix &other) const;
	// Multiply a matrix by a vertex, returning a vertex
	const Vertex operator*(const Vertex &other) const;

private:
	float _matrix[ROWS][COLUMNS];
	void cpyMatrix(const Matrix &other);
};

