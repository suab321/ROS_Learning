#include "ros/ros.h"
#include "std_msgs/String.h"


show_msg(const std_msgs::String::ConstPtr & msg){
    ROS_INFO("I heard [%s]",msg->data.c_str());
}

int main(int argc,char **argv){
     ros::init(argc,argv,"listener");

     ros::Subscriber sub=n.subscribe("chatter",1000,show_msg);

     ros::spin();
      return 0;
}