//
// Created by abhishek on 02/08/26.
//

#ifndef SHAPERECONSTRUCTOR_DRAWINGELEMENT_H
#define SHAPERECONSTRUCTOR_DRAWINGELEMENT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include<Shader.h>

#include "../include/stb_image.h"

class DrawingElement {
public :
    enum DrawElementType
    {
        TRIANGLES = GL_TRIANGLES,
        LINES     = GL_LINES
    };

    DrawingElement(Shader shader) ;

    std::vector<unsigned int> m_indices;
    std::vector<float> m_vertices;
    std::vector<std::pair<unsigned int,bool>> m_attributeArray;
    unsigned int m_stride;
    unsigned int m_VAO, m_VBO, m_EBO;
    unsigned int texture;
    DrawElementType m_type;
    Shader m_shader;

    void setVertices(std::vector<float> vertices,unsigned int stride);
    void setIndices(std::vector<unsigned int> indices);
    void setAttributeArray(std::vector<std::pair<unsigned int,bool>> attributeArray);
    void setup();
    void setType(DrawElementType type);
    void setTexture(unsigned int texture);
    void setShader();


    void draw(glm::mat4 view,glm::mat4 projection);
};


#endif //SHAPERECONSTRUCTOR_DRAWINGELEMENT_H
