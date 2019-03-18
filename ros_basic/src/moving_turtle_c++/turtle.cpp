#include<ros/ros.h>
#include<geometry_msgs/Twist.h>

using namespace std;

int main(int argv,char **argc){
    ros::init(argv,argc,"coordinates_publisher");

    ros::NodeHandle n;
    ros::Publisher pub;

    pub=n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);

    int count=0;
    while(ros::ok()){
        float lx,ly,lz,ax,ay,az;
        
        cout<<"Enter the linear x coordinate"<<endl;
        cin>>lx;
        cout<<"Enter the linear y coordinate"<<endl;
        cin>>ly;
        cout<<"Enter the linear z coordinate"<<endl;
        cin>>lz;
        cout<<"Enter the angular x coordinate"<<endl;
        cin>>ax;
        cout<<"Enter the angular y coordinate"<<endl;
        cin>>ay;
        cout<<"Enter the angular z coordinate"<<endl;
        cin>>az;

        geometry_msgs::Twist coor;
        coor.linear.x=lx;
        coor.linear.y=ly;
        coor.linear.z=lz;
        coor.angular.x=ax;
        coor.angular.y=ay;
        coor.angular.z=az;

        ROS_INFO("The linear coordinates you enterd are x:%f, y:%f,z:%f\n",lx,ly,lz);
        ROS_INFO("The angular coordinates you enterd are x:%f, y:%f,z:%f\n",ax,ay,az);
        pub.publish(coor);

       ros::spin();

    }
}