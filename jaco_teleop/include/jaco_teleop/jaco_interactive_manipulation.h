/*
 * jaco_interactive_manipulation.h
 *
 *      Author: David Kent
 */

#ifndef JACO_INTERACTIVE_MANIPULATION_H_
#define JACO_INTERACTIVE_MANIPULATION_H_

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <interactive_markers/interactive_marker_server.h>
#include <interactive_markers/menu_handler.h>
#include <jaco_ros/ExecuteGraspAction.h>
#include <jaco_ros/JacoFK.h>
#include <sensor_msgs/JointState.h>

class JacoInteractiveManipulation
{

public:
	
	/**
	 * Constructor
	 */
	JacoInteractiveManipulation();

	/**
	 * Process feedback for the interactive marker on the JACO's end effector
	 * @param feedback interactive marker feedback
	 */
	void processHandMarkerFeedback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback);
	
	/**
	 * Callback for the joint state listener
	 * @param msg new joint state message
	 */
	void updateJoints(const sensor_msgs::JointState::ConstPtr& msg);
	
	/**
	 * Update the interactive marker on the JACO's end effector to move based on the
	 * the current joint state of the arm
	 */
	void updateMarkerPosition();

private:

	/**
	 * Create the interactive marker on the JACO's end effector, including pose
	 * controls and menus
	 */
	void makeHandMarker();
	
	/**
	 * Send a 0 velocity command to the robot's arm
	 */
	void sendStopCommand();

	ros::NodeHandle n;
	
	//messages
	ros::Publisher armPoseCommand;
	ros::Publisher cartesianVelCommand;
	ros::Subscriber jointStateSubscriber;
	
	//services
	ros::ServiceClient jacoFkClient;	//forward kinematics
	
	//actionlib
	actionlib::SimpleActionClient<jaco_ros::ExecuteGraspAction> acGrasp;
	
	boost::shared_ptr<interactive_markers::InteractiveMarkerServer> imServer;	//interactive marker server
	interactive_markers::MenuHandler menuHandler;	//interactive marker menu handler
	std::vector<float> joints;	//current joint state
	bool lockPose;	//flag to stop the arm from updating on pose changes, this is used to prevent the slight movement when left clicking on the center of the marker
};

#endif
