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

std::vector<Vertex>& Model::GetOriginalVertices()
{
	return _vertices;
}

std::vector<Vertex>& Model::GetTransformedVertices()
{
	return _transformedVertices;
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
	Vertex newVertex = Vertex(x, y, z, 1.0);

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

	_transformedVertices = newVertices;
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model::DehomogenizeVertices()
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].DehomogenizeVertex();
	}
}
