//
// Created by abhishek on 04/08/26.
//

#include "../include/ImageOutlineManager.h"

ImageOutlineManager::ImageOutlineManager(int height,int width,glm::vec3 position,glm::vec3 forwardVector,std::string texturePath,Shader shader) {
    m_height = height;
    m_width = width;

    m_position = position;
    m_forwardVector = forwardVector;

    m_pixelData.resize(m_height);
    for (int i = 0;i<m_height;i++) {
        m_pixelData[i].resize(m_width);
        for (int j =0;j<m_width;j++) {
            m_pixelData[i][j].resize(4,0); //to accomodate the alpha channels
        }
    }


    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int m_imageWidth, m_imageHeight, m_nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &m_imageWidth, &m_imageHeight, &m_nrChannels, STBIR_RGBA); //STBIR_RGBA
    unsigned char * resizedData = new unsigned char[width * height * m_nrChannels];
    resizedData = resizeImage(data,m_imageWidth,m_imageHeight,m_nrChannels,m_width,m_height);
    if (resizedData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,GL_UNSIGNED_BYTE, resizedData);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] resizedData;
    } else {
        std::cout << "Failed to load image" << std::endl;
    }

    stbi_image_free(data);

    m_drawingElement = new DrawingElement(shader);

}



unsigned char * ImageOutlineManager::resizeImage(unsigned char *imageData,int currentWidth,int currentHeight,int nrChannels,int expectedWidth,int expectedHeight) {
    //finding the new aspect ration
    float heightRatio = (float)currentHeight / (float)expectedHeight;
    float widthRatio = (float)currentWidth / (float)expectedWidth;
    int newWidth,newHeight;
    if (heightRatio > widthRatio) {
        newHeight = expectedHeight;
        newWidth = (float)currentWidth / heightRatio;
    } else {
        newWidth = expectedWidth;
        newHeight = (float)currentHeight / widthRatio;
    }

    unsigned char* output = new unsigned char[newWidth * newHeight * nrChannels];

    stbir_resize_uint8_linear(
        imageData,
        currentWidth,
        currentHeight,
        0,
        output,
        newWidth,
        newHeight,
        0,
        STBIR_RGBA);

    // *imageData = *output;

    int wOffset = (expectedWidth - newWidth)/2;
    int hOffset = (expectedHeight - newHeight)/2;
    for (int i = 0;i<newHeight;i++) {
        for (int j =0;j<newWidth;j++) {
            int offset =( (i * newWidth) + j) * 4;
            m_pixelData[i + hOffset][j + wOffset][0] = output[offset + 0];
            m_pixelData[i + hOffset][j + wOffset][1] = output[offset + 1];
            m_pixelData[i + hOffset][j + wOffset][2] = output[offset + 2];
            m_pixelData[i + hOffset][j + wOffset][3] = output[offset + 3];
        }
    }

    unsigned char* finalImage = new unsigned char[expectedWidth * expectedHeight * 4];
    memset(finalImage, 0, expectedWidth * expectedHeight * 4);
    int xOffset = (expectedWidth  - newWidth) / 2;
    int yOffset = (expectedHeight - newHeight) / 2;

    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            int src = (y * newWidth + x) * 4;

            int dst =
                ((y + yOffset) * expectedWidth +
                 (x + xOffset)) * 4;

            memcpy(finalImage + dst,
                   output + src,
                   4);
        }
    }

    return finalImage;
}

void ImageOutlineManager::setupImageIn3dSpace() {
    m_drawingElement->setType(DrawingElement::DrawElementType::TRIANGLES);
    m_drawingElement->setAttributeArray({
    {3,true},
    {2,true}
    });
    m_drawingElement->m_stride = 5;
    m_drawingElement->m_vertices.clear();
    m_drawingElement->m_indices.clear();

    // float offValue = 0.01;

    // glm::vec3 middleForwardVector = m_forwardVector;
    glm::vec3 upVector = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 rightVector = glm::cross(m_forwardVector,upVector);
    rightVector = glm::normalize(rightVector);

    glm::vec3 location = m_position - (rightVector * (m_width/2.0f)) + (upVector * (m_height/2.0f));
    m_drawingElement->m_vertices.push_back(location.x);
    m_drawingElement->m_vertices.push_back(location.y);
    m_drawingElement->m_vertices.push_back(location.z);
    std::vector<int>textureCoordinate = {0,1};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),textureCoordinate.begin(),textureCoordinate.end());

    location = m_position + (rightVector * (m_width/2.0f)) + (upVector * (m_height/2.0f));
    m_drawingElement->m_vertices.push_back(location.x);
    m_drawingElement->m_vertices.push_back(location.y);
    m_drawingElement->m_vertices.push_back(location.z);
    textureCoordinate = {1,1};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),textureCoordinate.begin(),textureCoordinate.end());

    location = m_position + (rightVector * (m_width/2.0f)) - (upVector * (m_height/2.0f));
    m_drawingElement->m_vertices.push_back(location.x);
    m_drawingElement->m_vertices.push_back(location.y);
    m_drawingElement->m_vertices.push_back(location.z);
    textureCoordinate = {1,0};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),textureCoordinate.begin(),textureCoordinate.end());

    location = m_position - (rightVector * (m_width/2.0f)) - (upVector * (m_height/2.0f));
    m_drawingElement->m_vertices.push_back(location.x);
    m_drawingElement->m_vertices.push_back(location.y);
    m_drawingElement->m_vertices.push_back(location.z);
    textureCoordinate = {0,0};
    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),textureCoordinate.begin(),textureCoordinate.end());



    m_drawingElement->m_indices = {
        0,1,3,
        2,1,3
    };


    m_drawingElement->setup();
}

void ImageOutlineManager::draw(glm::mat4 view,glm::mat4 projection) {
    m_drawingElement->m_shader.use();
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_drawingElement->m_shader.setInt("ourTexture", 0);
    m_drawingElement->draw(view,projection);
}