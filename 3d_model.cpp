
#include "3d_model.h"
model3D::model3D(std::string vertexShader, std::string fragmentShader) : modelShader(vertexShader.c_str(), fragmentShader.c_str())
{
	glGenVertexArrays(1, &modelVAO);
	glGenBuffers(1, &modelVBO);
	glGenBuffers(1, &modelNormaleVBO);
	glGenBuffers(1, &modelEBO);
}
model3D::~model3D()
{
	glDeleteVertexArrays(1, &modelVAO);
	glDeleteBuffers(1, &modelVBO);
	glDeleteBuffers(1, &modelNormaleVBO);
	glDeleteBuffers(1, &modelEBO);
}
void model3D::setModel(std::vector<glm::vec3>& modelPoints, std::vector<int> indices)
{
	
}

void model3D::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{

}