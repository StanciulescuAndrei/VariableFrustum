#include "FrustumTracker.h"

FrustumTracker::FrustumTracker()
{
    estimated_head_position = glm::vec3(0.0f, 0.0f, 0.7f);
    
    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, frame_resolution.x);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, frame_resolution.y);

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
    }

    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

    K = (cv::Mat_<float>(3, 3) << 992.2142f, 0.0f, 640.0f, 0.0f, 1019.578f, 360.0f, 0.0f, 0.0f, 1.0f);
    K_inv = K.inv();

    std::cout<<K<<std::endl<<K_inv<<std::endl;

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

    glm::vec2 position_delta(0.0f);
    estimated_head_position = glm::vec3(0.0f, 0.0f, 0.7f);

    if(dets.size() > 0){
        dlib::full_object_detection landmarks = pose_model(dlibImage, dets[0]);
        uint16_t num_parts = landmarks.num_parts();
        for(int i = 0; i < num_parts; i++){
            dlib::point & feature_point = landmarks.part(i);
            cv::circle(frame, cv::Point(feature_point.x(), feature_point.y()), 1, cv::Scalar(0, 0, 255));
        }

        // Determine eye centers
        glm::vec2 eye_center_left(0.0f);
        glm::vec2 eye_center_right(0.0f);
        
        // Left eye 
        for(int i = 36; i < 42; i++){
            eye_center_left += glm::vec2(landmarks.part(i).x(), landmarks.part(i).y());
        }
        eye_center_left /= 6.0f;

        // Right eye 
        for(int i = 42; i < 48; i++){
            eye_center_right += glm::vec2(landmarks.part(i).x(), landmarks.part(i).y());
        }
        eye_center_right /= 6.0f;

        cv::circle(frame, cv::Point(eye_center_left.x, eye_center_left.y), 3, cv::Scalar(0, 255, 255));
        cv::circle(frame, cv::Point(eye_center_right.x, eye_center_right.y), 3, cv::Scalar(0, 255, 255));

        float measured_iod = glm::length(eye_center_left - eye_center_right);
        float head_distance = K.at<float>(0, 0) * intraocular_distance / measured_iod;

        std::cout<<head_distance * 100.0f <<std::endl;

        position_delta = (eye_center_left + eye_center_right) / 2.0f - glm::vec2(frame_resolution) / 2.0f;
        position_delta.x /= 3200.0f;
        position_delta.y /= 2400.0f;
    }

    estimated_head_position.x -= position_delta.x;
    estimated_head_position.y += position_delta.y;

    

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    viewFrustum = glm::frustum(-physical_width / 2 - estimated_head_position.x,
                                physical_width / 2 - estimated_head_position.x,
                                -physical_height / 2 - estimated_head_position.y,
                                physical_height / 2 - estimated_head_position.y,
                                estimated_head_position.z, 100.0f);
}

glm::mat4 FrustumTracker::getFrustum()
{
    return viewFrustum;
}

glm::vec3 FrustumTracker::getEstimatedPosition()
{
    return estimated_head_position;
}

void FrustumTracker::movePosition(const glm::vec3 &delta)
{
    estimated_head_position += delta;
}
