#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "test/ret.h"
#include "test/tray.h"
#include "test/trayArray.h"
#include "math.h"

test::trayArray cam;
test::tray datos;
test::ret r_t;
test::ret c;
turtlesim::Pose pos_t;
int count=0;
void posicion(const turtlesim::Pose &pos){
  pos_t=pos;

}
void accion(const test::ret &r){
  r_t=r;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "trayectoria");
  ros::NodeHandle nh;
  ros::Subscriber sub_pos = nh.subscribe("turtle1/pose", 1000, posicion);
  ros::Subscriber sub_ret = nh.subscribe("/retornar", 1000, accion);
  ros::Publisher pub_cam = nh.advertise<test::trayArray>("/camino",1000);
  ros::Rate loop(5);
  while(ros::ok()){
    ros::spinOnce();
    datos.x=pos_t.x;
    datos.y=pos_t.y;
    datos.theta=pos_t.theta;
    if(r_t.ret==0){
      cam.trayectoria.push_back(datos);
      r_t.ret=2;
      count=count+1;
    }
    if(r_t.ret==1){
    cam.trayectoria.push_back(datos);
    count=count+1;
    pub_cam.publish(cam);
    ROS_INFO("los datos son %d",count);
    ros::shutdown();
    }
 
  }  
  return 0;
}