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
    const cv::Mat & getFrame() {return frame;}
    const glm::ivec2 & getFrameResolution() {return frame_resolution;}

private:
    const float physical_width = 0.53f;
    const float physical_height = 0.3f;

    glm::vec3 estimated_head_position;
    glm::mat4 viewFrustum;

    cv::Mat frame;
    cv::VideoCapture cap;
    glm::ivec2 frame_resolution{640, 480};

    dlib::frontal_face_detector detector;
    dlib::shape_predictor pose_model;


    const int deviceID = 0;             // 0 = open default camera
    const int apiID = cv::CAP_ANY;      // 0 = autodetect default API
};

#endif