#include "ros/ros.h"
#include "test/tray.h"
#include "test/trayArray.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

geometry_msgs::Twist mover;
turtlesim::Pose cam;

void posicion(const turtlesim::Pose &pos){
    cam=pos;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "sub_array");
  ros::NodeHandle nh;
  ros::Rate robot(10);
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
  ros::Subscriber sub_pos = nh.subscribe("/turtle1/pose", 1000, posicion);
  while (ros::ok()){
    ros::spinOnce();
    mover.angular.z=-0.05;
    pub.publish(mover);
    robot.sleep();
    ROS_INFO("La posicion angular es %f", cam.theta);
    
  }  
  return 0;
}
