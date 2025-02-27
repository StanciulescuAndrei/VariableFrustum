#include "FrustumTracker.h"

FrustumTracker::FrustumTracker()
{
    estimatedHeadPosition = glm::vec3(0.0f, 0.0f, 0.7f);
}

FrustumTracker::~FrustumTracker()
{
}

void FrustumTracker::refreshFrustum()
{
    viewFrustum = glm::frustum(-physicalWidth / 2 - estimatedHeadPosition.x,
                                physicalWidth / 2 - estimatedHeadPosition.x,
                                -physicalHeight / 2 - estimatedHeadPosition.y,
                                physicalHeight / 2 - estimatedHeadPosition.y,
                                estimatedHeadPosition.z, 100.0f);
}

glm::mat4 FrustumTracker::getFrustum()
{
    return viewFrustum;
}

glm::vec3 FrustumTracker::getEstimatedPosition()
{
    return estimatedHeadPosition;
}

void FrustumTracker::movePosition(const glm::vec3 &delta)
{
    estimatedHeadPosition += delta;
}
