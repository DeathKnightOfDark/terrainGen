#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#ifndef  TERRAINGENERATOR_H
#include "terrainGenerator.h"
#endif // ! TERRAINGENERATOR_H


class terrainModel
{
public:
	inline terrainModel()
	{
		
		
		
		terInstanceLink = NULL;
	}
	inline terrainModel(terrainInstance* linkToExternalTerrainInstance)
	{
		terInstanceLink = linkToExternalTerrainInstance;
		
	}
	void inline setup()
	{
		glGenVertexArrays(1, &terrainVAO);
		glGenBuffers(1, &terrainVBO);
		glGenBuffers(1, &terrainNormaleVBO);
		glGenBuffers(1, &terrainEBO);
		if (terInstanceLink != NULL)
		{
			
			//std::vector<float> terrainPoints = terInstanceLink->getPointsAndNormalesAsOneDimensionalVector();
			std::vector<GLfloat> terrainPoints = terInstanceLink->getPointsAsOneDimensionalVector();
			std::vector<GLfloat> normalesPoints = terInstanceLink->getTerrainNormalesAsVector();
			std::vector<int> terrainIndices = terInstanceLink->getIndices();
			indicesSize = (int)terrainIndices.size();
			
			glBindVertexArray(terrainVAO);
			glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * terrainPoints.size(), terrainPoints.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, terrainNormaleVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalesPoints.size(), normalesPoints.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
			//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			//glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * terrainIndices.size(), terrainIndices.data(), GL_STATIC_DRAW);

			
		}
	}
	void inline update()
	{
		if (terInstanceLink != NULL)
		{//std::vector<float> terrainPoints = terInstanceLink->getPointsAndNormalesAsOneDimensionalVector();
			std::vector<GLfloat> terrainPoints = terInstanceLink->getPointsAsOneDimensionalVector();
			std::vector<GLfloat> normalesPoints = terInstanceLink->getTerrainNormalesAsVector();
			std::vector<int> terrainIndices = terInstanceLink->getIndices();
			indicesSize = (int)terrainIndices.size();
			






			glBindVertexArray(terrainVAO);
			glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * terrainPoints.size(), terrainPoints.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, terrainNormaleVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalesPoints.size(), normalesPoints.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

			//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			//glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * terrainIndices.size(), terrainIndices.data(), GL_STATIC_DRAW);

			
		}

	}

	void inline render()
	{
		glBindVertexArray(terrainVAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,terrainEBO);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	inline ~terrainModel()
	{

	}
	
private:
	unsigned int terrainVBO = 0;
	unsigned int terrainNormaleVBO = 0;
	unsigned int terrainEBO = 0;
	unsigned int terrainVAO = 0;
	int indicesSize = 0;
	terrainInstance *terInstanceLink;


	
};