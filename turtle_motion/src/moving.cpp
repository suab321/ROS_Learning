#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<turtlesim/Pose.h>
#include<iostream>

#define PI 3.141592

ros::Publisher pub;
ros::Subscriber sub;


using namespace std;

void move(double,double,double);
void rotate(double,double,double);
void desired_location();

double degree_to_radian(double);
double current_theta;

geometry_msgs::Twist coor;


void position(const turtlesim::Pose::ConstPtr &pos){
    current_theta=degree_to_radian(pos->theta);
}


int main(int argv,char **argc){
    ros::init(argv,argc,"moving_node");
    
    ros::NodeHandle n;


    double speed,distance,isforward,angle,direction,angular_speed;
    pub=n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",100);
    sub=n.subscribe("turtle1/pose",10,position);
    int t=1;
    int ch;
    while(t){
        cout<<"1. linear motion 2.rotation 3.desired direction"<<endl;
         cin>>ch;
    switch(ch){
        case 1:
            cout<<"Enter the distance"<<endl;
            cin>>distance;
            cout<<"Enter the speed"<<endl;
            cin>>speed;
            cout<<"1.forward 0.backward"<<endl;
            cin>>isforward;
             move(speed,distance,isforward);
             break;
        case 2:
            cout<<"Enter the angular speed"<<endl;
            cin>>angular_speed;
            cout<<"Enter the angle of rotation"<<endl;
            cin>>angle;
            cout<<"1.clockwise 0.anticlockwise"<<endl;
            cin>>direction;
            rotate(degree_to_radian(angular_speed),degree_to_radian(angle),direction);
            break;


        case 3:desired_location();
                break;
            }
            coor.linear.x=0;
            coor.angular.z=0;
            cout<<"1. continue 0.exit"<<endl;
            cin>>t;
    }

    
}

void move(double speed,double distance,double isforward){
    if(isforward)
        coor.linear.x= abs(speed);
    else
        coor.linear.x= -abs(speed);

    coor.linear.y=0;
    coor.linear.z=0;

    double t1=ros::Time::now().toSec();
    float target;
    ros::Rate loop_rate(10);
    do{
        pub.publish(coor);
        double tn=ros::Time::now().toSec();
        target=speed*(tn-t1);
        ros::spinOnce();
        loop_rate.sleep();
    }while(target < distance);
    coor.linear.x=0;
    pub.publish(coor);
}

void rotate(double angular_speed,double angle,double direction){
    if(direction == 0)
        coor.angular.z= angular_speed;
    else
        coor.angular.z= -angular_speed;
    double target=0.0;
    double t0=ros::Time::now().toSec();
    ros::Rate loop_rate(10);
    do{
        pub.publish(coor);
        double tn=ros::Time::now().toSec();
        target=angular_speed*(tn-t0);
        ros::spinOnce();
        loop_rate.sleep();
    }while(target < angle);

    coor.angular.z=0;

    pub.publish(coor);
}

void desired_location(){
    double desired;
    cout<<"Enter the desired location"<<endl;
    cin>>desired;
    desired=degree_to_radian(desired);
    double sub=desired-degree_to_radian(current_theta);
    coor.angular.z=20;
    cout<<sub<<endl;
    if(sub>0)
        rotate(degree_to_radian(20),abs(sub),1);
    else
        rotate(degree_to_radian(20),abs(sub),0);
}

//conversion
double degree_to_radian(double degree){
    return degree*PI/180.0;
}