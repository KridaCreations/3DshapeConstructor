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
    if ((distance * distance) > (m_voxelDiagonal * 2.0f * 2.0f)) {
        done = true;
        std::cout<<"final position image "<<m_image->m_position.x<<","<<m_image->m_position.y<<","<<m_image->m_position.z<<std::endl;
    }

}


