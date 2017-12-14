#include "Model.h"
#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include <algorithm>

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

void Model::CalculateBackfaces(Vertex cameraPos)
{
	Vertex v0, v1, v2;
	int i0, i1, i2;
	Vector3D a, b, normal, eyeVector;
	float tmp;

	for (int i = 0; i < _polygons.size(); i++)
	{
		i0 = _polygons[i].GetIndex(0);
		i1 = _polygons[i].GetIndex(1);
		i2 = _polygons[i].GetIndex(2);

		v0 = _transformedVertices[i0];
		v1 = _transformedVertices[i1];
		v2 = _transformedVertices[i2];

		a = v0 - v1;
		b = v0 - v2;

		normal = Vector3D::CrossProduct(b, a);
		_polygons[i].SetNormal(normal);

		eyeVector = v0 - cameraPos;

		tmp = Vector3D::DotProduct(normal, eyeVector);

		if (tmp < 0)
		{
			_polygons[i].MarkForCulling();
		}
		else
		{
			_polygons[i].UnMarkForCulling();
		}
	}
}

void Model::Sort(void)
{
	Vertex v0, v1, v2;
	int i0, i1, i2;
	float avgZ = 0.0f;

	for (int i = 0; i < _polygons.size(); i++)
	{
		i0 = _polygons[i].GetIndex(0);
		i1 = _polygons[i].GetIndex(1);
		i2 = _polygons[i].GetIndex(2);

		v0 = _transformedVertices[i0];
		v1 = _transformedVertices[i1];
		v2 = _transformedVertices[i2];

		avgZ = (v0.GetZ() + v1.GetZ() + v2.GetZ()) / 3;

		_polygons[i].SetAvgZDepth(avgZ);
	}

	std::sort(_polygons.begin(), _polygons.end(), [](const Polygon3D& lhs, const Polygon3D& rhs) {return lhs.GetAvgZDepth() > rhs.GetAvgZDepth();});
}


