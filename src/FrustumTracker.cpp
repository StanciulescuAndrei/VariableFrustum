#include "FrustumTracker.h"

FrustumTracker::FrustumTracker()
{
    estimated_head_position = glm::vec3(0.0f, 0.0f, 0.7f);
    
    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, frame_resolution.x);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, frame_resolution.y);
    cap.set(cv::CAP_PROP_FPS, 30);

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
    }

    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

    K = glm::mat3(992.2142f, 0.0f, 0.0f, 0.0f, 1019.578f, 0.0f, 640.0f, 360.0f, 1.0f);
    K_inv = glm::inverse(K);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << K[j][i] << " ";
        }
        std::cout << "\n";
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << K_inv[j][i] << " ";
        }
        std::cout << "\n";
    }

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

    cv::Mat downsampled;
    cv::resize(frame, downsampled, cv::Size(), 0.5f, 0.5f);

    dlib::matrix<dlib::bgr_pixel> dlibImage;
    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(downsampled));

    std::vector<dlib::rectangle> dets = detector(dlibImage);

    glm::vec2 position_delta(0.0f);
    estimated_head_position = glm::vec3(0.0f, 0.0f, 0.7f);

    if(dets.size() > 0){
        dlib::full_object_detection landmarks = pose_model(dlibImage, dets[0]);
        uint16_t num_parts = landmarks.num_parts();
        for(int i = 0; i < num_parts; i++){
            dlib::point & feature_point = landmarks.part(i);
            feature_point *= 2; // Account for downscaling
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
        float head_distance = K[0][0] * intraocular_distance / measured_iod;

        glm::vec2 head_center = (eye_center_left + eye_center_right) / 2.0f;

        glm::vec3 screen_pos = glm::vec3(head_center.x, head_center.y, 1.0f);

        glm::vec3 world_pos = K_inv * screen_pos * head_distance;

        estimated_head_position.z = head_distance;
        position_delta.x = world_pos.x;
        position_delta.y = world_pos.y;
    }

    estimated_head_position.x -= position_delta.x;
    estimated_head_position.y += position_delta.y;

    std::cout<< estimated_head_position.x << " "<< estimated_head_position.y<<" "<<estimated_head_position.z<<std::endl;

    

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
