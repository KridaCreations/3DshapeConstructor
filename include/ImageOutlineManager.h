//
// Created by abhishek on 04/08/26.
//


#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"

#include "../include/stb_image_resize2.h"
#include "../include/DrawingElement.h"
#include "../include/Shader.h"

#ifndef SHAPERECONSTRUCTOR_IMAGEOUTLINEMANAGER_H
#define SHAPERECONSTRUCTOR_IMAGEOUTLINEMANAGER_H


class ImageOutlineManager {
public:
    float m_height,m_width;
    int m_imageWidth,m_imageHeight,m_nrChannels;
    glm::vec3 m_position;
    glm::vec3 m_forwardVector;
    std::vector<std::vector<std::vector<float>>> m_pixelData;

    DrawingElement* m_drawingElement;

    unsigned int m_texture;

    ImageOutlineManager(int length,int width,glm::vec3 position,glm::vec3 forwardVector,std::string texturePath,Shader shader);

    void setupImageIn3dSpace();

    void draw(glm::mat4 view,glm::mat4 projection);

    unsigned char *  resizeImage(unsigned char *imageDate,int currentWidth,int m_nrChannels,int currentHeight,int newWidth,int newHeight);
};


#endif //SHAPERECONSTRUCTOR_IMAGEOUTLINEMANAGER_H
