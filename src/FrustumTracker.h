#ifndef FRUSTUM_TRACKER_H
#define FRUSTUM_TRACKER_H

#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

class FrustumTracker{
public:
    FrustumTracker();
    ~FrustumTracker();

    void refreshFrustum();
    glm::mat4 getFrustum();
    glm::vec3 getEstimatedPosition();
    void movePosition(const glm::vec3 & delta);
    cv::Mat & getFrame() {return frame;}

private:
    const float physicalWidth = 0.53f;
    const float physicalHeight = 0.3f;

    glm::vec3 estimatedHeadPosition;
    glm::mat4 viewFrustum;

    cv::Mat frame;
    cv::VideoCapture cap;

    const int deviceID = 0;             // 0 = open default camera
    const int apiID = cv::CAP_ANY;      // 0 = autodetect default API
};

#endif