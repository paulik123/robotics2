#include "ros/ros.h"
#include <cmath>
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Quaternion.h"
#include "examples/SetMode.h"
#include "examples/Goal.h"


#define RUN_PERIOD_DEFAULT 0.1
#define NAME_OF_THIS_NODE "joy_cmd"

class ROSnode {
private:
    double maxLinear, maxAngular;
    double x, y, theta;
    ros::NodeHandle Handle;
    ros::Subscriber joySub;
    ros::Publisher cmdPub;
    ros::ServiceServer goalSr;
    int currentMode;
    ros::ServiceClient client;

    
    geometry_msgs::Twist out;
            
    void joyCallback(const sensor_msgs::Joy::ConstPtr& msg);
    bool goalService(examples::Goal::Request &req, examples::Goal::Response &res);
public:
    void Prepare();
    void RunContinuously();
    void RunPeriodically();
};

void ROSnode::Prepare() {
    joySub = Handle.subscribe("joy", 10, &ROSnode::joyCallback, this);    
    cmdPub = Handle.advertise<geometry_msgs::Twist>("cmd_joy", 10);
    goalSr = Handle.advertiseService("goal", &ROSnode::goalService, this);
    
    currentMode = 0;

    Handle.param("/max_linear", maxLinear, 1.0);
    Handle.param("/max_angular", maxAngular, 1.0);
    Handle.param("/x", x, 10.0);
    Handle.param("/y", y, 10.0);
    Handle.param("/theta", theta, 0.0);


    ROS_INFO("Node %s ready to run.", ros::this_node::getName().c_str());
}

bool ROSnode::goalService(examples::Goal::Request &req, examples::Goal::Response &res) {
    //Desired position



    geometry_msgs::Pose goal;
    geometry_msgs::Quaternion q;
    q.x = (float) 0.0;
    q.y = (float) 0.0;
    q.z = (float) sin(theta/2.0);
    q.w = (float) cos(theta/2.0);
    goal.orientation = q;     // (x, y, z, w) = (0, 0, sin(theta/2), cos(theta/2)).
    goal.position.x = (float) x;
    goal.position.y = (float) y;
    goal.position.z = (float) 0.0;
    res.pose = goal;
    
    return true;
}

void ROSnode::joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
    if(msg->buttons[0] == 1){
            ros::ServiceClient client = Handle.serviceClient<examples::SetMode>("mode");
            examples::SetMode srv;
            if(currentMode == 0){
                srv.request.mode = 1;
                currentMode = 1;
            }else{
                srv.request.mode = 0;
                currentMode = 0;
            }

            //ROS_INFO("ServiceName=%s", client.getService().c_str());
            if (client.call(srv))
          {
            ROS_INFO("OK");
          }
          else
          {
            ROS_ERROR("KO");
          }


    }
    else{    
    out.linear.x = maxLinear * msg->axes[1];
    
    //right analog left/right
    out.angular.z = maxAngular * msg->axes[3];
    }
}

void ROSnode::RunContinuously() {
    ros::spin();
}

void ROSnode::RunPeriodically () {
    ros::Rate r(10);
    ros::Rate r1(1/RUN_PERIOD_DEFAULT);
    
    while(ros::ok()) {
        ros::spinOnce();
        cmdPub.publish(out);
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

