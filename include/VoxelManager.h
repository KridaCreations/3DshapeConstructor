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
    glm::vec3 m_position;
    DrawingElement* m_drawingElement;

    unsigned int m_texture;

    VoxelManager(int length,int width,int height,float sideLength,std::string texturePath,Shader shader,glm::vec3 position);

    void setupChunk();

    void removeBLock(int x,int y, int z);
    void addBLock(int x,int y, int z);

    void draw(glm::mat4 view,glm::mat4 projection);

    glm::vec3 checkVoxel(glm::vec3 location);

    void switchOffVoxel(glm::vec3 location);

    void switchOnVoxel(glm::vec3 location);
};


#endif //SHAPERECONSTRUCTOR_VOXELMANAGER_H
