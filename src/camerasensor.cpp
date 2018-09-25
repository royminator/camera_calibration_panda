#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "camerasensor.hpp"

////////////////////////////////////////////////////////////////////////////////
/*
static pcl::PointCloud<pcl::PointXYZ>::Ptr points_to_pcl(
    const rs2::points& points) {

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    auto sp = points.get_profile().as<rs2::video_stream_profile>();
    cloud->width = sp.width();
    cloud->height = sp.height();
    cloud->is_dense = false;
    cloud->points.resize(points.size());
    auto ptr = points.get_vertices();
    for (auto& p : cloud->points)
    {
        p.x = ptr->x;
        p.y = ptr->y;
        p.z = ptr->z;
        ptr++;
    }

    return cloud;
}
*/

////////////////////////////////////////////////////////////////////////////////
CameraSensor::CameraSensor() {

    res_x_ = 0; res_y_ = 0;

    SetExtrinsics(Eigen::Matrix4f::Identity());

    // Turn camera 180 degrees facing along the negative z axis
    // and move to (0.5, 0.5, 1.0) m
    extrinsics_(1, 1) = -1.0f;
    extrinsics_(2, 2) = -1.0f;
    extrinsics_(0, 3) = 0.5f;
    extrinsics_(1, 3) = 0.5f;
    extrinsics_(2, 3) = 1.5f;

    rs2::device dev = GetDevice("Intel");

    pipeline_ = rs2::pipeline();
    ActivateStream("ir_stream", &config_);

    pipeline_profile_ = pipeline_.start(config_);
    Warmup(&pipeline_, 50);

    if (StreamIsActive(RS2_STREAM_DEPTH, &pipeline_profile_)) {
        
		intrinsics_[RS2_STREAM_DEPTH] = GetIntrinsics("depth");
		dist_coeffs_[RS2_STREAM_DEPTH] = GetDistortionCoeffs("depth");

        auto sensor = pipeline_profile_.get_device().first<rs2::depth_sensor>();
        meter_scale_ = sensor.get_depth_scale();
    }

	if (StreamIsActive(RS2_STREAM_INFRARED, &pipeline_profile_)) {

		intrinsics_[RS2_STREAM_INFRARED] = GetIntrinsics("ir");
		dist_coeffs_[RS2_STREAM_INFRARED] = GetDistortionCoeffs("ir");
	}
}

////////////////////////////////////////////////////////////////////////////////
CameraSensor::~CameraSensor() { }

