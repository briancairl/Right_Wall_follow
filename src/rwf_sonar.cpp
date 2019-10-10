#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"
#include <array>
#include <iostream>
#include <cmath>
#include <vector>

class RightWallFollow {

public:
    // Class constructor
    RightWallFollow(ros::NodeHandle n);

    // Right wall follow algorithm
    void move();
	
private:
    // Action message 
    geometry_msgs::Twist action;

    // ROS necessities
    ros::Publisher pub;
    ros::Subscriber reading_front;
    ros::Subscriber reading_right;

    // Threshold where certain actions are taken (in cm)
    float right_thresh = 23;
    float front_thresh = 23;

    // Sensor readings
    float front_range;
    float right_range;

    // Callback functons to get readings from front and right sensors
    void RangeScanCallbackFrontSensor(const sensor_msgs::Range::ConstPtr& frontReading);
    void RangeScanCallbackRightSensor(const sensor_msgs::Range::ConstPtr& rightReading);    
};

RightWallFollow::RightWallFollow(ros::NodeHandle n){

    ROS_INFO("Initializing right wall follow object");

    reading_front = n.subscribe("robot0/sonar_0", 1000, &rwf::RangeScanCallbackFrontSensor,this);

    reading_right = n.subscribe("robot0/sonar_1", 1000, &rwf::RangeScanCallbackRightSensor,this);

    pub = n.advertise<geometry_msgs::Twist>("robot0/cmd_vel",1000);

    ROS_INFO("Initialized");

}

void RightWallFollow::RangeScanCallbackFrontSensor(const sensor_msgs::Range::ConstPtr& frontReading){

    front_range = frontReading->range;

    front_range = front_range*(100.0);

    // for debugging

    // ROS_INFO("value at front: %f cm.",front_range);
}

void RightWallFollow::RangeScanCallbackRightSensor(const sensor_msgs::Range::ConstPtr& rightReading){

    right_range = rightReading->range;

    right_range = right_range*(100.0);

    // For debugging 

    // ROS_INFO("value at right: %f cm.",right_range);
}

void RightWallFollow::move() {

    action.linear.x  = 0.0;
    action.angular.z = 0.0;

    if ((front_range < front_thresh)) { // If you are too close to the wall, turn left

        action.angular.z = 0.25;
        action.linear.x  = 0.0;

    } 
    else if (right_range < right_thresh){
    // If the right sensor reading is below the threshold, turn left AND go forward

        action.angular.z = 0.25;
        action.linear.x = 0.05;        

    } 
    else if (right_range > right_thresh) {
    // If the right sensor reading is above the threshold, turn right AND go forward

        action.angular.z = -0.5;
        action.linear.x  =  0.05;

    }

    // Publish the command
    pub.publish(action);

}

int main(int argc, char **argv){

	// initializes the ros node
	ros::init(argc, argv, "rwf");

	// complete node initialization
	ros::NodeHandle n;

	RightWallFollow test_object(n);

    // Loop at 10
    ros::Rate loop_rate(10);

    while(n.ok()){

    	ros::spinOnce();

        test_object.move();

    	loop_rate.sleep();

    }


}

