#include <rosbag_labeler/labeler.h>


bool bagManager::copyFile(std::string source, std::string dest)
{
  FILE *fs, *ft;
  char ch;
  fs = fopen(source.c_str(), "r");
  if(fs == NULL)
  {
    cout<<"\nError Occurred!";
    return 0;
  }
  ft = fopen(dest.c_str(), "w");
  if(ft == NULL)
  {
    cout<<"\nError Occurred!";
    return 0;
  }
  ch = fgetc(fs);
  while(ch != EOF)
  {
    fputc(ch, ft);
    ch = fgetc(fs);
  }
  cout<<"\nFile copied successfully.";
  fclose(fs);
  fclose(ft);
  cout<<endl;
  return 1;
}

bagManager::bagManager()
{
  ROS_INFO("Constructed as tabula rasa");
}

void bagManager::manage(std::string bag_name, std::string topic_name, std::string destination)
{
  try
  {
    view_bag.open(bag_name);
    write_bag.open(destination, rosbag::bagmode::Append);
  }
  catch(rosbag::BagIOException ex)
  {
    ROS_INFO("Error is %s", ex.what());
    return;
  }
  //r1000913c5_60f1b4879d4ba0371046ed67_2021-07-16-19-32-08.bag
  //write_bag.open(bag_name, rosbag::bagmode::Append);

  std::vector<std::string> topics;

  scan_pub_ = nh_.advertise<sensor_msgs::PointCloud>("label_scan", 1, true);

  ROS_INFO("Bag Read state %d, write state %d", view_bag.isOpen(), write_bag.isOpen());
  topics.push_back(topic_name);
  //topics.push_back(std::string("/r200171362/odom"));
  std::string labeled_topic = topic_name + "/labeled";
  rosbag::View view(view_bag, rosbag::TopicQuery(topics));
  ros::Rate rate(0.1);
  int label;

  initscr();
  ROS_INFO("Scr intted");
  int count = 0; // max steps to label
  foreach(rosbag::MessageInstance const m, view)
  {
      count++;
    ROS_INFO("Running  COUNT %d", count);
//      if(count >= 50)
//      {
//        endwin();
//       write_bag.close();
//        view_bag.close();
//        return;
//      }
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
        else if(label == 57)
        {
          endwin();
          write_bag.close();
          view_bag.close();
          return;
        }
        else
        {
          write_msg.label = 0;
          std::cout<<"Labeling as stable"<<std::endl;
        }
        write_bag.write(labeled_topic, s->header.stamp, write_msg);
      }
      nav_msgs::Odometry::ConstPtr i = m.instantiate<nav_msgs::Odometry>();
      if (i != NULL)
          std::cout << i->child_frame_id << std::endl;
    //rate.sleep();
    ros::spinOnce();
  }
  ROS_INFO("Out foreach");
  endwin(); 
  view_bag.close();
}


int main(int argc, char** argv)
{
  std::cout<<"Argc is "<< argc<<std::endl;
  std::string bag_name;
  std::string topic_name;
  std::string destination;
  for(int i=1; i<argc; i++)
  {
   std::cout<<argv[i]<<std::endl;
   if(i == 1)
     topic_name = argv[i];
   else if(i == 2)
     bag_name = argv[i];
   else if(i == 3)
     destination= argv[i];
  }
  std::cout << "Topic " << topic_name.c_str() << std::endl;
  std::cout << "Bag " << bag_name.c_str() << std::endl;
  std::cout << "Dest " << destination.c_str() << std::endl;
  if(argc < 4)
   return 1;

  ros::init(argc, argv, "rosbag_labeler");
  ROS_INFO("Started labeling");
  bagManager manager;
  manager.manage(bag_name, topic_name, destination);
  return 1;
}
