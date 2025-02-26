#include "ParticleSystem.h"

particleSystem::particleSystem() : particleShader("shaders/particleShader.vs", "shaders/particleShader.fs")
{
	emitterPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	particlesModelMatrices = std::vector<glm::mat4>(maxNumberOfParticles, glm::mat4(1.0f));
	particlesPositions = std::vector<glm::vec3>(maxNumberOfParticles, emitterPosition);
}
particleSystem::~particleSystem()
{
	particlesModelMatrices.clear();
	particlesPositions.clear();
}

void particleSystem::setup()
{
	std::vector<glm::vec3> points;
	std::vector<float> pointData;
	std::vector<int> indices;
	points.push_back(glm::vec3(0.5f,  0.0f, 0.5f));
	points.push_back(glm::vec3(0.5f, 0.0f, -0.5f));
	points.push_back(glm::vec3(-0.5f,  0.0f, -0.5f));
	points.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));
	for (int i = 0; i < points.size(); i++)
	{
		pointData.push_back(points.at(i).x);
		pointData.push_back(points.at(i).y);
		pointData.push_back(points.at(i).z);
	}

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	glGenVertexArrays(1, &particleVAO);
	
	glGenBuffers(1, &particleVBO);
	glGenBuffers(1, &particleEBO);

	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), points.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointData.size(), pointData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	for (int i = 0; i < particlesPositions.size(); i++)
	{
		particlesPositions.at(i).y = i*0.3;
		particlesModelMatrices.at(i) = glm::scale(particlesModelMatrices.at(i), glm::vec3(5.0,5.0,5.0));
		particlesModelMatrices.at(i) = glm::rotate(particlesModelMatrices.at(i),(float)20*i,(const glm::vec3)glm::vec3(1.0,1.0,0.0));
		particlesModelMatrices.at(i) = glm::translate(particlesModelMatrices.at(i), (const glm::vec3)particlesPositions.at(i));
	}
	
}

void particleSystem::render(glm::mat4 projection, glm::mat4 view)
{
	particleShader.use();
	particleShader.setMat4("projection", projection);
	particleShader.setMat4("view", view);
	for (auto it = particlesModelMatrices.begin(); it != particlesModelMatrices.end(); it++)
	{
		glBindVertexArray(particleVAO);
		
		GLenum err;
		particleShader.setMat4("model", *it);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		err = glGetError();
		
		glBindVertexArray(0);
	}
	

}


void particleSystem::setEmitterPosition(glm::vec3 newEmitterPosition)
{
	emitterPosition = newEmitterPosition;
	for (int i = 0; i < particlesPositions.size(); i++)
	{
		particlesPositions.at(i) = emitterPosition + particlesPositions.at(i);
		//particlesModelMatrices.at(i) = glm::scale(particlesModelMatrices.at(i), glm::vec3(5.0, 5.0, 5.0));
		//particlesModelMatrices.at(i) = glm::rotate(particlesModelMatrices.at(i), (float)20 * i, const glm::vec3(1.0, 1.0, 0.0));
		//particlesModelMatrices.at(i) = glm::translate(particlesModelMatrices.at(i), (const glm::vec3)particlesPositions.at(i));
	}
}

void particleSystem::moveParticles(float add)
{
	for (int i = 0; i < particlesPositions.size(); i++)
	{
		particlesPositions.at(i).y += add;
		if (particlesPositions.at(i).y > 0.03f) particlesPositions.at(i).y = particlesPositions.at(i).y - 1.0f;
		particlesModelMatrices.at(i) = glm::translate(particlesModelMatrices.at(i), particlesPositions.at(i));
		
	}
}