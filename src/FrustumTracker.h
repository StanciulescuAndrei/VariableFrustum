#ifndef FRUSTUM_TRACKER_H
#define FRUSTUM_TRACKER_H

#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>

#include <iostream>
#include <vector>

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

    dlib::frontal_face_detector detector;
    dlib::shape_predictor pose_model;


    const int deviceID = 0;             // 0 = open default camera
    const int apiID = cv::CAP_ANY;      // 0 = autodetect default API
};

#endif