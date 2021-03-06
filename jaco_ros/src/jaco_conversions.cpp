/*
 * jaco_conversions.cpp
 *
 * Common conversions for working with ROS and the JACO api
 *
 *      Author: David Kent
 */
 
#include <jaco_ros/jaco_conversions.h>

using namespace std;

JacoConversions::JacoConversions(void) 
{
    //advertise service
    eqServer = n.advertiseService("jaco_conversions/euler_to_quaternion", &JacoConversions::callEQ, this);
    qeServer = n.advertiseService("jaco_conversions/quaternion_to_euler", &JacoConversions::callQE, this);
}

bool JacoConversions::callEQ(jaco_ros::EulerToQuaternion::Request &req, jaco_ros::EulerToQuaternion::Response &res)
{	
    float t1 = req.roll;
    float t2 = req.pitch;
    float t3 = req.yaw;

    // Calculate the quaternion given roll, pitch, and yaw (rotation order XYZ -- 1:X, 2:Y, 3:Z)
    res.orientation.w = -sin(t1/2.0)*sin(t2/2.0)*sin(t3/2.0) + cos(t1/2.0)*cos(t2/2.0)*cos(t3/2.0);
    res.orientation.x = sin(t1/2.0)*cos(t2/2.0)*cos(t3/2.0) + sin(t2/2.0)*sin(t3/2.0)*cos(t1/2.0);
    res.orientation.y = -sin(t1/2.0)*sin(t3/2.0)*cos(t2/2.0) + sin(t2/2.0)*cos(t1/2.0)*cos(t3/2.0);
    res.orientation.z = sin(t1/2.0)*sin(t2/2.0)*cos(t3/2.0) + sin(t3/2.0)*cos(t2/t1)*cos(t2/2.0);

    return true;
}

bool JacoConversions::callQE(jaco_ros::QuaternionToEuler::Request &req, jaco_ros::QuaternionToEuler::Response &res)
{
    float q1 = req.orientation.w;
    float q2 = req.orientation.x;
    float q3 = req.orientation.y;
    float q4 = req.orientation.z;

    // Calculate necessary elements of the rotation matrix	
    float m11 = pow(q1, 2) + pow(q2, 2) - pow(q3, 2) - pow(q4, 2);
    float m12 = 2*(q2*q3 - q1*q4);
    float m13 = 2*(q2*q4 + q1*q3);
    float m23 = 2*(q3*q4 - q1*q2);
    float m33 = pow(q1, 2) - pow(q2, 2) - pow(q3, 2) + pow(q4, 2);

    // Calculate the roll, pitch, and yaw (rotation order XYZ -- 1:X, 2:Y, 3:Z)
    res.roll = atan2(-m23, m33);
    res.pitch = atan2(m13, sqrt(1 - pow(m13, 2)));
    res.yaw = atan2(-m12, m11);

    return true;
}

int main (int argc, char **argv)
{
    ros::init(argc, argv, "jaco_conversions");

    JacoConversions jc;

    ros::spin();
}

