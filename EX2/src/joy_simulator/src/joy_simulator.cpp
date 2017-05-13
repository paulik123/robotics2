#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/Joy.h>
#include <termios.h>

#include <sstream>

int getchnoblock()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt.c_cc[VMIN] = 0; 
  newt.c_cc[VTIME] = 0;
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "joy_simulator");

  ros::NodeHandle n;

  ros::Publisher joy_pub = n.advertise<sensor_msgs::Joy>("joy", 100);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    int c = getchnoblock();
    sensor_msgs::Joy joy_msg;
    joy_msg.header.stamp = ros::Time().now();


    if(c == 'w'){

      joy_msg.axes.push_back(0.0); // Left Horizontal
      joy_msg.axes.push_back(1.0); // Left Vertical (-1.0 through 1.0)
      joy_msg.axes.push_back(0.0); // Left Trigger
      joy_msg.axes.push_back(0.0); // Right Horizontal
      joy_msg.axes.push_back(0.0); // Right Vertical
      joy_msg.axes.push_back(0.0); // Right Trigger
      joy_msg.axes.push_back(0.0);

      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 's'){


      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(-1.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'd'){

      joy_msg.axes.push_back(1.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'a'){

      joy_msg.axes.push_back(-1.0);
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'i'){

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(1.0); 
      joy_msg.axes.push_back(0.0);
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'k'){

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(-1.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'l'){

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(1.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'j'){

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(-1.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);

            joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
    }
    else if(c == 'z'){
      joy_msg.buttons.push_back(1.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
    }
    else if(c == 'x'){
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(1.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
    }
    else if(c == 'c'){
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(1.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
    }else {
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);
      joy_msg.buttons.push_back(0.0);

      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0); 
      joy_msg.axes.push_back(0.0);
    }


    joy_pub.publish(joy_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
