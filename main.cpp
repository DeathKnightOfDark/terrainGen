#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>;
#include "shaders.h"
#include <vector>
#include <math.h>
#include <gl
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
struct coordsStruct
{
	float x;
	float y;
};
std::vector<float> genPoints(int pointsWidth, int pointsHeight, coordsStruct leftUpPoint, coordsStruct rightDownPoint)
{
	std::vector<float> returnResult;
	float xUnit = std::fabs( (rightDownPoint.x - leftUpPoint.x)/(float)pointsWidth);
	float yUnit = std::fabs((leftUpPoint.y - rightDownPoint.y)/(float)pointsHeight);
	for (int i{ 0 }; i < pointsHeight; i++)
	{
		for (int j{ 0 }; j < pointsWidth; j++)
		{
			returnResult.push_back(leftUpPoint.x + ((float)(j)*xUnit));
			returnResult.push_back(leftUpPoint.y - ((float)(i) * yUnit));
			returnResult.push_back(0.0f);
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
int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 800);
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
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	int points = 100;
	std::vector<float> verticesFloat = genPoints(points, points, coordsStruct{ -0.5f, 0.3f }, coordsStruct{ 0.5f, -0.7f });
	std::vector<int> indicesVec = getIndices(points, points);
	std::cout << "indices tris counter = " << indicesVec.size() / 3;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesFloat.size() , verticesFloat.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indicesVec.size(), indicesVec.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED\N" << infolog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glEnableVertexAttribArray(0);
	while (!glfwWindowShouldClose(window))
	{
		// rendering 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, indicesVec.size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		//glDrawArrays(GL_POINTS, 0, verticesFloat.size());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}