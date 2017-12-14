#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"

class Model
{
public:
	Model();
	~Model();

	// Getters/Setters
	std::vector<Polygon3D>& GetPolygons();
	std::vector<Vertex>& GetOriginalVertices();
	std::vector<Vertex>& GetTransformedVertices();
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

	// Transformation application
	void ApplyTransformToOriginalVertices(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);
	void DehomogenizeVertices();

	void CalculateBackfaces(Vertex cameraPos);

	void Sort(void);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transformedVertices;
};
