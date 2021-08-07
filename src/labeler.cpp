#include <rosbag_labeler/labeler.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "rosbag_labeler");
  ROS_INFO("Started labeling");
  ros::spin();
}
