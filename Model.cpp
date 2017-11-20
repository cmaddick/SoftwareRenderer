#include "Model.h"
#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"


Model::Model()
{
}


Model::~Model()
{
}

std::vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

std::vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

size_t Model::GetPolygonCount() const
{
	return sizeof _polygons;
}

size_t Model::GetVertexCount() const
{
	return sizeof _vertices;
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex newVertex = Vertex(x, y, z, 0);

	_vertices.push_back(newVertex);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D newPolygon = Polygon3D(i0, i1, i2);

	_polygons.push_back(newPolygon);
}

void Model::ApplyTransformToOriginalVertices(const Matrix &transform)
{
	std::vector<Vertex> newVertices;

	_transformedVertices.clear();

	for (std::vector<Vertex>::iterator it = _vertices.begin(); it != _vertices.end(); it++)
	{
		newVertices.push_back(transform * *it);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	std::vector<Vertex> newVertices;

	for (std::vector<Vertex>::iterator it = _transformedVertices.begin(); it != _transformedVertices.end(); it++)
	{
		*it = transform * *it;
	}
}

void Model::DehomogenizeVertices()
{
	for (std::vector<Vertex>::iterator it = _transformedVertices.begin(); it != _transformedVertices.end(); it++)
	{
		it->DehomogenizeVertex();
	}
}
