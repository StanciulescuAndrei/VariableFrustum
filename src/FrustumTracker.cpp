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

    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

}

FrustumTracker::~FrustumTracker()
{
}

void FrustumTracker::refreshFrustum()
{
    cap.read(frame);
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }

    dlib::array2d<dlib::bgr_pixel> dlibImage;
    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(frame));

    std::vector<dlib::rectangle> dets = detector(dlibImage);

    if(dets.size() > 0){
        dlib::full_object_detection landmarks = pose_model(dlibImage, dets[0]);
        uint16_t num_parts = landmarks.num_parts();
        for(int i = 0; i < num_parts; i++){
            dlib::point & feature_point = landmarks.part(i);
            cv::circle(frame, cv::Point(feature_point.x(), feature_point.y()), 1, cv::Scalar(0, 0, 255));
        }
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
