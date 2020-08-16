#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core/mat.hpp>
#include <cv.hpp>
using namespace cv;

void enable_stream_init(rs2::config cfg){
//    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);//向配置添加所需的流
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16,30);
    cfg.enable_stream(RS2_STREAM_INFRARED, 1, 640, 480, RS2_FORMAT_Y8, 30);
    cfg.enable_stream(RS2_STREAM_INFRARED, 2, 640, 480, RS2_FORMAT_Y8, 30);
}
int main() {
    std::cout << "Hello, World!" << std::endl;

    rs2::pipeline p;

    rs2::frameset frames;
    rs2::config cfg;
    enable_stream_init(cfg);
//    p.start(cfg);
    rs2::pipeline_profile selection = p.start(cfg);

    rs2::device selected_device = selection.get_device();
    auto depth_sensor = selected_device.first<rs2::depth_sensor>();
    if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
    {
//        depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1.f); // Enable emitter //实践证明这并不能开启
//        depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f); // Disable emitter // 关闭倒是有用，下面的关闭也有用
    }
    if (depth_sensor.supports(RS2_OPTION_LASER_POWER))
    {
        // Query min and max values:
        auto range = depth_sensor.get_option_range(RS2_OPTION_LASER_POWER);
//        depth_sensor.set_option(RS2_OPTION_LASER_POWER, range.max); // Set max power //这个才能开启，开启之后设备会记得这个设定，关闭同理。
        depth_sensor.set_option(RS2_OPTION_LASER_POWER, 0.f); // Disable laser
    }

    while(1) {
        frames = p.wait_for_frames();
//        rs2::align align_to_color(RS2_STREAM_COLOR);
//        frames = align_to_color.process(frames);
        // Get imu data
        //  Get_imu_data(frames);
//        rs2::frame color_frame = frames.get_color_frame();
        rs2::depth_frame depth_frame = frames.get_depth_frame();
        rs2::video_frame ir_frame_left = frames.get_infrared_frame(1);
        rs2::video_frame ir_frame_right = frames.get_infrared_frame(2);

        // Creating OpenCV Matrix from a color image
//        Mat color_mat(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        Mat left_mat(Size(640,480), CV_8UC1, (void*)ir_frame_left.get_data());
        Mat right_mat(Size(640,480), CV_8UC1, (void*)ir_frame_right.get_data());
        Mat depth_mat(Size(640,480), CV_16U, (void*)depth_frame.get_data(), Mat::AUTO_STEP);

//        cv::imshow("color_mat",color_mat);
        cv::imshow("left_mat",left_mat);
        cv::imshow("right_mat",right_mat);
        cv::imshow("depth_mat",depth_mat);
        cv::waitKey(1);


    }
    return 0;//如果这个return在while（） 里面，会导致只有 cv::waitKey(0);时才有一张图显示，而无法显示视频。

}
