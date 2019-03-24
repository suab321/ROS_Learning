#include<ros.h>
#include<ros/time.h>
#include<sensor_msgs/Range.h>

ros::NodeHandle nh;
sensor_msgs::Range r;
ros::Publisher pub_range("ultra_range",&r);

char frame_id[]="ultra";

const int trigpin=9;
const int echopin=10;

void setup(){
  nh.initNode();
  nh.advertise(pub_range);
  r.radiation_type=0;
  r.header.frame_id=frame_id;
  r.field_of_view=0.1;
  r.min_range=0.002;
  r.max_range=0.150;
}

long getRange(){
  long duration;
  long distance;

  digitalWrite(trigpin,OUTPUT);
  pinMode(trigpin,HIGH);
  delayMicroseconds(2);
  pinMode(trigpin,LOW);
  delayMicroseconds(5);
  pinMode(trigpin,HIGH);

  duration=pulseIn(echopin,HIGH);
  distance=duration*0.034/2;

  return distance;
  
}

void loop(){
  r.range=getRange();
  pub_range.publish(&r);
  r.header.stamp=nh.now();
  nh.spinOnce();
  delay(400);
  
}
