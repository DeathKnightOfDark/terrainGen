#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>;
#include "shaders.h"
#include <vector>
#include <math.h>
#include "shaderClass.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
struct coordsStruct
{
	float x;
	float y;
};
std::vector<float> genPoints(uint8_t *imageData, int pointsWidth, int pointsHeight, int colorImageChannelNumber,  coordsStruct leftUpPoint, coordsStruct rightDownPoint)
{
	
	int dataSize = pointsHeight * (pointsWidth * colorImageChannelNumber);
	std::vector<float> returnResult;
	float xUnit = std::fabs( (rightDownPoint.x - leftUpPoint.x)/(float)pointsWidth);
	float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y)/(float)pointsHeight);
	for (int i{ 0 }; i < pointsHeight; i++)
	{
		for (int j{ 0 }; j < pointsWidth; j++)
		{
			returnResult.push_back(leftUpPoint.x + ((float)(j)*xUnit));
			returnResult.push_back(leftUpPoint.y - ((float)(i) * yUnit));
			if (imageData)
			{
				if ((i < pointsHeight) && (j < pointsWidth))
				{
					int pos = i * (j * colorImageChannelNumber) + j * colorImageChannelNumber;
					float toPut = 0.0f;
					if (pos < dataSize)
					{
						toPut = (float)(imageData[pos]) / 255.0f;
						if (imageData[pos] < 128) toPut = toPut * (-1.0f);
						toPut = toPut / 10.0f;
					}

					returnResult.push_back(toPut);
				}
			}
			
			else
			{
				returnResult.push_back(0.0f);
			}
		}
	}
	return returnResult;
}
std::vector<int> getIndices(int pointsWidth, int pointsHeight)
{
	std::vector<int> returnResult;
	
	for (int y = 0; y < pointsHeight - 1; y++)
	{
		for (int x{ 0 }; x < pointsWidth - 1; x++)
		{
			int pointNum = y * pointsWidth + x;
			returnResult.push_back(pointNum);
			returnResult.push_back(pointNum+1);
			returnResult.push_back(pointNum + pointsWidth);

			returnResult.push_back(pointNum+pointsWidth+1);
			returnResult.push_back(pointNum + 1);
			returnResult.push_back(pointNum + pointsWidth);
		}
	}
	return returnResult;
}
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
float lasX = 400;
float lastY = 400;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lasX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lasX;
	float yoffset = lastY - ypos;
	lasX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += xoffset;
	//if (pitch > 89.0f) pitch = 89.0f;
	//if (pitch < -89.0f) pitch = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}
int main()
{
	int x=0, y=0, n=0;
	uint8_t* data = stbi_load("noiseTexture.png", &x, &y, &n, 0);
	std::cout << n << std::endl;
	int width = 800;
	int height = 800;
	firstMouse = true;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to load glad" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	int points = 256;
	std::vector<float> verticesFloat = genPoints(data, x, y,n, coordsStruct{ -0.5f, 0.3f }, coordsStruct{ 0.5f, -0.7f });
	std::vector<int> indicesVec = getIndices(x, y);
	std::cout << "indices tris counter = " << indicesVec.size() / 3;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesFloat.size() , verticesFloat.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indicesVec.size(), indicesVec.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	Shader myShader("vertexShader.vs", "fragmentShader.fs");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glEnableVertexAttribArray(0);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	
	

	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	

	//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const float radius = 5.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		// rendering 

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();
		glBindVertexArray(VAO);
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 800.0f / 800.0f, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(myShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		
		
		glDrawElements(GL_TRIANGLES, indicesVec.size(), GL_UNSIGNED_INT, 0);

		
		
		//glBindVertexArray(0);
		//glDrawArrays(GL_POINTS, 0, verticesFloat.size());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}