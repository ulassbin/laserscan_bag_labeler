#ifndef ROSBAG_LABELER_H_
#define ROSBAG_LABELER_H_

#include<iostream>
#include<stdio.h>
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

class bagManager
{
public:
  rosbag::Bag view_bag;
  rosbag::Bag write_bag;
  
  bagManager();
  void manage(std::string bag_name);
  ros::Publisher scan_pub_;
  ros::NodeHandle nh_;
};



#endif
