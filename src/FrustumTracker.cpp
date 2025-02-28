#include "FrustumTracker.h"

FrustumTracker::FrustumTracker()
{
    estimatedHeadPosition = glm::vec3(0.0f, 0.0f, 0.7f);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cap.open(deviceID, apiID);

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
    }
}

FrustumTracker::~FrustumTracker()
{
}

void FrustumTracker::refreshFrustum()
{
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

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
