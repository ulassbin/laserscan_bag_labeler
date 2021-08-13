#include <rosbag_labeler/labeler.h>


bagManager::bagManager()
{
  ROS_INFO("Constructed as tabula rasa");
}

void bagManager::manage(std::string bag_name)
{
  view_bag.open("/home/ulas/Desktop/bags/r200171362_test.bag");
  write_bag.open("/home/ulas/Desktop/bags/r200171362_test_labeled.bag", rosbag::bagmode::Append);

  std::vector<std::string> topics;

  scan_pub_ = nh_.advertise<sensor_msgs::PointCloud>("label_scan", 1, true);

  ROS_INFO("Bag Read state %d, write state %d", view_bag.isOpen(), write_bag.isOpen());
  topics.push_back(std::string("/r200171362/scan_ds"));
  //topics.push_back(std::string("/r200171362/odom"));

  rosbag::View view(view_bag, rosbag::TopicQuery(topics));
  ros::Rate rate(1);
  int label;

  initscr();
  ROS_INFO("Scr intted");
  int count = 0; // max steps to label
  foreach(rosbag::MessageInstance const m, view)
  {
      count++;
    ROS_INFO("Running  COUNT %d", count);
      if(count >= 50)
      {
        endwin();
        write_bag.close();
        view_bag.close();
        return;
      }
      if(!ros::ok())
      {
        ROS_INFO("Ros dead");
        return;
      }
      sensor_msgs::PointCloud::ConstPtr s = m.instantiate<sensor_msgs::PointCloud>();
      if (s != NULL)
      {
        std::cout << s->header.frame_id << std::endl;
        sensor_msgs::PointCloud temp = *s;
        temp.header.stamp = ros::Time::now();
        scan_pub_.publish(temp);
        std::cout<<"Input label"<<std::endl;
        label = getch();
        std::cout<<"Label is "<<label<<std::endl;
        milvus_msgs::LabeledPointCloud write_msg;
        write_msg.cloud = *s;
        write_msg.header = s->header;
        if(label == 49)
        {
          std::cout<<"Labeling as lost"<<std::endl;
          write_msg.label = 1;
        }
        else
        {
          write_msg.label = 0;
          std::cout<<"Labeling as stable"<<std::endl;
        }
        write_bag.write("labeled_cloud", s->header.stamp, write_msg);
      }
      nav_msgs::Odometry::ConstPtr i = m.instantiate<nav_msgs::Odometry>();
      if (i != NULL)
          std::cout << i->child_frame_id << std::endl;
    rate.sleep();
    ros::spinOnce();
  }
  ROS_INFO("Out foreach");
  endwin(); 
  view_bag.close();
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "rosbag_labeler");
  ROS_INFO("Started labeling");
  bagManager manager;
  manager.manage("test");
  ros::spin();
}
