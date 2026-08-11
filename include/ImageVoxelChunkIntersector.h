//
// Created by abhishek on 05/08/26.
//

#ifndef SHAPERECONSTRUCTOR_IMAGEVOXELCHUNKINTERSECTOR_H
#define SHAPERECONSTRUCTOR_IMAGEVOXELCHUNKINTERSECTOR_H



#include "../include/VoxelManager.h"
#include "../include/ImageOutlineManager.h"


class ImageVoxelChunkIntersector {

    public:

    VoxelManager *m_voxelManager;
    ImageOutlineManager *m_image;

    float fx = 3022.566;
    float fy = 2999.203;
    float cx = 1573.326;
    float cy = 2085.685;

    bool started = false, running = false,done = false,once = false;

    float m_imageStep = 0.0f;
    glm::vec3 m_imageStartPosition ;


    ImageVoxelChunkIntersector(VoxelManager *voxelManager, ImageOutlineManager *imageOutlineManager);

    bool isStarted() {return started;}
    bool isRunning() {return running;}
    bool isDone() {return done;}

    void start(float step);
    void moveStep();

    void perspectiveIntersection();


};


#endif //SHAPERECONSTRUCTOR_IMAGEVOXELCHUNKINTERSECTOR_H
