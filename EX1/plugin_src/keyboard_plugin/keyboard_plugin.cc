#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/physics/Joint.hh>

namespace gazebo
{
  /// \brief A plugin to control a Velodyne sensor.
  class VelodynePlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: VelodynePlugin() {}

    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Safety check
      if (_model->GetJointCount() == 0)
      {
        std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
        return;
      }

      // Store the model pointer for convenience.
      this->model = _model;

      // Get the first joint. We are making an assumption about the model
      // having one joint that is the rotational joint.
      this->wheel1_ = _model->GetJoint("differential_wheel1_joint");
      this->wheel2_ = _model->GetJoint("differential_wheel2_joint");
      this->cylinder_ = _model->GetJoint("differentialcylinder_mid_z");
      this->link_ = this->model->GetLink("differential_cylinde");
      this->y = 0;


      // Create the node
      this->node = transport::NodePtr(new transport::Node());
      #if GAZEBO_MAJOR_VERSION < 8
      this->node->Init(this->model->GetWorld()->GetName());
      #else
      this->node->Init(this->model->GetWorld()->Name());
      #endif

      // Create a topic name
      std::string topicName = "~/" + this->model->GetName() + "/vel_cmd";

      // Subscribe to the topic, and register a callback
      this->sub = this->node->Subscribe(topicName,
         &VelodynePlugin::OnMsg, this);
    }



    /// \brief Handle incoming message
    /// \param[in] _msg Repurpose a vector3 message. This function will
    /// only use the x component.
    private: void OnMsg(ConstVector3dPtr &_msg)
    {
      int cmd = _msg->x();

      if(cmd == 1){
        this->wheel1_->SetForce(0, 500.0);
        this->wheel2_->SetForce(0, 500.0);
      }else if(cmd == -1){
        this->wheel1_->SetForce(0, -500.0);
        this->wheel2_->SetForce(0, -500.0);
      }
      else if(cmd == 2){
        this->cylinder_->SetForce(0, 600.0);
      }
      else if(cmd == 3){
        this->cylinder_->SetForce(0, -600.0);
      }else if(cmd == 0){
        this->wheel1_->SetForce(0, 0);
        this->wheel2_->SetForce(0, 0);
        this->cylinder_->SetForce(0, 0);
      }
      this->cylinder_->SetForce(0, 0);
      this->cylinder_->SetVelocity(0, 0);
    }

    /// \brief A node used for transport
    private: transport::NodePtr node;

    /// \brief A subscriber to a named topic.
    private: transport::SubscriberPtr sub;

    /// \brief Pointer to the model.
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr wheel1_;
    private: physics::JointPtr wheel2_;
    private: physics::JointPtr cylinder_;
    physics::LinkPtr link_;
    double y;

    /// \brief A PID controller for the joint.
    private: common::PID pid;
  };

  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(VelodynePlugin)
}
#endif
