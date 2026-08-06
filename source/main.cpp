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
#include "ImageVoxelChunkIntersector.h"
//#include <bits/valarray_after.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window, VoxelManager *voxelChunk);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 300.0f);
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
    glEnable(GL_DEPTH_TEST);


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
    // int chunklength = 200,chunkWidth = 200, chunkHeight = 250;
    int chunklength = 25,chunkWidth = 25, chunkHeight = 37;
    VoxelManager newChunk(chunklength,chunkWidth,chunkHeight,sideLength,"../source/Resources/GreenRectangle.png",chunkShader,glm::vec3(0,0,0));
    // newChunk.setupChunk();
    newChunk.applyMarchingCube();
    float chunkDiagonal = glm::sqrt(((chunkWidth * chunkWidth) + (chunkHeight * chunkHeight)));

    float angleGap = 10;
    std::vector<ImageOutlineManager>images;
    for (float i = 0;i<180.0f;i += angleGap) {
        int imageHeight = ((float)chunkHeight)*sideLength, imageWidth = ((float)chunkWidth)*sideLength;
        glm::vec3 forwardVector = glm::vec3(glm::sin(glm::radians(i))*1,0,glm::cos(glm::radians(i))* -1);
        glm::vec3 imagePosition = newChunk.m_position - (forwardVector * ((chunkDiagonal / 2.0f) * 1.5f));
        imagePosition.y = newChunk.m_height/2.0f;
        ImageOutlineManager newImage3d(imageHeight,imageWidth,imagePosition,forwardVector,"../source/Resources/bottlePlushie.png",chunkShader);
        newImage3d.setupImageIn3dSpace();
        images.push_back(newImage3d);
    }



    std::vector<ImageVoxelChunkIntersector> intersectorArray;
    for (int i = 0;i<images.size();i++) {
        ImageVoxelChunkIntersector intersector(&newChunk,&images[i]);
        intersectorArray.push_back(intersector);
    }

    int currentIntersector = 0;

    while (!glfwWindowShouldClose(window)) {
        float currenFrame = glfwGetTime();
        deltaTime = currenFrame - lastFrame;
        lastFrame = currenFrame;

        processInput(window,&newChunk);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);

        glm::mat4 projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(fov),800.0f/600.0f,0.01f,2000.f);


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (auto &it:drawingElementsList) {
            it.m_shader.use();
            it.draw(view,projection);
        }

        newChunk.draw(view,projection);


        for (auto &it:images) {
            it.draw(view,projection);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        glfwSwapBuffers(window);
        glfwPollEvents();


        if (currentIntersector < intersectorArray.size()) {
            if (intersectorArray[currentIntersector].isDone() == false) {
                if (intersectorArray[currentIntersector].isRunning() == false) {
                    std::cout<<"starting the intersector  "<<currentIntersector<<std::endl;
                    intersectorArray[currentIntersector].start(sideLength);
                }else {
                    intersectorArray[currentIntersector].moveStep();
                    // std::cout<<"moved the intersector"<<std::endl;
                }
                if (intersectorArray[currentIntersector].isDone() == true) {
                    std::cout<<"done moving the intersector "<<currentIntersector<<std::endl;
                    // newChunk.setupChunk();
                    newChunk.applyMarchingCube();
                }
            }
            else {
                currentIntersector++;
            }
        }

    }


    glfwTerminate();
    return 0;

}


void processInput(GLFWwindow* window,VoxelManager *voxelChunk) {
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }

    const float cameraSpeed = 150.0f * deltaTime;

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
        }
    }else {
        iPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (jPressed == false) {
            jPressed = true;
        }
    }else {
        jPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (kPressed == false) {
            kPressed = true;
        }
    }else {
        kPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (lPressed == false) {
            lPressed = true;
        }
    }else {
        lPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        if (uPressed == false) {
            uPressed = true;
        }
    }else {
        uPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (oPressed == false) {
            oPressed = true;
        }
    }else {
        oPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (spacePressed == false) {
            spacePressed = true;
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