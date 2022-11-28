#include "ros/ros.h"
#include "test/tray.h"
#include "test/trayArray.h"



int main(int argc, char **argv)
{
  ros::init(argc, argv, "array");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<test::trayArray>("/prueba", 1000);
  ros::Rate loop_rate(10);
  test::tray dato;
  test::trayArray msg;
  float dato_x=0; float dato_y=0;
  int cont=0;
  while (ros::ok()){
    dato.x=dato_x;
    dato.y=dato_y;
    msg.trayectoria.push_back(dato);
    dato_x=dato_x+1;
    dato_y=dato_y+1;
    cont=cont+1;
    ros::spinOnce();
    loop_rate.sleep(); 
    if (cont==10){
        pub.publish(msg);
    }
  }

  return 0;
}