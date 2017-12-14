#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PointLight.h"

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

	// Backface culling
	void CalculateBackfaces(Vertex cameraPos);

	// Z depth sorting
	void Sort(void);

	// Light methods
	void CalculateLightingDirectional(std::vector<DirectionalLight> dLights);
	void CalculateLightingAmbient(AmbientLight aLight);
	void CalculateLightingPoint(std::vector<PointLight> pLights);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transformedVertices;

	// --- Reflection coeff ---
	// Ambient
	float _ka_r = 0.2f;
	float _ka_g = 0.2f;
	float _ka_b = 0.2f;

	// Diffuse
	float _kd_r = 0.5f;
	float _kd_g = 0.5f;
	float _kd_b = 0.5f;

	// Specular
	float _ks_r = 0.8f;
	float _ks_g = 0.8f;
	float _ks_b = 0.8f;
};
