//
// Created by abhishek on 02/08/26.
//

#include "../include/VoxelManager.h"


VoxelManager::VoxelManager(int length,int width,int height,float sideLength,std::string texturePath , Shader shader, glm::vec3 position) {
    m_length = length;
    m_width = width;
    m_height = height;
    m_sideLength = sideLength;
    m_position = position;
    m_voxels.resize(length);
    for (auto &it: m_voxels) {
        it.resize(height);
        for (auto &it1: it) {
            it1.resize(width);
            for (int i = 0; i < it1.size(); i++) {
                it1[i] = true;//(rand() % 2) ? true : false;
            }
        }
    }

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

void VoxelManager::setupChunk() {

    m_drawingElement->setType(DrawingElement::DrawElementType::TRIANGLES);
    m_drawingElement->setAttributeArray({
    {3,true},
    {2,true}
    });
    m_drawingElement->m_stride = 5;
    m_drawingElement->m_vertices.clear();
    m_drawingElement->m_indices.clear();
    float xoffset = m_position.x - ((float)m_width/2.0f);
    float zoffset = m_position.z - ((float)m_length/2.0f);
    int lastIndex = 0;
    for (int i = 0 ;i<m_voxels.size(); i++) {
        for (int j = 0; j < m_voxels[i].size(); j++) {
            for (int k = 0; k < m_voxels[i][j].size(); k++) {
                if (m_voxels[i][j][k]==true) {
                    // std::cout<<"i "<<i<<" j "<<j<<" k "<<k<<std::endl;
                    if ( ((k+1) >= m_voxels[i][j].size()) ||  (m_voxels[i][j][k+1]==false)) {

                        // *** + m_position.x - ((int)m_width/2.0)
                        float firstCornerx = ((float)i*(float)(m_sideLength)) + xoffset;
                        float firstCornery = ((float)j*(float)(m_sideLength)) ;
                        float firstCornerz = ((float)(k+1) * (float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[1] = location[1] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());


                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }
                    if ( ((k-1) < 0) || (m_voxels[i][j][k-1]==false)) {

                        float firstCornerx = ((float)i*(float)(m_sideLength)) + xoffset;
                        float firstCornery = (float)j*(float)(m_sideLength) ;
                        float firstCornerz = ((float)k * (float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};
                        // std::vector<float> location = {(float)i*(float)(m_sideLength),(float)j*(float)(m_sideLength),(float)k * (float)(m_sideLength), 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[1] = location[1] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());

                        // std::cout<<"last index "<<lastIndex<<std::endl;

                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }
                    if (((j+1) >= m_voxels[i].size()) || (m_voxels[i][j+1][k] == false) ) {

                        float firstCornerx = ((float)i*(float)(m_sideLength)) + xoffset;
                        float firstCornery = (float)(j + 1) * (float)(m_sideLength) ;
                        float firstCornerz = ((float)k*(float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};

                        // std::vector<float> location = {(float)i*(float)(m_sideLength),(float)(j + 1) * (float)(m_sideLength),(float)k*(float)(m_sideLength), 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());

                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }
                    if (((j-1) < 0) || (m_voxels[i][j-1][k] == false)  ) {

                        float firstCornerx = ((float)i*(float)(m_sideLength)) + xoffset;
                        float firstCornery = (float)j * (float)(m_sideLength) ;
                        float firstCornerz = ((float)k*(float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};

                        // std::vector<float> location = {(float)i*(float)(m_sideLength),(float)j * (float)(m_sideLength),(float)k*(float)(m_sideLength), 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[0] = location[0] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());

                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }
                    if (((i+1) >= m_voxels.size()) ||  (m_voxels[i+1][j][k] == false) ) {

                        float firstCornerx = ((float)(i + 1) * (float)(m_sideLength)) + xoffset;
                        float firstCornery = (float)j*(float)(m_sideLength) ;
                        float firstCornerz = ((float)k*(float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};

                        // std::vector<float> location = {(float)(i + 1) * (float)(m_sideLength),(float)j*(float)(m_sideLength),(float)k*(float)(m_sideLength), 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[1] = location[1] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());

                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }
                    if ( ((i-1) < 0) ||  (m_voxels[i-1][j][k] == false)) {

                        float firstCornerx = ((float)i * (float)(m_sideLength)) + xoffset;
                        float firstCornery = (float)j*(float)(m_sideLength) ;
                        float firstCornerz = ((float)k*(float)(m_sideLength)) + zoffset;
                        std::vector<float> location = {firstCornerx,firstCornery,firstCornerz, 1.0f, 1.0f};

                        // std::vector<float> location = {(float)i * (float)(m_sideLength),(float)j*(float)(m_sideLength),(float)k*(float)(m_sideLength), 1.0f, 1.0f};
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] + (float)m_sideLength;
                        location[4] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[1] = location[1] + (float)m_sideLength;
                        location[3] = 0.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());
                        location[2] = location[2] - (float)m_sideLength;
                        location[4] = 1.0f;
                        m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),location.begin(),location.end());

                        std::vector<int>indicesAppendArray = {lastIndex, lastIndex+1, lastIndex+3, lastIndex+1, lastIndex+2, lastIndex+3};
                        m_drawingElement->m_indices.insert(m_drawingElement->m_indices.end(),indicesAppendArray.begin(),indicesAppendArray.end());
                        lastIndex += 4;
                    }

                }
            }
        }
    }



    m_drawingElement->setup();

    std::cout<<"chunk setup done"<<std::endl;
}


void VoxelManager::applyMarchingCube() {
    m_drawingElement->setType(DrawingElement::DrawElementType::TRIANGLES);
    m_drawingElement->setAttributeArray({
    {3,true},
    {2,true}
    });
    m_drawingElement->m_stride = 5;
    m_drawingElement->m_vertices.clear();
    m_drawingElement->m_indices.clear();

    int edgeLastIndex = 0;

    for (int i = 0;i<m_voxels.size();i++) {
        for (int j = 0;j<m_voxels[i].size();j++) {
            for (int k = 0;k<m_voxels[i][j].size();k++) {
                int cubeIndex = getMarchingCubeIndex(i,j,k);

                int edges = edgeTable[cubeIndex];

                std::vector<int> edgesList = getEdgesIndex(edges);
                for (int edgeIndex = 0;edgeIndex<edgesList.size();edgeIndex++) {
                    glm::vec3 location = getEdgeLocation(i,j,k,edgesList[edgeIndex]);
                    int xTexture = (rand() % 2),yTexture = (rand() % 2);
                    std::vector<float> vertexArray = {location.x,location.y,location.z,(float)xTexture,(float)yTexture};
                    m_drawingElement->m_vertices.insert(m_drawingElement->m_vertices.end(),vertexArray.begin(),vertexArray.end());
                }

                int *triValue = triTable[cubeIndex];
                for (int itr = 0;itr<16;itr++) {
                    if (triValue[itr] != -1) {
                        auto it = std::find(edgesList.begin(), edgesList.end(), triValue[itr]);
                        int listIndex = it - edgesList.begin();
                        m_drawingElement->m_indices.push_back(edgeLastIndex + listIndex);
                    }
                }
                edgeLastIndex += edgesList.size();

            }
        }
    }

    m_drawingElement->setup();

    std::cout<<"marching cube chunk setup done"<<std::endl;

}

bool VoxelManager::getVoxelValue(int x, int y, int z) {
    if ((x >= 0) && (x < m_voxels.size()) && ((y >= 0) && (y < m_voxels[0].size())) && ((z >= 0) && (z < m_voxels[0][0].size()))) {
        return m_voxels[x][y][z];
    }
    return false;
}

glm::vec3 VoxelManager::getVoxelPosition(int x, int y, int z) {
    glm::vec3 cornerPosition = m_position - glm::vec3((float)m_width/2.0f,0,(float)m_length/2.0f);
    glm::vec3 voxelPosition = cornerPosition + glm::vec3(x * m_sideLength,y * m_sideLength, z * m_sideLength);
    voxelPosition += glm::vec3((float)m_sideLength/2.0f,(float)m_sideLength/2.0f,(float)m_sideLength/2.0f);
    return voxelPosition;
}

glm::vec3 VoxelManager::getEdgeLocation(int x, int y, int z,int index) {
    glm::vec3 voxelPosition = getVoxelPosition(x,y,z);
    if (index == 0) {
        voxelPosition += glm::vec3(m_sideLength/2.0f,0,0);
    }
    else if (index == 1) {
        voxelPosition += glm::vec3(m_sideLength,0,m_sideLength/2.0f);
    }
    else if (index == 2) {
        voxelPosition += glm::vec3(m_sideLength/2.0,0,m_sideLength);
    }
    else if (index == 3) {
        voxelPosition += glm::vec3(0,0,m_sideLength/2.0f);
    }
    else if (index == 4) {
        voxelPosition += glm::vec3(m_sideLength/2.0f,m_sideLength,0);
    }
    else if (index == 5) {
        voxelPosition += glm::vec3(m_sideLength,m_sideLength,m_sideLength/2.0f);
    }
    else if (index == 6) {
        voxelPosition += glm::vec3(m_sideLength/2.0f,m_sideLength,m_sideLength);
    }
    else if (index == 7) {
        voxelPosition += glm::vec3(0,m_sideLength,m_sideLength/2.0f);
    }
    else if (index == 8) {
        voxelPosition += glm::vec3(0,m_sideLength/2.0f,0);
    }
    else if (index == 9) {
        voxelPosition += glm::vec3(m_sideLength,m_sideLength/2.0f,0);
    }
    else if (index == 10) {
        voxelPosition += glm::vec3(m_sideLength,m_sideLength/2.0f,m_sideLength);
    }
    else if (index == 11) {
        voxelPosition += glm::vec3(0,m_sideLength/2.0f,m_sideLength);
    }

    return voxelPosition;

}


std::vector<int> VoxelManager::getEdgesIndex(int value) {
    std::vector<int> edges;
    int index = 0;
    while (value) {
        if ((value%2) == 1) {
            edges.push_back(index);
        }
        value = value/2;
        index++;
    }
    return edges;
}

int VoxelManager::getMarchingCubeIndex(int x, int y, int z) {
    int indexValue = 0;
    if (getVoxelValue(x,y,z) == true) { // 0
        indexValue |= 1;
    }
    if (getVoxelValue(x+1,y,z) == true) { // 1
        indexValue |= 2;
    }
    if (getVoxelValue(x+1,y,z+1) == true) { // 2
        indexValue |= 4;
    }
    if (getVoxelValue(x,y,z+1) == true) { // 3
        indexValue |= 8;
    }
    if (getVoxelValue(x,y+1,z) == true) { // 4
        indexValue |= 16;
    }
    if (getVoxelValue(x+1,y+1,z) == true) { // 5
        indexValue |= 32;
    }
    if (getVoxelValue(x+1,y+1,z+1) == true) { // 6
        indexValue |= 64;
    }
    if (getVoxelValue(x,y+1,z+1) == true) { // 7
        indexValue |= 128;
    }
    return indexValue;
}

void VoxelManager::draw(glm::mat4 view,glm::mat4 projection) {
    m_drawingElement->m_shader.use();
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_drawingElement->m_shader.setInt("ourTexture", 0);
    m_drawingElement->draw(view,projection);
}

glm::vec3 VoxelManager::checkVoxel(glm::vec3 location) {
    glm::vec3 offset = glm::vec3((float)m_width/2.0f,0,(float)m_length/2.0f);
    glm::vec3 corner = m_position - offset;
    location = location - corner;
    int xVoxel = location.x / m_sideLength;
    int yVoxel = location.y / m_sideLength;
    int zVoxel = location.z / m_sideLength;

    // std::cout<<"for location "<<location.x<<","<<location.y<<","<<location.z<<std::endl;
    // std::cout<<"voxel "<<xVoxel<<","<<yVoxel<<","<<zVoxel<<std::endl;
    return glm::vec3(xVoxel,yVoxel,zVoxel);


}


void VoxelManager::switchOffVoxel(glm::vec3 location) {
    if (((location.x >= 0) && (location.x < m_voxels.size())) && (location.y >= 0) && (location.y < m_voxels[0].size()) && (location.z >= 0) && (location.z < m_voxels[0][0].size())) {
        m_voxels[location.x][location.y][location.z] = false;
    }
}

void VoxelManager::switchOnVoxel(glm::vec3 location) {
    if (((location.x >= 0) && (location.x < m_voxels.size())) && (location.y >= 0) && (location.y < m_voxels[0].size()) && (location.z >= 0) && (location.z < m_voxels[0][0].size())) {
        m_voxels[location.x][location.y][location.z] = true;
    }
}