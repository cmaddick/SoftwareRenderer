#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
class Model
{
public:
	Model();
	~Model();

	std::vector<Polygon3D>& GetPolygons();
	std::vector<Vertex>& GetVertices();
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
};

