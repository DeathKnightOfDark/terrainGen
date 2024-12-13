#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <iostream>;
#include "shader.h"
#include <vector>
#include <math.h>
//#include "filesystem.h"
//#include "shaderClass.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "camera.h"
#include "terrainGen.h"
#include "terrainGenerator.h"

void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#define ICEFIELD_HEIGHTMAP 1
#define NOISE_HEIGHTMAP 0
#define DRAWING_CUR NOISE_HEIGHTMAP


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::vec3 cameraPos = glm::vec3(0.0f, 20.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 20.0f, 3.0f));
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}
float lasX = SCR_WIDTH/2;
float lastY = SCR_HEIGHT/2;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;
glm::vec3 lightPos(1.2f, 2.0f, 3.0f);

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xpos = static_cast<float>(xPosIn)*0.6f;
	float ypos = static_cast<float>(yPosIn)*0.6f;
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
	camera.ProcessMouseMovement(xoffset, yoffset);
}

glm::vec3 getNormale(glm::vec3 startPoint, glm::vec3 pointOne, glm::vec3 pointTwo)
{
    return glm::normalize(glm::cross((pointOne - startPoint), (pointTwo - startPoint)));
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
int main()
{    // glfw: initialize and configure
        // ------------------------------
        // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("colors.vs", "colors.fs");
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");


    glm::vec3 pointOne{0.0f, 0.0f, 0.0f}, pointTwo(1.0f, 0.0f, 0.0f), pointThree{ 0.0f, 0.0f, -1.0f }, pointFour{ 1.0f, 0.0f, -1.0f };
    glm::vec3 normOne = getNormale(pointOne, pointTwo, pointThree);
    glm::vec3 normTwo = getNormale(pointTwo, pointOne, pointThree);
    glm::vec3 normThree = getNormale(pointThree, pointOne, pointTwo);
    std::cout << "first pass" << std::endl;
   // glm::vec3 normThree = glm::normalize(glm::cross((pointTwo - pointThree), (pointOne - pointThree)));
    std::cout << "normale one = (" << normOne.x << ";" << normOne.y << ";" << normOne.z << ")" << std::endl;
    std::cout << "normale two = (" << normTwo.x << ";" << normTwo.y << ";" << normTwo.z << ")" << std::endl;
    std::cout << "normale three = (" << normThree.x << ";" << normThree.y << ";" << normThree.z << ")" << std::endl;

    normOne = getNormale(pointOne,  pointThree,pointTwo );
    normTwo = getNormale(pointTwo,  pointThree, pointOne);
    normThree = getNormale(pointThree,  pointTwo, pointOne);
    std::cout << "second pass" << std::endl;
    // glm::vec3 normThree = glm::normalize(glm::cross((pointTwo - pointThree), (pointOne - pointThree)));
    std::cout << "normale one = (" << normOne.x << ";" << normOne.y << ";" << normOne.z << ")" << std::endl;
    std::cout << "normale two = (" << normTwo.x << ";" << normTwo.y << ";" << normTwo.z << ")" << std::endl;
    std::cout << "normale three = (" << normThree.x << ";" << normThree.y << ";" << normThree.z << ")" << std::endl;
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    ////////////////////////////////////////////
    int x, y, channels;
#if (DRAWING_CUR==ICEFIELD_HEIGHTMAP)

    unsigned char* data = stbi_load("iceland_heightmap.png", &x, &y, &channels, 0);
#endif
#if (DRAWING_CUR==NOISE_HEIGHTMAP)
    unsigned char* data = stbi_load("noiseTexture.png", &x, &y, &channels, 0);
    //unsigned char* data = stbi_load("height.jpg", &x, &y, &channels, 0);
#endif
    std::cout << "x=" << x << std::endl;
    std::cout << "y=" << y << std::endl;
    std::cout << "channels=" << channels << std::endl;
    //std::vector<float> terrainPoints = terrainGen::genPoints(data, x, y, channels, coordsStruct{ -0.5f,0.5f }, coordsStruct{ 0.5f, -0.5f });
    x = 128;
    y = 128;
    //std::vector<float> terrainPoints = terrainGen::genPointsUsingPerlin(x, y, coordsStruct{ -0.5f,0.5f }, coordsStruct{ 0.5f, -0.5f });
    //terrainGen::makeSimpleTestErosion(terrainPoints, 0.02f);
    //terrainGen::makeSimpleTestErosion(terrainPoints, 0.02f);
    //terrainGen::makeSimpleTestErosion(terrainPoints, 0.02f);

    terrainInstance testTerInst;
    testTerInst.GenEmptyPoints(x);
    testTerInst.setXcoordsAndZCoords(terrainCoordPoind2d{ -1.0f, 1.0f }, terrainCoordPoind2d{ 1.0f, -1.0f });
    testTerInst.generateYCoords(63344210000);
    testTerInst.genNormales();
    //testTerInst.erodeWithRain(30);
    //std::vector<float> testTerInstPoints = testTerInst.getPointsAsOneDimensionalVector();
    std::vector<float> testTerInstPoints = testTerInst.getPointsAndNormalesAsOneDimensionalVector();
    //std::cout << "points number=" << terrainPoints.size()/3<< std::endl;
    std::vector<int> terrainIndices = terrainGen::getIndices(x, y);
     
    ////////////////////////////////////////////
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO, terrainVBO, terrainVAO, terrainEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &terrainVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &terrainVBO);
    glGenBuffers(1, &terrainEBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);*/

   

    

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(terrainVAO);

    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* testTerInstPoints.size(), testTerInstPoints.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* terrainIndices.size(), terrainIndices.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normale
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    int trisNumber = terrainIndices.size();
    //terrainPoints.clear();
    terrainIndices.clear();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 0.5f, 0.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
#if (DRAWING_CUR==ICEFIELD_HEIGHTMAP) 
        model = glm::scale(model, glm::vec3(100.0f, 10.0f, 100.0f));
#endif 
#if (DRAWING_CUR==NOISE_HEIGHTMAP)
        model = glm::scale(model, glm::vec3(25.0f, 15.0f, 25.0f));
#endif
        
        //model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);

        // render the cube
        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //lightingShader.use();
        glBindVertexArray(terrainVAO);
        glDrawElements(GL_TRIANGLES, trisNumber, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
    }
