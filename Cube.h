//
// Created by abhishek on 02/08/26.
//
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"
#include "../include/DrawingElement.h"
#include "../include/Shader.h"
#ifndef SHAPERECONSTRUCTOR_CUBE_H
#define SHAPERECONSTRUCTOR_CUBE_H


class Cube {

public:
    int m_length,m_width, m_height;
    float m_sideLength;
    glm::vec3 m_lowerLeftCorner;
    std::vector<std::vector<std::vector<bool>>> m_voxels;

    DrawingElement* m_drawingElement;

    unsigned int m_texture;

    Cube(float sideLength,glm::vec3 lowerLeftCorner,std::string texturePath,Shader shader);

    void setupChunk();

    void draw(glm::mat4 view,glm::mat4 projection);

};


#endif //SHAPERECONSTRUCTOR_CUBE_H
