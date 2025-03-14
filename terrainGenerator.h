#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#ifndef  TERRAINGENERATOR_H

#define TERRAINGENERATOR_H


typedef std::function<float(float, float, float, int, int, int, int)> perlinSeedFunctionType;
typedef std::function<float(float, float, float, float, float, float, float)> perlinGenerationFunctionType;


struct terrainCoordPoind2d
{
	float x;
	float y;
};


struct Particle {
	Particle(glm::vec2 _pos) { pos = _pos; }
	glm::vec2 pos;
	glm::vec2 speed = glm::vec2(0.0);
	float volume = 1.0;
	float sediment = 0.0;
};
class terrainInstance
{
public:
	 terrainInstance()
	{
		numberOfPointsOnOneDimension = 0;
		dim = glm::vec2(numberOfPointsOnOneDimension, numberOfPointsOnOneDimension);
	}
	void  GenEmptyPoints(int numberOfPoints)
	{
		terrainPoints.clear();
		terrainIndices.clear();
		this->numberOfPointsOnOneDimension = numberOfPoints;
		dim.x = numberOfPointsOnOneDimension;
		dim.y = numberOfPointsOnOneDimension;
		for (int i = 0; i < numberOfPointsOnOneDimension; i++)
		{
			terrainPoints.push_back(std::vector<glm::vec3>{(const unsigned _int64)numberOfPointsOnOneDimension, glm::vec3{ 0.0f, 0.0f, 0.0f }});
			terrainPointsNormales.push_back(std::vector<glm::vec3>{(const unsigned _int64)numberOfPointsOnOneDimension, glm::vec3(0.0f, 0.0f, 0.0f)});

		}
		for (int y = 0; y < dim.y - 1; y++)
		{
			for (int x{ 0 }; x < dim.x - 1; x++)
			{
				int pointNum = y * dim.x + x;
				terrainIndices.push_back(pointNum);
				terrainIndices.push_back(pointNum + 1);
				terrainIndices.push_back(pointNum + dim.x);

				terrainIndices.push_back(pointNum + dim.x + 1);
				terrainIndices.push_back(pointNum + 1);
				terrainIndices.push_back(pointNum + dim.x);
			}
		}
	}

