#pragma once
#ifndef TERRAINGEN_H
#define TERRAINGEN_H 
/*#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

struct coordsStruct
{
	float x;
	float y;
};

struct vertexStruct
{
	float xCoord;
	float yCoord;
	float zCoord;
	std::vector<float> getAsVector() { return std::vector<float>{xCoord, yCoord, zCoord}; }
};
typedef std::vector<vertexStruct> vertexRow;


namespace terrainGen
{
	float inline convertVal(float val, coordsStruct oldRange, coordsStruct newRange)
	{
		return ((val - oldRange.x) * (newRange.y - newRange.x)) / (oldRange.y - oldRange.x) + newRange.x;
	}

	std::vector<float> inline genPoints(uint8_t* imageData, int pointsWidth, int pointsHeight, int colorImageChannelNumber, coordsStruct leftUpPoint, coordsStruct rightDownPoint)
	{
		int dataSize = pointsHeight * (pointsWidth * colorImageChannelNumber);
		std::vector<float> returnResult;
		float xUnit = std::fabs((rightDownPoint.x - leftUpPoint.x) / (float)pointsWidth);
		float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y) / (float)pointsHeight);
		float lasVal = 0.0f;
		float yScale = 64.0f / 256.0f;
		float yShift = 1.0f;
		for (int i{ 0 }; i < pointsHeight; i++)
		{
			for (int j{ 0 }; j < pointsWidth; j++)
			{
				returnResult.push_back(leftUpPoint.x + ((float)(j)*xUnit));
				//returnResult.push_back(leftUpPoint.y - ((float)(i)*yUnit));
				if (imageData)
				{
					if ((i < pointsHeight) && (j < pointsWidth))
					{
						//int pos = j + (pointsWidth * i) * colorImageChannelNumber;
						int pos = j*colorImageChannelNumber + (pointsWidth * i) * (colorImageChannelNumber);
						float toPut = 0.0f;
						if (pos < dataSize)
						{
							//toPut = (float)(imageData[pos]) / 255.0f;
							toPut = ((float)(imageData[pos]) / 255.0f) * yScale;
							//toPut = convertVal(toPut, coordsStruct{ 0.0f, 1.0f }, coordsStruct{ -0.7f, 0.7f });
						}






						returnResult.push_back(toPut);
						lasVal = returnResult.back();
					}
				}

				else
				{
					//returnResult.push_back(0.0f);
				}
				returnResult.push_back(leftUpPoint.y - ((float)(i)*yUnit));
			}
		}
		return returnResult;
	}

	std::vector<float> inline genPointsUsingPerlin(int pointsWidth, int pointsHeight, coordsStruct leftUpPoint, coordsStruct rightDownPoint)
	{
		srand(time(0));
		int seed = rand();
		std::vector<float> returnResult;
		float xUnit = std::fabs((rightDownPoint.x - leftUpPoint.x) / (float)pointsWidth);
		float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y) / (float)pointsHeight);
		float lasVal = 0.0f;
		float yScale = 64.0f / 256.0f;
		float yShift = 12.0f;
		float yCoordRand = float(1+rand()%20);
		for (int i{ 0 }; i < pointsHeight; i++)
		{
			for (int j{ 0 }; j < pointsWidth; j++)
			{
				float x = leftUpPoint.x + ((float)(j)*xUnit);
				float z = leftUpPoint.y - ((float)(i)*yUnit);
				returnResult.push_back(x);		
				
				float yCoordToPush2 = stb_perlin_noise3_seed(x, yCoordRand, z, 2, 2, 2, seed);
				float yCoordToPush = stb_perlin_ridge_noise3(x, yCoordToPush2, z, 2.0f, 0.5f, 1.1f, 16);
				
				lasVal = yCoordToPush;
				returnResult.push_back(yCoordToPush *0.55f-0.2f);
				returnResult.push_back(z);
			}
		}
		
		return returnResult;
	}


	std::vector<vertexRow> inline genTerrainPointsUsingPerlin(int pointsWidth, int pointsHeight, int generatorSeed, coordsStruct leftUpPoint, coordsStruct rightDownPoint)
	{
		std::vector<vertexRow> returnResult;
		float xUnit = std::fabs((rightDownPoint.x - leftUpPoint.x) / (float)pointsWidth);
		float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y) / (float)pointsHeight);
		float lasVal = 0.0f;
		float yScale = 64.0f / 256.0f;
		float yShift = 12.0f;
		float yCoordRand = float(1 + rand() % 20);
		for (int i{ 0 }; i < pointsHeight; i++)
		{
			vertexRow rowToPush;
			for (int j{ 0 }; j < pointsWidth; j++)
			{
				vertexStruct d;
				d.xCoord  = leftUpPoint.x + ((float)(j)*xUnit);
				d.zCoord  = leftUpPoint.y - ((float)(i)*yUnit);
				

				float yCoordToPush2 = stb_perlin_noise3_seed(d.xCoord, yCoordRand, d.zCoord, 2, 2, 2, generatorSeed);
				d.yCoord =  stb_perlin_ridge_noise3(d.xCoord, yCoordToPush2, d.zCoord, 2.0f, 0.5f, 1.1f, 16)*0.55f - 0.2f;

				
				//returnResult.push_back(yCoordToPush * 0.55f - 0.2f);
				rowToPush.push_back(d);
			}
			returnResult.push_back(rowToPush);
		}

		return returnResult;
	}

	
	void inline makeSimpleTestErosion(std::vector<float>& pointsVec, float erosionStrength)
	{
		for (int i = 0; i < pointsVec.size(); i+=3)
		{
			if (pointsVec.at(i + 1) > 0.0f) pointsVec.at(i + 1) -= erosionStrength;
			else pointsVec.at(i + 1) += erosionStrength;
		}
	}
	void inline rainErosion(int x, int y, int rainDropLiveCycleNymber)
	{

	}
	std::vector<int> inline getIndices(int pointsWidth, int pointsHeight)
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
}*/
#endif // !TERRAINGEN_H

