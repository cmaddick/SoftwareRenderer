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
