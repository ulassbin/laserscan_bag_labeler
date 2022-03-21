#ifndef ROSBAG_LABELER_H_
#define ROSBAG_LABELER_H_

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ncurses.h>


#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <milvus_msgs/LabeledPointCloud.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
class bagManager
{
public:
  rosbag::Bag view_bag;
  rosbag::Bag write_bag;
  
  bagManager();
  bool copyFile(std::string source, std::string dest);
  void manage(std::string bag_name, std::string topic_name, std::string destination);
  ros::Publisher scan_pub_;
  ros::NodeHandle nh_;
};



#endif
