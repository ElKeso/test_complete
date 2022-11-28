#include "ros/ros.h"
#include "test/ret.h"
#include "test/tray.h"
#include "test/trayArray.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include "math.h"


int f=0; float cte=1.5;
float PI = 3.14159265;
float v_l; float v_a;
geometry_msgs::Twist mover;
test::tray datos;
test::trayArray camino;
turtlesim::Pose cam;
float t_l=0.15; float t_a=0.1; 
int i=0; int c; int c_p; int stop=0;
float x; float y;



void retorno(const test::ret &r){
    c=r.datos;
    c=c-2;
    c_p=2;
    f=r.ret;
    stop=6;
}

void trayectoria(const test::trayArray &t){
    camino=t;
}

void posicion(const turtlesim::Pose &pos){
    cam=pos;
}

float eu_lineal(float x, float y){
  v_l=sqrt(pow((x-cam.x),2)+pow((y-cam.y),2));
  return v_l;
}

float eu_angular(float x, float y){
  v_a=atan2(y-cam.y,x-cam.x);
  return v_a;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "retorno");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
  ros::Subscriber sub_cam = nh.subscribe("/camino", 1000, trayectoria);
  ros::Subscriber sub_pos = nh.subscribe("/turtle1/pose", 1000, posicion);
  ros::Subscriber sub_ret = nh.subscribe("/retornar",1000, retorno);
  ros::Rate loop_rate(100);//rate del while
  ros::Rate robot(10);// publicacion para moverse
  c=0;
  datos.x=c;
  datos.y=c;
  datos.theta=c;
  camino.trayectoria.push_back(datos);
  
  while(ros::ok()){
    ros::spinOnce();
    //moverse a las cordenadas...
    if(f==1){
    x=camino.trayectoria[c].x;
    y=camino.trayectoria[c].y;
    switch(stop){
      case 1://rotamos para llegar al angulo correcto
        //funcion para rotar
      if(abs(eu_angular(x, y)-cam.theta)>t_a){
        mover.linear.x=0;
        if((2*PI)-eu_angular(x, y)>eu_angular(x, y)){
          mover.angular.z=0.2;
        }
        else {
          mover.angular.z=-0.2;
        } 
      }
        else{
          //frenar...
          stop=2;
        }
        pub.publish(mover);
        robot.sleep();
      break;
         
      case 2://avanzamos hasta avanzar la distancia determinada
      //funcion para avanzar
        if((eu_lineal(x, y))>=t_l){
          mover.linear.x=0.2; 
          mover.angular.z=0;   
        }
        else{
          //frenamos...
          stop=3;
        }
        pub.publish(mover);
        robot.sleep();
      break;

      case 3://cambiamos coordenadas de trayectoria
        //funcion para recorrer matriz de coordenadas
          c=c-c_p;
          if(c>5){
            stop=1;
          }
          else{
            if(c<=5){
              if(c<=1){
                c=0;
                stop=4;
              }
              else{
                c=1;
                stop=1;
              }
            }
          }
  //pub.publish(mover);
      break;

      case 4://rotamos al angulo de origen
        //funcion para rotar robot a su posicion original
          if(abs(cam.theta)>0.01){
            mover.linear.x=0;
            mover.angular.z=0.2;
          }
          else{
            stop=5;
            //frenamos...  
            mover.linear.x=0;
            mover.angular.z=0;
          }
        pub.publish(mover);
        robot.sleep();
      break;

      case 5://apagamos
        ros::shutdown();
      break;

      case 6://frenar primero!
        //frenar...
        for(int i=0; i<1000; i=i+1){
          mover.linear.x=0;
          pub.publish(mover);
        }
        stop=1;
      break;
     //hacer default para no hacer nada y esperar denuevo otro retorno
    }
    ROS_INFO("estoy en la secuencia %d, voy en %d y coordenadas son x=%f y=%f y el angulo es %f",stop, c, x, y, eu_angular(x, y));
    loop_rate.sleep();
  }
}
  return 0;
}

