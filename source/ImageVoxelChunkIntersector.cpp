//
// Created by abhishek on 05/08/26.
//
#include "../include/ImageVoxelChunkIntersector.h"


ImageVoxelChunkIntersector::ImageVoxelChunkIntersector(VoxelManager *voxelManager, ImageOutlineManager *imageOutlineManager) {
    m_voxelManager = voxelManager;
    m_image = imageOutlineManager;
}

void ImageVoxelChunkIntersector::start(float step) {
    started = true;
    running = true;
    m_imageStep = step;
    m_imageStartPosition = m_image->m_position;
}

void ImageVoxelChunkIntersector::moveStep() {
    m_image->setPosition(m_image->m_position + (m_image->m_forwardVector * m_imageStep));
    for (int i = 0;i<m_image->m_height;i++) {
        for (int j=0;j<m_image->m_width;j++) {
            if (m_image->isPixelTransparent(i,j)) {
                glm::vec3 pixelPosition = m_image->getImagePixel3dPosition(i,j);

                glm::vec3 voxelLocation = m_voxelManager->checkVoxel(pixelPosition);
                // std::cout<<"switching off the voxel "<<voxelLocation<<std::endl;

                // if (once == false) {
                //     std::cout<<"i "<<i<<" "<<j<<std::endl;
                //     std::cout<<"pixel position "<<pixelPosition.x<<","<<pixelPosition.y<<","<<pixelPosition.z<<std::endl;
                //     std::cout<<"switching off the voxel "<<voxelLocation.x<<","<<voxelLocation.y<<","<<voxelLocation.z<<std::endl;
                //
                // }

                m_voxelManager->switchOffVoxel(voxelLocation);
            }

        }
    }
    once = true;

    float distance = glm::distance(m_imageStartPosition,m_image->m_position);
    float m_voxelDiagonal = (m_voxelManager->m_width * m_voxelManager->m_width) + (m_voxelManager->m_length * m_voxelManager->m_length);
    // std::cout<<"distance "<<distance<<" "<<(distance*distance)<<std::endl;
    if ((distance * distance) > (m_voxelDiagonal * 2.5)) {
        done = true;
        std::cout<<"final position image "<<m_image->m_position.x<<","<<m_image->m_position.y<<","<<m_image->m_position.z<<std::endl;
    }

}


void ImageVoxelChunkIntersector::perspectiveIntersection() {

    // glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
    //
    // glm::mat4 projection = glm::mat4(1.0f);
    //
    // projection = glm::perspective(glm::radians(fov),800.0f/600.0f,0.01f,2000.f);
    glm::vec3 cameraPos = m_image->m_position;//(m_voxelManager->m_position  - (80.0f * m_image->m_forwardVector));
    cameraPos.y = m_image->m_position.y;
    glm::vec3 cameraFront = cameraPos + m_image->m_forwardVector;

    std::cout<<"camera pos "<<cameraPos.x<<","<<cameraPos.y<<","<<cameraPos.z<<std::endl;

    std::cout<<"fx "<<(fx/m_image->m_ratio)<<" fy "<<(fy/m_image->m_ratio)<<std::endl;

    float cameraFocalLegth = 24.0f;
    glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    auto &voxels = m_voxelManager->m_voxels;
    for (int i = 0 ; i < voxels.size(); i++) {
        for (int j = 0;j < voxels[0].size(); j++) {
            for (int k = 0;k < voxels[0][0].size(); k++) {
                glm::vec3 voxelPosition = m_voxelManager->getVoxelPosition(i,j,k);
                glm::vec3 cameraSpaceVoxelPosition = view * glm::vec4(voxelPosition,1.0f);

                // 1. If the voxel is behind the camera (OpenGL Z > 0 is behind), turn it off and skip.
                if (cameraSpaceVoxelPosition.z >= 0.0f) {
                    m_voxelManager->switchOffVoxel(glm::vec3(i,j,k));
                    continue;
                }



                float pX = (fx * (cameraSpaceVoxelPosition.x / (1.0f * cameraSpaceVoxelPosition.z))) + cx;
                pX = pX / m_image->m_ratio;

                float pY = (fy * (cameraSpaceVoxelPosition.y / (1.0f * cameraSpaceVoxelPosition.z))) + cy;
                pY = pY / m_image->m_ratio;

                if (((pY >= 0) && (pY < m_image->m_height)) && ((pX >= 0) && (pX < m_image->m_width))) {
                    if (m_image->isPixelTransparent((int)pY,(int)pX)) {
                        m_voxelManager->switchOffVoxel(glm::vec3(i,j,k));
                    }
                }else {
                    m_voxelManager->switchOffVoxel(glm::vec3(i,j,k));
                }

            }
        }
    }
}