////////////////////////////////////////////////////////////////////////////////
rs2::frameset CameraSensor::Capture() {

    rs2::frameset frames = pipeline_.wait_for_frames(); 
    return frames;
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::CaptureDepth(cv::Mat* image) {

    Warmup(&pipeline_, 10);
    rs2::depth_frame frame = Capture().first(RS2_STREAM_DEPTH).as<rs2::depth_frame>();

    *image = cv::Mat(cv::Size(frame.get_width(), frame.get_height()),
                     CV_16UC1, (void*) frame.get_data());
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::CaptureIr(cv::Mat* image) {

    Warmup(&pipeline_, 10);
    rs2::video_frame vframe = Capture().first(RS2_STREAM_INFRARED).as<rs2::video_frame>();

    *image = cv::Mat(cv::Size(vframe.get_width(), vframe.get_height()),
                     CV_8UC1, (void*) vframe.get_data());
}

////////////////////////////////////////////////////////////////////////////////
/*
pcl::PointCloud<pcl::PointXYZ>::Ptr CameraSensor::CapturePointCloud() {

    rs2::depth_frame frame = Capture().first(RS2_STREAM_DEPTH).as<rs2::depth_frame>();

    rs2::pointcloud pcloud;
    rs2::points points = pcloud.calculate(frame);

    return points_to_pcl(points);
}
*/

////////////////////////////////////////////////////////////////////////////////
const Eigen::Matrix4f CameraSensor::Extrinsics() const {

    return extrinsics_;
}

////////////////////////////////////////////////////////////////////////////////
Eigen::Vector3f CameraSensor::Position() const {

    return extrinsics_.col(3).head(3);
}

////////////////////////////////////////////////////////////////////////////////
Eigen::Matrix3f CameraSensor::RotationMatrix() const {

    return extrinsics_.topLeftCorner<3, 3>();
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::SetExtrinsics(Eigen::Matrix4f extrinsics) {

    extrinsics_ = extrinsics;
    inv_extrinsics_ = extrinsics_.inverse();
}

////////////////////////////////////////////////////////////////////////////////
float CameraSensor::MeterScale() const {
    
    return meter_scale_;
}

////////////////////////////////////////////////////////////////////////////////
const Eigen::Matrix4f CameraSensor::InverseExtrinsics() const {

    return inv_extrinsics_;
}

////////////////////////////////////////////////////////////////////////////////
const Eigen::Vector3f CameraSensor::PixelToPoint(int row, int col,
                                                 float depth) const {

    
    float point[3];
    float pixel[] = {(float) col, (float) row};
    rs2_deproject_pixel_to_point(point, &depth_intrinsics_, pixel, depth);
    return Eigen::Vector3f(point[0], point[1], point[2]);
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::PointToPixel(const Eigen::Vector3f& point, int* row,
                                int* col) const {

    float rs_pixel[2];
    float rs_point[] = {point[0], point[1], point[2]};

    // TODO: Add check to verify that depth_intrinscs_ are instantiated
    // Inaccurate when the sensor distortion model is modified brow conrady
    rs2_project_point_to_pixel(rs_pixel, &depth_intrinsics_, rs_point);
}

////////////////////////////////////////////////////////////////////////////////
const Eigen::Vector3f CameraSensor::TransformToWorldFrame(
    const Eigen::Vector3f& camera_point) const {

    Eigen::Vector4f camera_point_dot;
    camera_point_dot << camera_point, 1.0f;

    return (extrinsics_ * camera_point_dot).head(3);
}

////////////////////////////////////////////////////////////////////////////////
const Eigen::Vector3f CameraSensor::TransformToCameraFrame(
    const Eigen::Vector3f& world_point) const {

    Eigen::Vector4f world_point_dot;
    world_point_dot << world_point, 1.0f;

    return (inv_extrinsics_ * world_point_dot).head(3);
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::ActivateStream(const std::string stream, rs2::config* config) {

    rs2_stream stream_type;

    if (stream == "depth_stream")
        stream_type = RS2_STREAM_DEPTH;
    else if (stream == "ir_stream")
        stream_type = RS2_STREAM_INFRARED;

    config->enable_stream(stream_type);
    std::cout << "Activated stream: " << stream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
rs2::device CameraSensor::GetDevice(const std::string dev_name) {

    rs2::context ctx;
    rs2::device_list devices = ctx.query_devices();
    rs2::device selected_device;

    if (devices.size() == 0) {

        std::cerr << "No device connected" << std::endl;

        rs2::device_hub device_hub(ctx);
        selected_device = device_hub.wait_for_device();
    }
    else {
        for (rs2::device device : devices) {

            std::string curr_dev_name = GetDeviceName(device);

            if (dev_name == curr_dev_name.substr(0, dev_name.length())) {

                std::cout << "Found desired device: " << curr_dev_name << std::endl;
                selected_device = device;
                break;
            }
        }
    }

    return selected_device;
}

////////////////////////////////////////////////////////////////////////////////
std::string CameraSensor::GetDeviceName(const rs2::device& dev) {

    std::string name = "Unknown Device";
    if (dev.supports(RS2_CAMERA_INFO_NAME))
        name = dev.get_info(RS2_CAMERA_INFO_NAME);

    std::string sn = "########";
    if (dev.supports(RS2_CAMERA_INFO_SERIAL_NUMBER))
        sn = std::string("#") + dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);

    return name + " " + sn;
}

////////////////////////////////////////////////////////////////////////////////
rs2_intrinsics CameraSensor::GetStreamIntrinsics(rs2::pipeline_profile* profile,
	rs2_stream stream_type) {

    std::vector<rs2::stream_profile> streams = profile->get_streams();
    rs2_intrinsics intrin;

    for (const rs2::stream_profile s : streams) {

        if (s.stream_type() == stream_type) {

            rs2::video_stream_profile vp = s.as<rs2::video_stream_profile>();
            intrin = vp.get_intrinsics();
            break;
        }
    }

    return intrin;
}

////////////////////////////////////////////////////////////////////////////////
bool CameraSensor::StreamIsActive(rs2_stream stream_type,
	rs2::pipeline_profile* profile) {

    bool res = false;
    std::vector<rs2::stream_profile> streams = profile->get_streams();

    for (const rs2::stream_profile s : streams) {

        if (s.stream_type() == stream_type) {

            res = true;
            break;
        }
    }

    return res;
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::Warmup(rs2::pipeline* pipeline, uint32_t n_frames) {

    for (uint32_t i = 0; i < n_frames; i++)
        Capture();
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::RotateX(float angle) {

    Eigen::Matrix4f rot = Eigen::Matrix4f::Identity();
    rot(1, 1) = cosf(angle);
    rot(1, 2) = -sinf(angle);
    rot(2, 1) = sinf(angle);
    rot(2, 2) = cosf(angle);

    Eigen::Matrix4f extrinsics = rot * extrinsics_;

    SetExtrinsics(extrinsics);
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::RotateY(float angle) {

    Eigen::Matrix4f rot = Eigen::Matrix4f::Identity();
    rot(0, 0) = cosf(angle);
    rot(0, 2) = sinf(angle);
    rot(2, 0) = -sinf(angle);
    rot(2, 2) = cosf(angle);

    Eigen::Matrix4f extrinsics = rot * extrinsics_;

    SetExtrinsics(extrinsics);
}

////////////////////////////////////////////////////////////////////////////////
void CameraSensor::RotateZ(float angle) {

    Eigen::Matrix4f rot = Eigen::Matrix4f::Identity();
    rot(0, 0) = cosf(angle);
    rot(0, 1) = -sinf(angle);
    rot(1, 0) = sinf(angle);
    rot(1, 1) = cosf(angle);

    Eigen::Matrix4f extrinsics = rot * extrinsics_;

    SetExtrinsics(extrinsics);
}

////////////////////////////////////////////////////////////////////////////////
cv::Mat CameraSensor::GetIntrinsics(std::string stream_name) {

	rs2_stream stream_type = StringToStreamType(stream_name);
	
	rs2_intrinsics intrin = GetStreamIntrinsics(&pipeline_profile_, stream_type);

	cv::Mat intrin_out = cv::Mat::zeros(cv::Size(3, 3), CV_64FC1);

	intrin_out.at<double>(0, 0) = intrin.fx;
	intrin_out.at<double>(1, 1) = intrin.fy;
	intrin_out.at<double>(0, 2) = intrin.ppx;
	intrin_out.at<double>(1, 2) = intrin.ppy;
	intrin_out.at<double>(2, 2) = 1.0f;

	return intrin_out;
}

////////////////////////////////////////////////////////////////////////////////
cv::Mat CameraSensor::GetDistortionCoeffs(std::string stream_name) {
	
	rs2_stream stream_type = StringToStreamType(stream_name);

	rs2_intrinsics intrin = GetStreamIntrinsics(&pipeline_profile_, stream_type);

	cv::Mat distortion_coeffs(cv::Size(1, 5), CV_64FC1);

	for (uint8_t i = 0; i < 5; i++)
		distortion_coeffs.at<double>(0, i) = intrin.coeffs[i];

	return distortion_coeffs;
}

////////////////////////////////////////////////////////////////////////////////
rs2_stream CameraSensor::StringToStreamType(std::string stream_name) {

	rs2_stream stream_type;

	if (stream_name == "ir")
		stream_type = RS2_STREAM_INFRARED;
	else if (stream_name == "depth")
		stream_type = RS2_STREAM_DEPTH;
	
	return stream_type;
}

////////////////////////////////////////////////////////////////////////////////
cv::Mat CameraSensor::Intrinsics(std::string stream) {

    rs2_stream stream_type = StringToStreamType(stream);
	return intrinsics_[stream_type];
}

////////////////////////////////////////////////////////////////////////////////
cv::Mat CameraSensor::DistCoeffs(std::string stream) {

	return dist_coeffs_[StringToStreamType(stream)];
}

/// @file

