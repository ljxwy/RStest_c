#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core/mat.hpp>
#include <cv.hpp>
using namespace cv;

void enable_stream_init(rs2::config cfg){
//    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 90);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16,90);
    cfg.enable_stream(RS2_STREAM_INFRARED, 1, 640, 480, RS2_FORMAT_Y8, 90);
    cfg.enable_stream(RS2_STREAM_INFRARED, 2, 640, 480, RS2_FORMAT_Y8, 90);
}

void enableEmitter(rs2::pipeline_profile selection) {
    rs2::device selected_device = selection.get_device();
    auto depth_sensor = selected_device.first<rs2::depth_sensor>();
    if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
    {
//        depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1.f); // Enable emitter //实践证明这并不能开启
        depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f); // Disable emitter // 关闭倒是有用，下面的关闭也有用
    }
    if (depth_sensor.supports(RS2_OPTION_LASER_POWER))
    {
        // Query min and max values:
        auto range = depth_sensor.get_option_range(RS2_OPTION_LASER_POWER);
//        depth_sensor.set_option(RS2_OPTION_LASER_POWER, range.max); // Set max power //这个才能开启，开启之后设备会记得这个设定，关闭同理。
        depth_sensor.set_option(RS2_OPTION_LASER_POWER, 0.f); // Disable laser
    }
}

void Get_imu_data(rs2::frameset frames)
{
        if (rs2::motion_frame accel_frame = frames.first_or_default(RS2_STREAM_ACCEL))
        {
            rs2_vector accel_sample = accel_frame.get_motion_data();
            std::cout << "Accel:" << accel_sample.x << ", " << accel_sample.y << ", " << accel_sample.z << std::endl;
        }
        if (rs2::motion_frame gyro_frame = frames.first_or_default(RS2_STREAM_GYRO))
        {
            rs2_vector gyro_sample = gyro_frame.get_motion_data();
            std::cout << "Gyro:" << gyro_sample.x << ", " << gyro_sample.y << ", " << gyro_sample.z << std::endl;
        }
}


int main() {
//    std::cout << "Hello, World!" << std::endl;
    rs2::context ctx;
    auto list = ctx.query_devices();
    if (list.size() == 0)
        throw std::runtime_error("No device detected. Is it plugged in?");

    rs2::pipeline p;
    rs2::frameset frames;
    rs2::config cfg;
    enable_stream_init(cfg);
    rs2::pipeline_profile selection = p.start(cfg);
    enableEmitter(selection);

    while(list.size()) {
        frames = p.wait_for_frames();
//        rs2::align align_to_color(RS2_STREAM_COLOR);
//        frames = align_to_color.process(frames);
//          Get_imu_data(frames);
//        rs2::frame color_frame = frames.get_color_frame();
        rs2::depth_frame depth_frame = frames.get_depth_frame();
        rs2::video_frame ir_frame_left = frames.get_infrared_frame(1);
        rs2::video_frame ir_frame_right = frames.get_infrared_frame(2);

        // Creating OpenCV Matrix from a color image
//        Mat color(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        Mat left(Size(640,480), CV_8UC1, (void*)ir_frame_left.get_data());
        Mat right(Size(640,480), CV_8UC1, (void*)ir_frame_right.get_data());
        Mat depth(Size(640,480), CV_16U, (void*)depth_frame.get_data(), Mat::AUTO_STEP);

//        cv::imshow("color",color);
        cv::imshow("left",left);
        cv::imshow("right",right);
        cv::imshow("depth",depth);
        cv::waitKey(1);

    }
    return 0;

}


