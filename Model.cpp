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
	// Flags polygons for backface culling

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

		// Construct vectors a, b that can give the normal vector
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
	// Sort polygons by Z depth

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

void Model::CalculateLightingDirectional(std::vector<DirectionalLight> dLights)
{
	int totR, totG, totB;
	int tmpR, tmpG, tmpB;

	for (int i = 0; i < _polygons.size(); i++)
	{
		// Get already set RGB values from other lighting methods
		totR = GetRValue(_polygons[i].GetColour());
		totG = GetGValue(_polygons[i].GetColour());
		totB = GetBValue(_polygons[i].GetColour());

		for (int j = 0; j < dLights.size(); j++)
		{
			tmpR = dLights[j].GetR();
			tmpG = dLights[j].GetG();
			tmpB = dLights[j].GetB();

			// Apply reflection coefficients
			tmpR = tmpR * _kd_r;
			tmpG = tmpG * _kd_g;
			tmpB = tmpB * _kd_b;

			// Normalise vectors
			Vector3D lightNormal = dLights[j].GetDirection().GetNormalisedVector();
			Vector3D polyNormal =  _polygons[i].GetNormal().GetNormalisedVector();

			float dotProd = Vector3D::DotProduct(lightNormal, polyNormal);

			tmpR = tmpR * dotProd;
			tmpG = tmpG * dotProd;
			tmpB = tmpB * dotProd;

			totR = totR + tmpR;
			totG = totG + tmpG;
			totB = totB + tmpB;
		}

		// Cap values between 0 and 255
		if (totR > 255)
		{
			totR = 255;
		}
		else if (totR < 0)
		{
			totR = 0;
		}

		if (totG > 255)
		{
			totG = 255;
		}
		else if (totG < 0)
		{
			totG = 0;
		}

		if (totB > 255)
		{
			totB = 255;
		}
		else if (totB < 0)
		{
			totB = 0;
		}

		_polygons[i].SetColour(totR, totG, totB);
	}
}

void Model::CalculateLightingAmbient(AmbientLight aLight)
{
	int totR, totG, totB;
	int tmpR, tmpG, tmpB;

	for (int i = 0; i < _polygons.size(); i++)
	{
		totR = 0;
		totG = 0;
		totB = 0;

		tmpR = aLight.GetR();
		tmpG = aLight.GetG();
		tmpB = aLight.GetB();

		totR = tmpR * _ka_r;
		totG = tmpG * _ka_g;
		totB = tmpB * _ka_b;

		_polygons[i].SetColour(totR, totG, totB);
	}
}

void Model::CalculateLightingPoint(std::vector<PointLight> pLights)
{
	int totR, totG, totB;
	int tmpR, tmpG, tmpB;

	for (int i = 0; i < _polygons.size(); i++)
	{
		// Get already set RGB values from other lighting methods
		totR = GetRValue(_polygons[i].GetColour());
		totG = GetGValue(_polygons[i].GetColour());
		totB = GetBValue(_polygons[i].GetColour());

		for (int j = 0; j < pLights.size(); j++)
		{
			tmpR = pLights[j].GetR();
			tmpG = pLights[j].GetG();
			tmpB = pLights[j].GetB();

			// Apply reflection coefficients
			tmpR = tmpR * _kd_r;
			tmpG = tmpG * _kd_g;
			tmpB = tmpB * _kd_b;

			// Calculate vector TO the light source position
			Vector3D toLight = Vector3D(_transformedVertices[_polygons[i].GetIndex(0)].GetX() - pLights[j].GetPosition().GetX(),
										_transformedVertices[_polygons[i].GetIndex(0)].GetY() - pLights[j].GetPosition().GetY(),
										_transformedVertices[_polygons[i].GetIndex(0)].GetZ() - pLights[j].GetPosition().GetZ());

			float attenuation = 1 / toLight.GetMagnitude() * pow(toLight.GetMagnitude(), 2);

			// Normalise vectors
			Vector3D polyNormal = _polygons[i].GetNormal().GetNormalisedVector();
			Vector3D toLightNormal = toLight.GetNormalisedVector();

			float dotProd = Vector3D::DotProduct(toLightNormal, polyNormal);

			tmpR = tmpR * dotProd * attenuation;
			tmpG = tmpG * dotProd * attenuation;
			tmpB = tmpB * dotProd * attenuation;

			totR = totR + tmpR;
			totG = totG + tmpG;
			totB = totB + tmpB;
		}

		// Cap values between 0 and 255
		if (totR > 255)
		{
			totR = 255;
		}
		else if (totR < 0)
		{
			totR = 0;
		}

		if (totG > 255)
		{
			totG = 255;
		}
		else if (totG < 0)
		{
			totG = 0;
		}

		if (totB > 255)
		{
			totB = 255;
		}
		else if (totB < 0)
		{
			totB = 0;
		}

		_polygons[i].SetColour(totR, totG, totB);
	}
}
