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
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <thread>
#include <mutex>

#ifndef STB_PERLIN_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#endif 
#include "camera.h"
#include "terrainGenerator.h"
#include "terrainModel.h"
#include "ParticleSystem.h"
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#define ICEFIELD_HEIGHTMAP 1
#define NOISE_HEIGHTMAP 0
#define DRAWING_CUR NOISE_HEIGHTMAP

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(28.36f, 42.37f, 46.43f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(cameraPos);

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 2.5f * deltaTime;
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    
}
float lasX = SCR_WIDTH/2;
float lastY = SCR_HEIGHT/2;
float yaw = -124.0f;
float pitch = -35.0f;
float fov = 45.0f;
bool firstMouse = true;
glm::vec3 lightPos(5.2f, 200.0f, -10.0f);

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	/*float xpos = static_cast<float>(xPosIn) * 0.6f;
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
    std::cout << "camera.yaw=" << camera.Yaw << "; camera.pitch = " << camera.Pitch << ";\n";*/
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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_STENCIL_TEST);
    
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("colors.vs", "colors.fs");
   
    camera.Yaw = -123.0f;
    camera.Pitch = -35.0f;
    camera.ProcessMouseMovement(0.0f, 0.0f);
    
  

    ////////////////////////////////////////////
    int x, y;
  
    x = 512;
    y = 512;
    
    std::function<float(float, float, float, int, int, int, int)> func1 = stb_perlin_noise3_seed;
    std::function<float(float, float, float, float, float, float, float)> func2 = stb_perlin_ridge_noise3;

    terrainInstance testTerInst;
    testTerInst.GenEmptyPoints(x);
    testTerInst.setXcoordsAndZCoords(terrainCoordPoind2d{ -1.0f, 1.0f }, terrainCoordPoind2d{ 1.0f, -1.0f });
    testTerInst.generateYCoords(1441212, 0.9, 0.4,func1, func2);
    testTerInst.genNormales();
    
    //testTerInst.erode(20);

    terrainModel testTerrainModel(&testTerInst);
    testTerrainModel.setup();
    
    
    float lastErodeTime = 0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // render loop
    // -----------
   // testTerInst->setIsReadyToUpdate(true);
    glm::vec3 flatColor = glm::vec3(0.27, 0.64, 0.27);
    glm::vec3 steepColor = glm::vec3(0.7);
    float angle = 0;

    //framebufferTry
   
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 
    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer,0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float quadVertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f, 
        -1.0f, -1.0f, 0.0f, 0.0f, 
        1.0f, -1.0f, 1.0f, 0.0f, 

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 
        1.0f, 1.0f, 1.0f, 1.0f
    };
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //pSystem.moveParticles(0.0003f*deltaTime);
        //pSystem.render(projection, view);
        
        // be sure to activate shader when setting uniforms/drawing objects
        
       
        
        // view/projection transformations
        
        
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view); 
        lightingShader.setFloat("time", glfwGetTime());
        lightingShader.setVec3("ambientColor", glm::vec3(0.1, 0.6, 0.1));
        lightingShader.setVec3("lightPos", glm::vec3(0.0,30.0,0.0));
        lightingShader.setVec3("cameraPos", camera.Position);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(25.0f, 15.0f, 25.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
       // model = glm::translate(model, glm::vec3(0.0, 10.0, 0.0));
        lightingShader.setMat4("model", model);
        testTerrainModel.render();
        
        
        
       
#if (DRAWING_CUR==ICEFIELD_HEIGHTMAP) 
        model = glm::scale(model, glm::vec3(100.0f, 10.0f, 100.0f));
#endif 
#if (DRAWING_CUR==NOISE_HEIGHTMAP)
       
#endif
        angle += 60 * deltaTime;
        

        

       
      


        



       


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    //glDeleteBuffers(1, &VBO);
    //delete(testTerInst);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
    }
