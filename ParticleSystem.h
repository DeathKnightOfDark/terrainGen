#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#endif // !PARTICLE_SYSTEM_H

class particleSystem
{
public:
	particleSystem();
	~particleSystem();
	void setup();
	void render(glm::mat4 projection, glm::mat4 view);
	void setEmitterPosition(glm::vec3 newEmitterPosition);
	void moveParticles(float add);
private:
	unsigned int particleVBO = 0;
	unsigned int particleVAO = 0;
	unsigned int particleEBO = 0;
	glm::vec3 emitterPosition;
	int maxNumberOfParticles = 10;
	std::vector<glm::mat4> particlesModelMatrices;
	std::vector<glm::vec3> particlesPositions;
	Shader particleShader;
};