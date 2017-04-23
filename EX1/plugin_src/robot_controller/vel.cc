#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <termios.h>

// Gazebo's API has changed between major releases. These changes are
// accounted for with #if..#endif blocks in this file.
#if GAZEBO_MAJOR_VERSION < 6
#include <gazebo/gazebo.hh>
#else
#include <gazebo/gazebo_client.hh>
#endif

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

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo as a client
#if GAZEBO_MAJOR_VERSION < 6
  gazebo::setupClient(_argc, _argv);
#else
  gazebo::client::setup(_argc, _argv);
#endif

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to the  velodyne topic
  gazebo::transport::PublisherPtr pub =
    node->Advertise<gazebo::msgs::Vector3d>("~/Jack2/vel_cmd");

  // Wait for a subscriber to connect to this publisher
  pub->WaitForConnection();

  // Create a a vector3 message
  gazebo::msgs::Vector3d msg;


  // Send the message

  while(1){
      int c = getchnoblock();   // call your non-blocking input function
  if (c == 'w'){ 
        #if GAZEBO_MAJOR_VERSION < 6
      gazebo::msgs::Set(&msg, gazebo::math::Vector3(1, 0, 0));
    #else
      gazebo::msgs::Set(&msg, ignition::math::Vector3d(1, 0, 0));
    #endif
  }
  else if (c == 's'){
        #if GAZEBO_MAJOR_VERSION < 6
      gazebo::msgs::Set(&msg, gazebo::math::Vector3(-1, 0, 0));
    #else
      gazebo::msgs::Set(&msg, ignition::math::Vector3d(-1, 0, 0));
    #endif
  }
  else if (c == 'a'){
        #if GAZEBO_MAJOR_VERSION < 6
      gazebo::msgs::Set(&msg, gazebo::math::Vector3(2, 0, 0));
    #else
      gazebo::msgs::Set(&msg, ignition::math::Vector3d(2, 0, 0));
    #endif
  }

  else if (c == 'd'){
        #if GAZEBO_MAJOR_VERSION < 6
      gazebo::msgs::Set(&msg, gazebo::math::Vector3(3, 0, 0));
    #else
      gazebo::msgs::Set(&msg, ignition::math::Vector3d(3, 0, 0));
    #endif
  }else {
            #if GAZEBO_MAJOR_VERSION < 6
      gazebo::msgs::Set(&msg, gazebo::math::Vector3(0, 0, 0));
    #else
      gazebo::msgs::Set(&msg, ignition::math::Vector3d(0, 0, 0));
    #endif
  }

    pub->Publish(msg);
  }
      



  // Make sure to shut everything down.
#if GAZEBO_MAJOR_VERSION < 6
  gazebo::shutdown();
#else
  gazebo::client::shutdown();
#endif
}
