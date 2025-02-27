#ifndef FRUSTUM_TRACKER_H
#define FRUSTUM_TRACKER_H

#include <glm/gtc/matrix_transform.hpp>

class FrustumTracker{
public:
    FrustumTracker();
    ~FrustumTracker();

    void refreshFrustum();
    glm::mat4 getFrustum();
    glm::vec3 getEstimatedPosition();
    void movePosition(const glm::vec3 & delta);

private:
    const float physicalWidth = 0.53f;
    const float physicalHeight = 0.3f;

    glm::vec3 estimatedHeadPosition;
    glm::mat4 viewFrustum;
};

#endif