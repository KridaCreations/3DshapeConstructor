//
// Created by abhishek on 02/08/26.
//

#include "../include/DrawingElement.h"

DrawingElement::DrawingElement(Shader shader) : m_shader(shader) {
}

void DrawingElement::setVertices(std::vector<float> vertices,unsigned int stride) {
    m_vertices = vertices;
    m_stride = stride;
}

void DrawingElement::setIndices(std::vector<unsigned int> indices) {
    m_indices = indices;
}

void DrawingElement::setAttributeArray(std::vector<std::pair<unsigned int, bool> > attributeArray) {
    m_attributeArray = attributeArray;
}

void DrawingElement::setType(DrawElementType type) {
    m_type = type;
}

void DrawingElement::setup() {

    // binding the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // binding the VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);

    // binding the EBO
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices.data(), GL_STATIC_DRAW);

    unsigned int offset = 0;
    for (unsigned int i = 0; i < m_attributeArray.size(); i++) {
        if (m_attributeArray[i].second == true) {
            glVertexAttribPointer(i,m_attributeArray[i].first,GL_FLOAT,GL_FALSE,m_stride*sizeof(float),(void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(i);
        }
        offset += m_attributeArray[i].first;
    }
}

void DrawingElement::draw(glm::mat4 view,glm::mat4 projection) {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    std::cout<<"drawing the  axis"<<std::endl;
    m_shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    m_shader.setMatrix4fv("model", model);
    glDrawArrays(static_cast<GLenum>(m_type),0,m_indices.size());
}