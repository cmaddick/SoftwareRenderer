#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "DirectionalLight.h"

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

	void CalculateLightingDirectional(std::vector<DirectionalLight> dLights);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transformedVertices;

	// Reflection coeff
	float _ka_r = 0.8f;
	float _ka_g = 0.8f;
	float _ka_b = 0.8f;

	float _kd_r = 0.8f;
	float _kd_g = 0.8f;
	float _kd_b = 0.8f;

	float _ks_r = 0.8f;
	float _ks_g = 0.8f;
	float _ks_b = 0.8f;
};
