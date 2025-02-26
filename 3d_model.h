#pragma once
#ifndef MODEL_3D_H
#define MODEL_3D_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "shader.h"

class model3D
{
private:
	Shader modelShader;
	unsigned int modelVBO = 0;
	unsigned int modelNormaleVBO = 0;
	unsigned int modelEBO = 0;
	unsigned int modelVAO = 0;
	int indicesSize = 0;
public: 
	model3D(std::string vertexShader, std::string fragmentShader);
	~model3D();
	void setModel(std::vector<glm::vec3>& modelPoints, std::vector<int> indices);
	void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
};
#endif // !MODEL_3D_H
