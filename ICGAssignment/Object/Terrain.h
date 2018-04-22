#pragma once

#include <time.h>

#include <cyPoint.h>
#include <cyTriMesh.h>

#include "Core/Math/Matrix.h"
#include "Core/Math/PerlinNoise.h"



struct Terrain
{
public:
	Terrain(float centerX, float centerZ, float terrainSize, int gridCount, float basicHeight, float heightVariety)
		: centerX_(centerX), centerZ_(centerZ), terrainSize_(terrainSize), gridCount_(gridCount), basicHeight_(basicHeight), heightVariety_(heightVariety)
	{
		terrainMesh_ = new cy::TriMesh();
	}

	~Terrain()
	{
		delete terrainMesh_;
	}

	void InitTerrain()
	{
		int count = (gridCount_ + 1) * (gridCount_ + 1);
		terrainMesh_->SetNumVertex(count);
		terrainMesh_->SetNumNormals(count);
		terrainMesh_->SetNumTexVerts(count);
		terrainMesh_->SetNumFaces(gridCount_ * gridCount_ * 2);

		float initX = centerX_ - terrainSize_ * 0.5f;
		float initZ = centerZ_ - terrainSize_ * 0.5f;
		float gridSize = terrainSize_ / float(gridCount_);
		float texGridSize = 1.0f / float(gridCount_);

		PerlinNoise heightNoise(uint32(time(0)));
		
		for (int i = 0; i <= gridCount_; ++i)
		{
			for (int j = 0; j <= gridCount_; ++j)
			{
				//lastRandHeight += Math::RandGaussian(0.0f, texGridHeightVariety);
				float randHeight = (heightNoise.noise(float(i) / float(gridCount_), float(j) / float(gridCount_), 0.8f) * 2.0f - 1.0f) * heightVariety_;
				terrainMesh_->V(i * (gridCount_ + 1) + j) = cy::Point3f(initX + j * gridSize, basicHeight_ + randHeight , initZ + i * gridSize);
				terrainMesh_->VT(i * (gridCount_ + 1) + j) = cy::Point3f(float(j) * texGridSize, float(i) * texGridSize, 0.0f);
				if (i != gridCount_ && j != gridCount_)
				{
					int index1 = i * (gridCount_ + 1) + j, index2 = i * (gridCount_ + 1) + j + 1,
						index3 = (i + 1) * (gridCount_ + 1) + j + 1, index4 = (i + 1) * (gridCount_ + 1) + j;
					
					terrainMesh_->F((i * gridCount_ + j) * 2) = cy::TriMesh::TriFace(index1, index4, index3);
					terrainMesh_->F((i * gridCount_ + j) * 2 + 1) = cy::TriMesh::TriFace(index1, index3, index2);
				}
			}
		}

		for (int i = 0; i <= gridCount_; ++i)
		{
			for (int j = 0; j <= gridCount_; ++j)
			{
				if (i == 0 || i == gridCount_ || j == 0 || j == gridCount_)
				{
					terrainMesh_->VN(i * (gridCount_ + 1) + j) = cy::Point3f(0.0f, 1.0f, 0.0f);
				}
				else
				{
					cy::Point3f leftPoint = terrainMesh_->V(i * (gridCount_ + 1) + j - 1);
					cy::Point3f rightPoint = terrainMesh_->V(i * (gridCount_ + 1) + j + 1);
					cy::Point3f upPoint = terrainMesh_->V((i + 1) * (gridCount_ + 1) + j);
					cy::Point3f downPoint = terrainMesh_->V((i - 1) * (gridCount_ + 1) + j);

					float xValue = -(rightPoint.y - leftPoint.y) / (rightPoint.x - leftPoint.x);
					float zValue = -(upPoint.y - downPoint.y) / (upPoint.z - downPoint.z);
					terrainMesh_->VN(i * (gridCount_ + 1) + j) = cy::Point3f(xValue, 2.0f, zValue);
				}
			}
		}
	}

	cy::TriMesh *GetTerrainMesh() const { return terrainMesh_; }

private:
	float centerX_;
	float centerZ_;
	float terrainSize_;
	int gridCount_;
	float basicHeight_;
	float heightVariety_;
	cy::TriMesh *terrainMesh_;
};

