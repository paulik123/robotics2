#include "ros/ros.h"
#include <cmath>
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Quaternion.h"
#include <nav_msgs/Odometry.h>


#define RUN_PERIOD_DEFAULT 0.1
#define NAME_OF_THIS_NODE "pose_from_odom"

class ROSnode {
private:
    ros::NodeHandle Handle;
    ros::Subscriber odomSub;
    ros::Publisher posePub;
    geometry_msgs::PoseStamped out;

    double x, y, yaw;
    double t;
            
    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg);
public:
    void Prepare();
    void RunContinuously();
    void RunPeriodically();
};

void ROSnode::Prepare() {
    odomSub = Handle.subscribe("odom", 10, &ROSnode::odomCallback, this);    
    posePub = Handle.advertise<geometry_msgs::PoseStamped>("pose", 10);

    x = 0.0;
    y = 0.0;
    yaw = 0.0;
    t = -1.0;





    ROS_INFO("Node %s ready to run.", ros::this_node::getName().c_str());
}



void ROSnode::odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
    if(t < 0) { t = msg->header.stamp.toSec(); return; }
    float v = msg->twist.twist.linear.x;
    float w = msg->twist.twist.angular.z;
    double dt = msg->header.stamp.toSec() - t;
    x = x + v*cos(yaw)*dt;
    y = y + v*sin(yaw)*dt;
    yaw = yaw + w*dt;
    t = msg->header.stamp.toSec();


    out.header = msg->header;
    out.header.frame_id = "/base_link";

    geometry_msgs::Quaternion q;
    q.x = (float) 0.0;
    q.y = (float) 0.0;
    q.z = (float) sin(yaw/2.0);
    q.w = (float) cos(yaw/2.0);
    out.pose.orientation = q;

    out.pose.position.x = x;
    out.pose.position.y = y;
    out.pose.position.z = 0.0;
}

void ROSnode::RunContinuously() {
    ros::spin();
}

void ROSnode::RunPeriodically () {
    ros::Rate r(10);
    ros::Rate r1(1/RUN_PERIOD_DEFAULT);
    
    while(ros::ok()) {
        ros::spinOnce();
        posePub.publish(out);
        r.sleep();
    }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, NAME_OF_THIS_NODE);
  ROSnode mNode;
   
  mNode.Prepare();
  //mNode.RunContinuously();
  mNode.RunPeriodically();
  
  return (0);
}

