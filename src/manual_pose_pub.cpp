#include <iostream>
#include <queue>

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

////////////////////////////////////////////////////////////////////////////////
#define MIN_X 0.0
#define MAX_X 0.7
#define MIN_Y -0.7
#define MAX_Y 0.7
#define MIN_Z 0.0
#define MAX_Z 1.0

////////////////////////////////////////////////////////////////////////////////
bool valid_xyz(double x, double y, double z) {

	bool input_x_valid = (x >= MIN_X && x <= MAX_X);
	bool input_y_valid = (y >= MIN_Y && y <= MAX_Y);
	bool input_z_valid = (z >= MIN_Z && z <= MAX_Z);
	return (input_x_valid && input_y_valid && input_z_valid);
}

////////////////////////////////////////////////////////////////////////////////
bool get_user_position(double* x, double* y, double* z) {

	std::string input;
	std::string command, tmp;

    std::queue<std::string> v;

    getline(std::cin, input);
    std::istringstream iss(input);
    getline(iss, command, ' ');

    if (command == "pos") {

        while (getline(iss, tmp, ' ')) {

            v.push(tmp);
        }

        if (v.size() != 3) {

            std::cout << "Syntax error: 3 arguments expected" << std::endl;
            return false;
        }

        *x = std::atof(v.front().c_str());
        v.pop();
        *y = std::atof(v.front().c_str());
        v.pop();
        *z = std::atof(v.front().c_str());
        v.pop();

        if (!valid_xyz(*x, *y, *z)) {

            std::cout << "Setpoint out of bounds" << std::endl;
            return false;
        }

        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    ros::init(argc, argv, "main");
    ros::NodeHandle nh;

    ros::Publisher pose_pub = nh.advertise<geometry_msgs::Pose>("pose", 100);

    double x, y, z;

    while (ros::ok()) {

        bool valid_pos = get_user_position(&x, &y, &z);

        if (valid_pos) {

            geometry_msgs::Pose target_pose;
            std::cout << "user position: " << x << ", " << y << ", " << z << std::endl;
            target_pose.position.x = x;
            target_pose.position.y = y;
            target_pose.position.z = z;
            target_pose.orientation.x = 1.0;

            pose_pub.publish(target_pose);
            std::cout << "published pose" << std::endl;
        }

        ros::spinOnce();
    }

    return 0;
}
