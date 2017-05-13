#include "ros/ros.h"
#include <cmath>
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "examples/Goal.h"


#define RUN_PERIOD_DEFAULT 0.1
#define NAME_OF_THIS_NODE "control"

class ROSnode {
private:
    ros::NodeHandle Handle;
    ros::Subscriber currentPoseSub;
    ros::Publisher cmdPub;
    bool gotPose, orientation, position;
    geometry_msgs::Pose goal;

            
    void currentPoseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg);
public:
    void Prepare();
    void RunContinuously();
    void RunPeriodically();
};

void ROSnode::Prepare() {
    currentPoseSub = Handle.subscribe("pose", 10, &ROSnode::currentPoseCallback, this);    
    cmdPub = Handle.advertise<geometry_msgs::Twist>("cmd_auto", 10);

    gotPose = false;
    position = false;
    orientation = false;

    ROS_INFO("Node %s ready to run.", ros::this_node::getName().c_str());
}



void ROSnode::currentPoseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
    geometry_msgs::Twist out;
    ros::ServiceClient client = Handle.serviceClient<examples::Goal>("goal");

    if(!gotPose) {
        examples::Goal srv;
        srv.request.ok = true;
        if(client.call(srv)) {
        goal = srv.response.pose;
        //ROS_INFO("Goal x=%f, y=%f",goal.position.x, goal.position.y );
        //ROS_INFO("MSG x=%f, y=%f",msg->pose.position.x, msg->pose.position.y );
        gotPose = true; 
        position = false; 
        orientation = false;
        } else { return; }
        }
    if(!orientation) {
        double gy = goal.position.y; 
        double gx = goal.position.x;
        double th = atan2(gy - msg->pose.position.y, gx - msg->pose.position.x);
        //quaternion magic double yaw <- yawFromQuaternion(msg-
        //>pose.orientation)
        geometry_msgs::Quaternion q = msg->pose.orientation;
        double ysqr = q.y * q.y;
        double t3 = 2.0 * (q.w * q.z + q.x * q.y);
        double t4 = 1.0 - 2.0 * (ysqr + q.z * q.z);  
        double yaw = atan2(t3, t4);


        if(fabs(yaw - th) < 0.05){
             orientation = true;
             ROS_INFO("Orientation = true - yaw=%f, th=%f", yaw, th);
        }
       
        else{
            ROS_INFO("Orientation = false - yaw=%f, th=%f", yaw, th);
            out.angular.z = 0.05;
        }
        

    }

    if(!position && orientation) {
        double gy = goal.position.y;
        double gx = goal.position.x;
        double py = msg->pose.position.y;
        double px = msg->pose.position.x;

        double xdif = gx-px;
        double ydif = gy-py;

        double d2 = pow(xdif, 2) + pow(ydif, 2);
	double d = sqrt(d2);
        //control magic out.linear.x <- DistanceToSpeed(d2);
        double vel = d/10.0 + 0.02;
;
	if(vel > 0.5) vel = 0.5;
        out.linear.x = vel;

        
        if(d < 0.2) { 
            out.linear.x = 0.0; 
            position = true; 
            ROS_INFO("Position = true - D=%f, VEL= %f",d, vel);
            }
            else{
                ROS_INFO("Position = false - D=%f, VEL=%f",d, vel);
            }
        }
        if(position && orientation){
        gotPose = false;
        }
        cmdPub.publish(out);

    }

void ROSnode::RunContinuously() {
    ros::spin();
}

void ROSnode::RunPeriodically () {
    ros::Rate r(10);
    ros::Rate r1(1/RUN_PERIOD_DEFAULT);
    
    while(ros::ok()) {
        ros::spinOnce();
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

