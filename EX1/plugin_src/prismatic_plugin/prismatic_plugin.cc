#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <stdio.h>
#include <unistd.h> // sleep function you added yourself
#include <gazebo/physics/Joint.hh> //These two lines you added to use the function getforcetorque
#include <gazebo/sensors/sensors.hh>


namespace gazebo
{  
  class SetJoints : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) 
    {
      // Store the pointer to the model
      this->model = _parent;

      this->x = 0;


      this->joint1_ = this->model->GetJoint("mid_prismatic_joint");
      this->joint2_ = this->model->GetJoint("fork1_prismatic");
      this->joint3_ = this->model->GetJoint("fork2_prismatic");

      this->link_ = this->model->GetLink("link");
      std::string sensorName = link_->GetSensorName(0);
      sensors::SensorPtr sensor = sensors::get_sensor(sensorName);
      if (sensor->Type() == "ray")
      {
        this->raySensor =
            std::dynamic_pointer_cast<sensors::RaySensor>(sensor);
      }


      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&SetJoints::OnUpdate, this, _1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
    

      if(this->raySensor->Range(0) < this->raySensor->RangeMax() && this->x == 1){
        this->joint1_->SetForce(0, 10000.0);
        this->joint2_->SetForce(0, 10000.0);
        this->joint3_->SetForce(0, 10000.0);
      }
      else if(this->raySensor->Range(0) < 0.07 && this->x == 0){
        this->joint1_->SetForce(0, 10000.0);
        this->joint2_->SetForce(0, 10000.0);
        this->joint3_->SetForce(0, 10000.0);
        this->x = 1;
      }else{
        this->x = 0;
      }
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;


    //common::Time last_update_time_; Not needed

    //Other pointers
    physics::JointPtr joint1_;
    physics::JointPtr joint2_;
    physics::JointPtr joint3_;
    sensors::RaySensorPtr raySensor;
    physics::LinkPtr link_;
    int x;


  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(SetJoints)
}