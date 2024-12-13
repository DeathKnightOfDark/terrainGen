#pragma once
#ifndef  TERRAINGENERATOR_H
#include <vector>
#include <cmath>
#include <map>


#ifndef STB_PERLIN_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#endif // 

#include <glm/glm.hpp>
#define TERRAINGENERATOR_H

struct waterDrop
{
	/// <summary>
	/// Сколько циклов капле осталось
	/// </summary>
	int lifeTimeCyclesRemaining;

	/// <summary>
	/// Объем земли, забранный каплей
	/// </summary>
	float earthVelocity;
};
struct terrainCoordPoind2d
{
	float coordX;
	float coordY;
};

struct terrainCoordPoint3d 
{
	float coordX;
	float coordY;
	float coordZ;
	glm::vec3 getAsGLMVec3() { return glm::vec3{coordX, coordY, coordZ}; }
};
class terrainInstance
{
public: 
	inline terrainInstance()
	{
		numberOfPointsOnOneDimension = 0;
	}
	void inline GenEmptyPoints(int numberOfPoints)
	{
		terrainPoints.clear();
		this->numberOfPointsOnOneDimension = numberOfPoints;
		for (int i = 0; i < numberOfPointsOnOneDimension; i++)
		{
			terrainPoints.push_back(std::vector<terrainCoordPoint3d>{(const unsigned _int64)numberOfPointsOnOneDimension, terrainCoordPoint3d{ 0.0f, 0.0f, 0.0f }});

		}
	}

	void inline setXcoordsAndZCoords(terrainCoordPoind2d leftUpPoint, terrainCoordPoind2d rightDownPoint)
	{
		float xUnit = std::fabs((rightDownPoint.coordX - leftUpPoint.coordX) / (float)numberOfPointsOnOneDimension);
		float yUnit = std::fabs((leftUpPoint.coordY - rightDownPoint.coordY) / (float)numberOfPointsOnOneDimension);
		for (int i=0;i<terrainPoints.size();i++)
		{
			for (int j=0;j<terrainPoints.at(i).size();j++)
			{
				terrainPoints.at(i).at(j).coordX = leftUpPoint.coordX + ((float)(j)*xUnit);
				terrainPoints.at(i).at(j).coordZ = leftUpPoint.coordY - ((float)(i)*yUnit);
			}
		}
	}

	void inline generateYCoords(int generatorSeed = 0, float powerCoeff = 1.0f, float yShift = 0.0f)
	{
		float max = 0.0f, min = 0.0f;
		for (int i = 0; i < terrainPoints.size(); i++)
		{
			for (int j = 0; j < terrainPoints.at(i).size(); j++)
			{
				float yCoordToPush2 = stb_perlin_noise3_seed(terrainPoints.at(i).at(j).coordX, terrainPoints.at(i).at(j).coordY, terrainPoints.at(i).at(j).coordZ, 2, 2, 2, generatorSeed);
				terrainPoints.at(i).at(j).coordY = stb_perlin_ridge_noise3(terrainPoints.at(i).at(j).coordX, yCoordToPush2, terrainPoints.at(i).at(j).coordZ, 2.0f, 0.5f, 1.1f, 16);
				terrainPoints.at(i).at(j).coordY *= powerCoeff;
				terrainPoints.at(i).at(j).coordY -= yShift;
				if (terrainPoints.at(i).at(j).coordY > max) max = terrainPoints.at(i).at(j).coordY;
				if (terrainPoints.at(i).at(j).coordY < min) min = terrainPoints.at(i).at(j).coordY;
			}
		}
		//normalize 
		for (int i = 0; i < terrainPoints.size(); i++)
		{
			for (int j = 0; j < terrainPoints.at(i).size(); j++)
			{
				terrainPoints.at(i).at(j).coordY = (terrainPoints.at(i).at(j).coordY - min) / (max - min);
			}
		}
	}
	
