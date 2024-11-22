#pragma once
#ifndef TERRAINGEN_H
#define TERRAINGEN_H 
#include <vector>
#include <cmath>
struct coordsStruct
{
	float x;
	float y;
};
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
						int pos = i * (j * colorImageChannelNumber) + j * colorImageChannelNumber;
						float toPut = 0.0f;
						if (pos < dataSize)
						{
							toPut = (float)(imageData[pos]) / 255.0f;
							//if (imageData[pos] < 128) toPut = toPut * (-1.0f);
							//toPut = lasVal * (1.0f - 0.9f) + toPut * 0.9f;
							//toPut = toPut / 1.0f;
							toPut = convertVal(toPut, coordsStruct{ 0.0f, 1.0f }, coordsStruct{ -0.5f, 0.5f });
						}






						returnResult.push_back(toPut);
						lasVal = returnResult.back();
					}
				}

				else
				{
					returnResult.push_back(0.0f);
				}
				returnResult.push_back(leftUpPoint.y - ((float)(i)*yUnit));
			}
		}
		return returnResult;
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
}
#endif // !TERRAINGEN_H

