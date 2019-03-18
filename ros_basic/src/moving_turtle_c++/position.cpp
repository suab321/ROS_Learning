#include<ros/ros.h>
#include<turtlesim/Pose.h>

void show_pos(const turtlesim::Pose::ConstPtr &coor){
    float x=coor->x;
    float y=coor->y;
    //float z=coor->z;
    ROS_INFO("x is %uf; y is %uf",x,y);
}

int main(int argv,char **argc){

    ros::init(argv,argc,"position");

    ros::NodeHandle n;
    
    ros::Subscriber sub=n.subscribe("turtle1/pose",100,show_pos);

    ros::spin();
}