	void  setXcoordsAndZCoords(terrainCoordPoind2d leftUpPoint, terrainCoordPoind2d rightDownPoint)
	{
		float xUnit = std::fabs((rightDownPoint.x - leftUpPoint.x) / (float)numberOfPointsOnOneDimension);
		float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y) / (float)numberOfPointsOnOneDimension);
		for (int i = 0; i < terrainPoints.size(); i++)
		{
			for (int j = 0; j < terrainPoints.at(i).size(); j++)
			{
				terrainPoints.at(i).at(j).x = leftUpPoint.x + ((float)(j)*xUnit);
				terrainPoints.at(i).at(j).z = leftUpPoint.y - ((float)(i)*yUnit);
			}
		}
	}

	void  generateYCoords(int generatorSeed, float powerCoeff, float yShift, perlinSeedFunctionType func1, perlinGenerationFunctionType func2)
	{
		float max = 0.0f, min = 0.0f;
		for (int i = 0; i < terrainPoints.size(); i++)
		{
			for (int j = 0; j < terrainPoints.at(i).size(); j++)
			{
				//float yCoordToPush2 = stb_perlin_noise3_seed(terrainPoints.at(i).at(j).x, terrainPoints.at(i).at(j).y, terrainPoints.at(i).at(j).z, 2, 2, 2, generatorSeed);
				float yCoordToPush2 = func1(terrainPoints.at(i).at(j).x, terrainPoints.at(i).at(j).y, terrainPoints.at(i).at(j).z, 2, 2, 2, generatorSeed);
				//terrainPoints.at(i).at(j).y = stb_perlin_ridge_noise3(terrainPoints.at(i).at(j).x, yCoordToPush2, terrainPoints.at(i).at(j).z, 2.0f, 0.5f, 1.1f, 16);
				//stb_perlin_ridge_noise3()
				terrainPoints.at(i).at(j).y = func2(terrainPoints.at(i).at(j).x, yCoordToPush2, terrainPoints.at(i).at(j).z, 2.0f, 0.4f, 1.1f, 32);
				terrainPoints.at(i).at(j).y *= powerCoeff;
				terrainPoints.at(i).at(j).y -= yShift;
				if (terrainPoints.at(i).at(j).y > max) max = terrainPoints.at(i).at(j).y;
				if (terrainPoints.at(i).at(j).y < min) min = terrainPoints.at(i).at(j).y;
			}
		}
		//normalize 
		for (int i = 0; i < terrainPoints.size(); i++)
		{
			for (int j = 0; j < terrainPoints.at(i).size(); j++)
			{
				terrainPoints.at(i).at(j).y = (terrainPoints.at(i).at(j).y - min) / (max - min);
			}
		}
	}

	const std::vector<float> inline getPointsAsOneDimensionalVector()
	{
		std::vector<float> retRes;
		for (auto rowIt = terrainPoints.begin(); rowIt != terrainPoints.end(); rowIt++)
		{
			for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
			{
				retRes.push_back(colIt->x);
				retRes.push_back(colIt->y);
				retRes.push_back(colIt->z);
			}
		}
		return retRes;
	}


	void  genNormales()
	{
		
		for (int i = 0; i < dim.y - 1; i++)
		{
			std::vector<glm::vec3> normalesRow;
			for (int j = 0; j < dim.x - 1; j++)
			{
				glm::vec3 a = terrainPoints.at(i).at(j);
				glm::vec3 b = terrainPoints.at(i+1).at(j);
				glm::vec3 c = terrainPoints.at(i).at(j+1);
				glm::vec3 d = terrainPoints.at(i+1).at(j+1);
				glm::vec3 n1 = glm::cross(a - b, c - b);
				//glm::vec3 n1 = glm::vec3(1.0, 0.0, 0.0);
				glm::vec3 n2 = glm::cross(d - c, b - c);
				//glm::vec3 n2 = glm::vec3(0.0, 1.0, 0.0);
				terrainPointsNormales.at(i).at(j) = n1;
				//terrainPointsNormales.at(i).at(j) = glm::vec3(1.0, 0.0, 0.0);
				//terrainPointsNormales.at(i+1).at(j) = n1;
				//terrainPointsNormales.at(i).at(j+1) = n1;

				terrainPointsNormales.at(i + 1).at(j + 1) = n2;
				terrainPointsNormales.at(i).at(j + 1) = n2;
				terrainPointsNormales.at(i+1).at(j) = n2;

			}
		}
	}
	std::vector<float>  getPointsAndNormalesAsOneDimensionalVector()
	{
		std::vector<float> retRes;
		for (int i = 0; i < numberOfPointsOnOneDimension; i++)
		{
			for (int j = 0; j < numberOfPointsOnOneDimension; j++)
			{
				retRes.push_back(terrainPoints.at(i).at(j).x);
				retRes.push_back(terrainPoints.at(i).at(j).y);
				retRes.push_back(terrainPoints.at(i).at(j).z);
				retRes.push_back(terrainPointsNormales.at(i).at(j).x);
				retRes.push_back(terrainPointsNormales.at(i).at(j).y);
				retRes.push_back(terrainPointsNormales.at(i).at(j).z);
				//retRes.push_back(0.0);
				//retRes.push_back(0.0);
				//retRes.push_back(0.0);
			}
		}
		return retRes;
	}

	glm::vec3  surfaceNormal(int i, int j)
	{
		if (((i - 1 >= 0) && (j - 1 >= 0)) && ((i + 1 < numberOfPointsOnOneDimension) && (j + 1 < numberOfPointsOnOneDimension)))
		{
			glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i + 1).at(j).y), 1.0, 1.0));
			n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i - 1).at(j).y - terrainPoints.at(i).at(j).y), 1.0, 1.0));
			n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i).at(j + 1).y)));
			n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (terrainPoints.at(i).at(j - 1).y - terrainPoints.at(i).at(j).y)));

			n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i + 1).at(j + 1).y) / sqrt(2), sqrt(2), scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i + 1).at(j + 1).y) / sqrt(2)));
			n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i + 1).at(j - 1).y) / sqrt(2), sqrt(2), scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i + 1).at(j - 1).y) / sqrt(2)));
			n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i - 1).at(j + 1).y) / sqrt(2), sqrt(2), scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i - 1).at(j + 1).y) / sqrt(2)));
			n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i - 1).at(j - 1).y) / sqrt(2), sqrt(2), scale * (terrainPoints.at(i).at(j).y - terrainPoints.at(i - 1).at(j - 1).y) / sqrt(2)));
			return n;
		}
		else return glm::vec3(0.0);
	}

	void  erode(int cycles)
	{
		srand(time(NULL));
		for (int i = 0; i < cycles; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				glm::vec2 newpos = glm::vec2(rand() % (int)dim.x, rand() % (int)dim.y);
				Particle drop(newpos);
				while (drop.volume > minVol)
				{
					glm::ivec2 ipos = drop.pos;
					glm::vec3 n = surfaceNormal(ipos.x, ipos.y);
					drop.speed += dt * glm::vec2(n.x, n.z) / (drop.volume * density);
					drop.pos += dt * drop.speed;
					drop.speed *= (1.0 - dt * friction);

					if (!glm::all(glm::greaterThanEqual(drop.pos, glm::vec2(0))) || !glm::all(glm::lessThan(drop.pos, dim))) break;
					float maxSediment = drop.volume * glm::length(drop.speed) * (terrainPoints.at(ipos.x).at(ipos.y).y - terrainPoints.at((int)drop.pos.x).at((int)drop.pos.y).y);
					if (maxSediment < 0.0) maxSediment = 0.0;
					float sdiff = maxSediment - drop.sediment;

					drop.sediment += dt * depositionRate * sdiff;
					terrainPoints.at(ipos.x).at(ipos.y).y -= dt * drop.volume * depositionRate * sdiff;

					drop.volume *= (1.0 - dt * evapRate);
				}
			}
		}
	}
	glm::vec2  get_dim() { return dim; }
	const glm::vec3  getPoint(int xPos, int yPos)
	{
		glm::vec3 retRes = glm::vec3(0.0, 0.0, 0.0);
		
			if ((yPos >= 0) && (yPos < terrainPoints.size()))
			{
				if ((xPos >= 0) && (xPos < terrainPoints.at(yPos).size()))
				{
					retRes = terrainPoints.at(yPos).at(xPos);
				}
			}
			
		
		return retRes;
	}

	

	 glm::vec2 getDim() { return this->dim; }
	

	static std::vector<int>  genIndices(int pointsWidth, int pointsHeight)
	{

		std::vector<int> returnResult;

		for (int y = 0; y < pointsHeight - 1; y++)
		{
			for (int x{ 0 }; x < pointsWidth - 1; x++)
			{
				int pointNum = y * pointsWidth + x;
				returnResult.push_back(pointNum);
				returnResult.push_back(pointNum + 1);
				returnResult.push_back(pointNum + pointsWidth);

				returnResult.push_back(pointNum + pointsWidth + 1);
				returnResult.push_back(pointNum + 1);
				returnResult.push_back(pointNum + pointsWidth);
			}
		}

		return returnResult;
	}
	static std::vector<int>  genIndices(terrainInstance* inputInstance)
	{
		return terrainInstance::genIndices(inputInstance->getDim().x, inputInstance->getDim().y);
	}
	const std::vector<int>  getIndices()
	{
		return std::vector<int>(terrainIndices.begin(), terrainIndices.end());
	}
	const std::vector<GLfloat>  getTerrainPointsAsVector()
	{
		std::vector<GLfloat> retResult;
		for (int i = 0; i < dim.y; i++)
		{
			for (int j = 0; j < dim.x; j++)
			{
				retResult.push_back(terrainPoints.at(i).at(j).x);
				retResult.push_back(terrainPoints.at(i).at(j).y);
				retResult.push_back(terrainPoints.at(i).at(j).z);
			}
		}
		return retResult;
	}
	const std::vector<GLfloat>  getTerrainNormalesAsVector()
	{
		std::vector<GLfloat> retResult;
		for (int i = 0; i < dim.y; i++)
		{
			for (int j = 0; j < dim.x; j++)
			{
				retResult.push_back(terrainPointsNormales.at(i).at(j).x);
				retResult.push_back(terrainPointsNormales.at(i).at(j).y);
				retResult.push_back(terrainPointsNormales.at(i).at(j).z);

			}
		}
		return retResult;
	}
	const bool  getIsReadyToUpdate() { return isReadyToUpdate; }
	void  setIsReadyToUpdate(bool input) { isReadyToUpdate = input; }
private: 
	std::vector<std::vector<glm::vec3>> terrainPoints;
	std::vector<int> terrainIndices;
	int numberOfPointsOnOneDimension;
	std::vector<std::vector<glm::vec3>> terrainPointsNormales;
	glm::vec2 dim;
	float minVol = 0.01;
	float dt = 1.2;
	float density = 1.0;
	float evapRate = 0.0001;
	float depositionRate = 0.1;
	double scale = 60.0;
	float friction = 0.1;
	bool isReadyToUpdate = false;

};
#endif // ! TERRAINGENERATOR_H

