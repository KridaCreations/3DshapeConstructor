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

    // m_voxelsNormals.resize(length);
    // for (int i = 0;i<m_voxelsNormals.size();i++) {
    //     m_voxelsNormals[i].resize(height);
    //     for (int j = 0;j<m_voxelsNormals[i].size();j++) {
    //         m_voxelsNormals[i][j].resize(width);
    //         for (int k = 0;k<m_voxelsNormals[i][j].size();k++) {
    //             int x = (int)getVoxelValue(i+1,j,k) - (int)getVoxelValue(i-1,j,k);
    //             int y = (int)getVoxelValue(i,j+1,k) - (int)getVoxelValue(i,j-1,k);
    //             int z = (int)getVoxelValue(i,j,k+1) - (int)getVoxelValue(i,j,k-1);
    //             glm::vec3 normal = glm::vec3(x,y,z);
    //             normal = glm::normalize(normal);
    //             m_voxelsNormals[i][j][k] = normal;
    //         }
    //     }
    // }


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
    {3,true}
    });
    m_drawingElement->m_stride = 6;
    m_drawingElement->m_vertices.clear();
    m_drawingElement->m_indices.clear();

    std::unordered_map<uint64_t,uint64_t> edgeToVertex;
    std::vector<std::pair<glm::vec3,glm::vec3>> verticesData;


    for (int i = -1;i<(int)m_voxels.size();i++) {
        for (int j = -1;j<(int)m_voxels[0].size();j++) {
            for (int k = -1;k<(int)m_voxels[0][0].size();k++) {
                int cubeIndex = getMarchingCubeIndex(i,j,k);

                int edges = edgeTable[cubeIndex];

                std::vector<int> edgesList = getEdgesIndex(edges);
                std::vector<int> mapIndex;
                for (int edgeItrIndex = 0;edgeItrIndex<edgesList.size();edgeItrIndex++) {
                    glm::vec3 location = getEdgeLocation(i,j,k,edgesList[edgeItrIndex]);
                    uint64_t edgeKey = getEdgeKey(location);
                    int edgeIndex = 0;
                    if (edgeToVertex.count(edgeKey) == 0) {
                        edgeIndex = verticesData.size();
                        edgeToVertex[edgeKey] = edgeIndex;
                        verticesData.push_back({location,glm::vec3(0.0f,0.0f,0.0f)});
                    }
                    else {
                        edgeIndex = edgeToVertex[edgeKey];
                    }
                    mapIndex.push_back(edgeIndex);
                }

                int *triValue = triTable[cubeIndex];
                for (int itr = 0;itr<16;itr++) {
                    if (triValue[itr] != -1) {
                        auto it = std::find(edgesList.begin(), edgesList.end(), triValue[itr]);
                        int listIndex = it - edgesList.begin();
                        m_drawingElement->m_indices.push_back(mapIndex[listIndex]);
                        // int edgeIndex = triValue[itr];
                        // glm::vec3 edgeLocation = getEdgeLocation(i,j,k,edgeIndex);
                        // uint64_t edgeKey = getEdgeKey(edgeLocation);
                        // if (edgeToVertex.count(edgeKey) > 0) {
                        //     m_drawingElement->m_indices.push_back(edgeToVertex[edgeKey]);
                        // }
                    }
                }
            }
        }
    }

    for (int i = 0;i<m_drawingElement->m_indices.size();i+=3) {
        int i1 = m_drawingElement->m_indices[i];
        int i2 = m_drawingElement->m_indices[i+1];
        int i3 = m_drawingElement->m_indices[i+2];
        //
        // std::pair<glm::vec3,glm::vec3> vertexInfo1 = verticesData[index1];
        // std::pair<glm::vec3,glm::vec3> vertexInfo2 = verticesData[index2];
        // std::pair<glm::vec3,glm::vec3> vertexInfo3 = verticesData[index3];

        glm::vec3 n = glm::normalize(glm::cross(
           verticesData[i2].first - verticesData[i1].first,
           verticesData[i3].first - verticesData[i1].first));

        verticesData[i1].second += n;
        verticesData[i2].second += n;
        verticesData[i3].second += n;
    }

    for (auto &it:verticesData) {
        it.second = glm::normalize(it.second);
    }

    for (int i = 0;i<verticesData.size();i++) {
        m_drawingElement->m_vertices.push_back(verticesData[i].first.x);
        m_drawingElement->m_vertices.push_back(verticesData[i].first.y);
        m_drawingElement->m_vertices.push_back(verticesData[i].first.z);
        m_drawingElement->m_vertices.push_back(verticesData[i].second.x);
        m_drawingElement->m_vertices.push_back(verticesData[i].second.y);
        m_drawingElement->m_vertices.push_back(verticesData[i].second.z);
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
    glm::vec3 cornerPosition = m_position - glm::vec3((float)(m_width * m_sideLength)/2.0f,0,(float)(m_length * m_sideLength)/2.0f);
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

uint64_t VoxelManager::getEdgeKey(glm::vec3 edge) {
    // uint64_t maxLen = 3000;//std::max(m_length,std::max(m_width,m_length));
    // uint64_t factor = maxLen;
    // uint64_t key = (uint64_t)((edge.x * 10)+ maxLen);
    // key = ((uint64_t)((edge.y * 10) + maxLen) * factor) + key;
    //
    // factor = factor * factor;
    // key = ((uint64_t)((edge.z*10) + maxLen) * factor) + key;
    // return key;


    int64_t maxLen = 3000;
    int64_t factor = 6000;

    // Add maxLen before converting to uint64_t to prevent negative underflow
    uint64_t key = (uint64_t)((int64_t)std::round(edge.x * 10.0f) + maxLen);
    key = ((uint64_t)((int64_t)std::round(edge.y * 10.0f) + maxLen) * factor) + key;

    uint64_t factor2 = factor * factor;
    key = ((uint64_t)((int64_t)std::round(edge.z * 10.0f) + maxLen) * factor2) + key;

    return key;


    // int32_t maxDist = 1000;
    // int32_t factor = maxDist * 2;
    // int32_t key = ((int32_t)std::round(edge.x) + maxDist);
    // key = (((int32_t)std::round(edge.y) + maxDist) * factor) + key;
    // factor = factor * factor;
    //
    // key = (((int32_t)std::round(edge.z) + maxDist) * factor) + key;
    // return key;
}

glm::vec3 VoxelManager::getVoxelNormal(int x, int y, int z) {
    if ((x >= 0) && (x < m_voxelsNormals.size()) && (y >= 0) && (y < m_voxelsNormals[0].size()) && (z >= 0) && (z < m_voxelsNormals[0][0].size())) {
        return m_voxelsNormals[x][y][z];
    }
    return glm::vec3(0,0,0);
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
    m_drawingElement->m_shader.setVec3("lightSource", glm::vec3(100.0,100.0,100.0));
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