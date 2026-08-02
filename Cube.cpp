//
// Created by abhishek on 02/08/26.
//

#include "Cube.h"

Cube::Cube(float sideLength,glm::vec3 lowerLeftCorner,std::string texturePath,Shader shader) {
    m_lowerLeftCorner = lowerLeftCorner;
    m_sideLength = sideLength;

    // unsigned int texture;
    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int texWidth, texHeight, texNrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texNrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load image" << std::endl;
    }
    stbi_image_free(data);


    m_drawingElement = new DrawingElement(shader);
}

void Cube::setupChunk() {
    m_drawingElement->setType(DrawingElement::DrawElementType::TRIANGLES);
    m_drawingElement->setAttributeArray({
    {3,true},
    {3,true}
    });
    m_drawingElement->m_stride = 6;

    m_drawingElement->m_vertices.clear();
    m_drawingElement->m_indices.clear();

    float offValue = 0.2;

    std::vector<float> location = {m_lowerLeftCorner.x - offValue , m_lowerLeftCorner.y - offValue, m_lowerLeftCorner.z - offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x - offValue , m_lowerLeftCorner.y - offValue , m_lowerLeftCorner.z + m_sideLength + offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x + m_sideLength + offValue , m_lowerLeftCorner.y - offValue , m_lowerLeftCorner.z + m_sideLength + offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x + m_sideLength + offValue , m_lowerLeftCorner.y - offValue , m_lowerLeftCorner.z - offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());


    location = {m_lowerLeftCorner.x - offValue , m_lowerLeftCorner.y + m_sideLength + offValue, m_lowerLeftCorner.z - offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x - offValue , m_lowerLeftCorner.y + m_sideLength + offValue , m_lowerLeftCorner.z + m_sideLength + offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x + m_sideLength + offValue , m_lowerLeftCorner.y + m_sideLength + offValue , m_lowerLeftCorner.z + m_sideLength + offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    location = {m_lowerLeftCorner.x + m_sideLength + offValue , m_lowerLeftCorner.y + m_sideLength + offValue , m_lowerLeftCorner.z - offValue,1.0f,0.0f,0.0f};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(), location.begin(), location.end());

    m_drawingElement->m_indices = {
        0,1,3,
        2,1,3,
        4,5,1,
        4,0,1,
        5,6,1,
        2,6,1,
        4,5,7,
        6,5,7,
        7,6,2,
        7,3,2,
        4,7,0,
        3,7,0
    };


    m_drawingElement->setup();
}


void Cube::draw(glm::mat4 view,glm::mat4 projection) {
    m_drawingElement->m_shader.use();

    m_drawingElement->draw(view,projection);
}