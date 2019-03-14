#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<turtlesim/Pose.h>
#include<iostream>

#define PI 3.141592
#define k 0.5

ros::Publisher pub;
ros::Subscriber sub;


using namespace std;

void move(double,double,double);
void rotate(double,double,double);
void desired_angle();
void goal_location();
void pattern();
void spiral_pattern();
double getDistance(double,double,double,double);

double degree_to_radian(double);
turtlesim::Pose position_bot;

geometry_msgs::Twist coor;


void position(const turtlesim::Pose::ConstPtr &pos){
     position_bot.x=pos->x;
     position_bot.theta=pos->theta;
     position_bot.y=pos->y;
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
        cout<<"1. linear motion 2.rotation 3.desired direction 4.goto desired location 5.pattern 6.spiral pattern"<<endl;
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


        case 3:desired_angle();
                break;
        case 4:goal_location();
                break;
        case 5:pattern();
                break;

        case 6:spiral_pattern();break;
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

void desired_angle(){
    double desired;
    cout<<"Enter the desired location"<<endl;
    cin>>desired;
    desired=degree_to_radian(desired);
    double sub=desired-degree_to_radian(position_bot.theta);
    coor.angular.z=20;
    cout<<sub<<endl;
    if(sub>0)
        rotate(degree_to_radian(20),abs(sub),1);
    else
        rotate(degree_to_radian(20),abs(sub),0);
}

void goal_location(){
    double x,y;
    cout<<"Enter the x coordinate"<<endl;
    cin>>x;
    cout<<"Enter the y-coordinate"<<endl;
    cin>>y;
    geometry_msgs::Twist target;
    ros::Rate loop_rate(100);
    do{
        double velocity=k*getDistance(x,y,position_bot.x,position_bot.y);
        target.linear.x=velocity;
        target.linear.y=0;
        target.linear.z=0;
        target.angular.x=0;
        target.angular.y=0;
        target.angular.z=4*(atan2(y-position_bot.y,x-position_bot.x)-position_bot.theta);

        pub.publish(target);

        ros::spinOnce();
        loop_rate.sleep();
    }while(getDistance(x,y,position_bot.x,position_bot.y) > 0.01);
    target.linear.x=0;
    target.angular.z=0;
    pub.publish(target);
}

//calculating distance in 2-D plane

double getDistance(double x1,double y1,double x2,double y2){   
     return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

//bar graph like pattern
void pattern(){
    ros::Rate r(1);
    move(2.0,4.0,1);
    do{
    double dir=1;
    rotate(degree_to_radian(10),degree_to_radian(90),0);
    move(3,4,1);
    rotate(degree_to_radian(10),degree_to_radian(90),0);
    move(1,1,1);
    rotate(degree_to_radian(10),degree_to_radian(90),0);
    move(3,4,1);
    rotate(degree_to_radian(10),degree_to_radian(90),0);
    if(dir)
        move(1,1,0);
    else
        move(1,1,1);

    ros::spinOnce();
    r.sleep();
    }while(position_bot.x < 9.5);
}

//spiral pattern
void spiral_pattern(){
    geometry_msgs::Twist spiral;
    double an_speed=4.0;
    double rk=0.5;
    ros::Rate r(1);
    do{
        rk=rk+1.0;
        spiral.linear.x=rk;
        spiral.angular.z=an_speed;
        pub.publish(spiral);

        ros::spinOnce();
        r.sleep();
    }while(position_bot.x<10.5 && position_bot.y<10.5);

    spiral.linear.x=0;
    spiral.angular.z=0;
    pub.publish(spiral);
}

//conversion
double degree_to_radian(double degree){
    return degree*PI/180.0;
}