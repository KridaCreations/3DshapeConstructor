#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<Shader.h>

#include "../include/stb_image.h"
#include "../include/DrawingElement.h"

#include <math.h>

#include "VoxelManager.h"
#include "../Cube.h"
#include "ImageOutlineManager.h"
//#include <bits/valarray_after.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window, Cube* naviCube, VoxelManager *voxelChunk);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

float yaw = -90.0f;
float pitch = 0.0f;

bool firstMouse = true;

float fov = 45.0f;



bool jPressed = false;
bool kPressed = false;
bool lPressed = false;
bool iPressed = false;
bool oPressed = false;
bool uPressed = false;
bool spacePressed = false;
bool escapePressed = false;

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout <<"Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0,1200,900);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;


    //storing the elements to draw
    std::vector<DrawingElement> drawingElementsList;

    // pushing the x Axis
    Shader ourShader("../source/Shaders/axisVertexShader.shader", "../source/Shaders/axisFragmentShader.shader");

    DrawingElement xAxis(ourShader);
    xAxis.setType(DrawingElement::DrawElementType::LINES);
    xAxis.setVertices({
        -1000.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        1000.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f
    },6);
    xAxis.setIndices({
        0, 1
    });
    xAxis.setAttributeArray({
    {3,true},
    {3,true}
    });
    xAxis.setup();
    drawingElementsList.push_back(xAxis);


    DrawingElement yAxis(ourShader);
    yAxis.setType(DrawingElement::DrawElementType::LINES);
    yAxis.setVertices({
        0.0f, -1000.0f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f,  1000.0f, 0.0f,      0.0f, 1.0f, 0.0f
    },6);
    yAxis.setIndices({
        0, 1
    });
    yAxis.setAttributeArray({
    {3,true},
    {3,true}
    });
    yAxis.setup();
    drawingElementsList.push_back(yAxis);

    DrawingElement zAxis(ourShader);
    zAxis.setType(DrawingElement::DrawElementType::LINES);
    zAxis.setVertices({
        0.0f, 0.0f, -1000.0f,      0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1000.0f,      0.0f, 0.0f, 1.0f
    },6);
    zAxis.setIndices({
        0, 1
    });
    zAxis.setAttributeArray({
    {3,true},
    {3,true}
    });
    zAxis.setup();
    drawingElementsList.push_back(zAxis);

    Shader chunkShader("../source/Shaders/VertexShader.shader", "../source/Shaders/FragmentShader.shader");
    float sideLength = 1;
    VoxelManager newChunk(150,150,200,sideLength,"../source/Resources/container.jpg",chunkShader);
    newChunk.setupChunk();


    // VoxelManager newChunk(150,150,200,sideLength,"../source/Resources/container.jpg",chunkShader);
    // newChunk.setupChunk();


    ImageOutlineManager newImage3d(200*sideLength,150*sideLength,glm::vec3(75,100,-10),glm::vec3(0,0,10),"../source/Resources/StarPlushie.png",chunkShader);
    newImage3d.setupImageIn3dSpace();


    Shader cubeShader("../source/Shaders/CubeVertexShader.shader", "../source/Shaders/CubeFragmentShader.shader");
    Cube newCube(sideLength,glm::vec3(0,0,0),"../source/Resources/container.jpg",cubeShader);
    newCube.setupChunk();
    // texture..........................................................





    // enabling depth testing
    glEnable(GL_DEPTH_TEST);




    while (!glfwWindowShouldClose(window)) {
        float currenFrame = glfwGetTime();
        deltaTime = currenFrame - lastFrame;
        lastFrame = currenFrame;

        processInput(window,&newCube,&newChunk);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);

        glm::mat4 projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(fov),800.0f/600.0f,0.01f,500.f);


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (auto &it:drawingElementsList) {
            it.m_shader.use();
            it.draw(view,projection);
        }

        newChunk.draw(view,projection);


        newImage3d.draw(view,projection);


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        newCube.draw(view,projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}


void processInput(GLFWwindow* window,Cube* naviCube,VoxelManager *voxelChunk) {
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }

    const float cameraSpeed = 50.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(glm::cross(cameraFront,cameraUp),cameraFront)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(glm::cross(cameraFront,cameraUp),cameraFront)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (iPressed == false) {
            iPressed = true;
            naviCube->m_lowerLeftCorner.z -= naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        iPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (jPressed == false) {
            jPressed = true;
            naviCube->m_lowerLeftCorner.x -= naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        jPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (kPressed == false) {
            kPressed = true;
            naviCube->m_lowerLeftCorner.z += naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        kPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (lPressed == false) {
            lPressed = true;
            naviCube->m_lowerLeftCorner.x += naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        lPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        if (uPressed == false) {
            uPressed = true;
            naviCube->m_lowerLeftCorner.y -= naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        uPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (oPressed == false) {
            oPressed = true;
            naviCube->m_lowerLeftCorner.y += naviCube->m_sideLength;
            naviCube->setupChunk();
        }
    }else {
        oPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (spacePressed == false) {
            spacePressed = true;
            int k = ((float)naviCube->m_lowerLeftCorner.z / naviCube->m_sideLength);
            int j = ((float)naviCube->m_lowerLeftCorner.y / naviCube->m_sideLength);
            int i = ((float)naviCube->m_lowerLeftCorner.x / naviCube->m_sideLength);

            std::cout<<"pressed "<<i <<" "<<j<<" "<<k<<std::endl;
            voxelChunk->m_voxels[i][j][k] = !voxelChunk->m_voxels[i][j][k];
            std::cout<<"new "<<voxelChunk->m_voxels[i][j][k]<<std::endl;
            voxelChunk->setupChunk();
        }
    }else {
        spacePressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // std::cout<<"pressed "<<std::endl;
        if (escapePressed == false) {
            escapePressed = true;
            int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);

            if (cursorMode == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else if (cursorMode == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }else {
        // std::cout<<"pressed "<<std::endl;
        escapePressed = false;
    }

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
    if (cursorMode != GLFW_CURSOR_DISABLED) {
        return;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;


    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);


}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}