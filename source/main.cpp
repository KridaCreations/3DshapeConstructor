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
//#include <bits/valarray_after.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;






int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout <<"Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0,800,600);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;


    //storing the elements to draw
    std::vector<DrawingElement> drawingElementsList;

    // pushing the x Axis

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    unsigned char *data = stbi_load("../source/Resources/container.jpg",&width,&height,&nrChannels,0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load image" << std::endl;
    }
    stbi_image_free(data);


    Shader ourShader("../source/Shaders/axisVertexShader.shader", "../source/Shaders/axisFragmentShader.shader");

    DrawingElement xAxis(ourShader);
    xAxis.setType(DrawingElement::DrawElementType::LINES);
    xAxis.setVertices({
        -100.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f
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
        0.0f, -100.0f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f,  100.0f, 0.0f,      0.0f, 1.0f, 0.0f
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
        0.0f, 0.0f, -100.0f,      0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  100.0f,      0.0f, 0.0f, 1.0f
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


    // texture..........................................................



    // enabling depth testing
    glEnable(GL_DEPTH_TEST);




    while (!glfwWindowShouldClose(window)) {
        float currenFrame = glfwGetTime();
        deltaTime = currenFrame - lastFrame;
        lastFrame = currenFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);

        glm::mat4 projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.01f,100.f);



        for (auto &it:drawingElementsList) {
            it.m_shader.setMatrix4fv("view", view);
            it.m_shader.setMatrix4fv("projection", projection);
            it.draw(view,projection);
        }

        // glBindVertexArray(VAO1);
        // for (unsigned int i = 0; i < 10; i++) {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model,cubePositions[i]);
        //     float angle = 20.0f * i;
        //     // model = glm::rotate(model,glm::radians(angle) * (float)glfwGetTime(), glm::vec3(2.0f,0.6f,1.0f));
        //     model = glm::rotate(model,glm::radians(angle) * (float)glfwGetTime(), cubePositions[i]);
        //
        //     ourShader.setMatrix4fv("model", model);
        //
        //     glDrawArrays(GL_TRIANGLES,0,36);
        // }



        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const float cameraSpeed = 5.5f * deltaTime;

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

}
