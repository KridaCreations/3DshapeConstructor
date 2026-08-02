//
// Created by abhishek on 02/08/26.
//

#ifndef SHAPERECONSTRUCTOR_VOXELMANAGER_H
#define SHAPERECONSTRUCTOR_VOXELMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"
#include "../include/DrawingElement.h"
#include "../include/Shader.h"

class VoxelManager {

    public:
    int m_length,m_width, m_height;
    float m_sideLength;
    std::vector<std::vector<std::vector<bool>>> m_voxels;

    DrawingElement* m_drawingElement;

    unsigned int m_texture;

    VoxelManager(int length,int width,int height,float sideLength,std::string texturePath,Shader shader);

    void setupChunk();

    void draw(glm::mat4 view,glm::mat4 projection);


};


#endif //SHAPERECONSTRUCTOR_VOXELMANAGER_H