	std::vector<float> inline getPointsAsOneDimensionalVector()
	{
		std::vector<float> retRes;
		for (auto rowIt = terrainPoints.begin(); rowIt != terrainPoints.end(); rowIt++)
		{
			for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
			{
				retRes.push_back(colIt->coordX);
				retRes.push_back(colIt->coordY);
				retRes.push_back(colIt->coordZ);
			}
		}
		return retRes;
	}
	std::vector<float> inline getPointsAndNormalesAsOneDimensionalVector()
	{
		std::vector<float> retRes;
		for (int i = 0; i < numberOfPointsOnOneDimension; i++)
		{
			for (int j = 0; j < numberOfPointsOnOneDimension; j++)
			{
				retRes.push_back(terrainPoints.at(i).at(j).coordX);
				retRes.push_back(terrainPoints.at(i).at(j).coordY);
				retRes.push_back(terrainPoints.at(i).at(j).coordZ);
				retRes.push_back(terrainPointsNormales.at(i).at(j).x);
				retRes.push_back(terrainPointsNormales.at(i).at(j).y);
				retRes.push_back(terrainPointsNormales.at(i).at(j).z);
			}
		}
		return retRes;
	}
	void inline erodeWithRain(int rainCycles)
	{
		const float earthPerSample = 0.005f;
		float max = 0.0f, min = 0.0f;
		for (int rainCycle = 0; rainCycle < rainCycles; rainCycle++)
		{
			std::vector<std::vector<waterDrop>> dropMap{(const unsigned _int64)this->numberOfPointsOnOneDimension, std::vector<waterDrop>{(const unsigned _int64)numberOfPointsOnOneDimension, { 5, 0 }}};

			for (int dropLife = 0; dropLife < 5; dropLife++)
			{
				
				for (int i = 0; i < dropMap.size(); i++)
				{
					for (int j = 0; j < dropMap.at(i).size(); j++)
					{
						float min = terrainPoints.at(i).at(j).coordY;
						int maxI = i, maxJ = j;
						///высчитываем, куда капле двигаться, по поверхности фоннеймана
						for (int fi = i - 1; fi <= i + 1; fi++)
						{
							if ((fi >= 0) && (fi < numberOfPointsOnOneDimension))
							{
								
								for (int fj = j - 1; fj <= j + 1; fj++)
								{
									if ((fj >= 0) && (fj < numberOfPointsOnOneDimension))
									{
										if (terrainPoints.at(fi).at(fj).coordY < min)
										{
											min = terrainPoints.at(fi).at(fj).coordY;
											maxI = fi;
											maxJ = fj;
										}
									}
								}
							}
						}
						dropMap.at(i).at(j).earthVelocity += earthPerSample;
						terrainPoints.at(i).at(j).coordY -= earthPerSample;
						dropMap.at(maxI).at(maxJ).earthVelocity += dropMap.at(i).at(j).earthVelocity;
						dropMap.at(i).at(j).earthVelocity = 0.0f;
						
					}
				}
			}

			for (int i = 0; i < numberOfPointsOnOneDimension; i++)
			{
				for (int j = 0; j < numberOfPointsOnOneDimension; j++)
				{
					terrainPoints.at(i).at(j).coordY += dropMap.at(i).at(j).earthVelocity;
					if (terrainPoints.at(i).at(j).coordY > max) max = terrainPoints.at(i).at(j).coordY;
					if (terrainPoints.at(i).at(j).coordY < min) min = terrainPoints.at(i).at(j).coordY;
				}
			}
			//normalize 
			for (int i = 0; i < terrainPoints.size(); i++)
			{
				for (int j = 0; j < terrainPoints.at(i).size(); j++)
				{
					terrainPoints.at(i).at(j).coordY = (terrainPoints.at(i).at(j).coordY - min) / (max - min);
					
				}
			}
		}

	
		
	}
	void inline genNormales()
	{
		if (terrainPointsNormales.size()!=0) terrainPointsNormales.clear();
		terrainPointsNormales = std::vector<std::vector<glm::vec3>>{ (const unsigned _int64)numberOfPointsOnOneDimension, std::vector<glm::vec3>{(const unsigned _int64)numberOfPointsOnOneDimension,glm::vec3{0.0f,0.0f,0.0f}} };
		for (int i = 1; i < terrainPoints.size(); i++)
		{
			int startIndex = 1, endIndex = (int)terrainPoints.size();
			if (i % 2 == 0)
			{
				startIndex = 0;
				endIndex -= 1;
			}
			for (int j = 0; j < (int)terrainPoints.at(i).size()-1; j++)
			{
				glm::vec3 vecOne, vecTwo;
				if(startIndex == 0)
				{
					 vecOne = terrainPoints.at(i).at(j + 1).getAsGLMVec3() - terrainPoints.at(i).at(j).getAsGLMVec3();
					 vecTwo = terrainPoints.at(i+1).at(j).getAsGLMVec3() - terrainPoints.at(i).at(j).getAsGLMVec3();
				}
				else
				{
					vecOne = terrainPoints.at(i).at(j+1).getAsGLMVec3() - terrainPoints.at(i).at(j).getAsGLMVec3();
					vecTwo = terrainPoints.at(i-1).at(j+1).getAsGLMVec3() - terrainPoints.at(i).at(j).getAsGLMVec3();
				}
				glm::vec3 crProd = glm::cross(vecOne, vecTwo);
				terrainPointsNormales.at(i).at(j) = glm::normalize(crProd);
				
			}
			

		}
	}

private: 
	std::vector<std::vector<terrainCoordPoint3d>> terrainPoints;
	int numberOfPointsOnOneDimension;
	std::vector<std::vector<glm::vec3>> terrainPointsNormales;

};
#endif // ! TERRAINGENERATOR_H

