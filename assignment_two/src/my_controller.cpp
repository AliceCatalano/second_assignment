#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "my_srv/Userint.h"
#include "std_srvs/Empty.h"
#include <string>

float min_l, min_c, min_r;
ros::Publisher pub;
ros::Subscriber sub;
ros::ServiceClient client;
float vel=1.0;
float ang_vel=1.5;

bool move(my_srv::Userint::Request &req, my_srv::Userint::Response &res){
	my_srv::Userint comm_srv;
	std_srvs::Empty srv;
	geometry_msgs::Twist my_vel;
		
	my_vel.linear.x = vel;
	my_vel.angular.z = ang_vel;
	
	printf("increase (a) or decrease (d) the velocity as you like! Be careful when near to the turn.\n the (r) button will bring the robot to initial position.\n");
	if(req.drive=="a"){
		res.speed= vel+0.1*vel;
	}
	else if(req.drive=="d"){
		res.speed= vel-0.1*vel;
	}
	if(req.drive=="r"){
		vel=res.speed;
		ros::NodeHandle nh;
		ros::ServiceClient client = nh.serviceClient<std_srvs::Empty>("/reset_positions");
		std_srvs::Empty srv;
		client.waitForExistence();
		client.call(srv);
		
	}
	vel=res.speed;
	return res.speed;
}

void motoCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{	
	
	//call the client
	my_srv::Userint comm_srv;
	comm_srv.request.drive=1;
	client.call(comm_srv);
	
	int i;
	
	//sectioning the ranges field in 3 sections
	float right= msg->ranges[288];
	float center= msg->ranges[360];
	float left= msg->ranges[576];
	
	geometry_msgs::Twist my_vel;
	my_vel.linear.x = vel;
	my_vel.angular.z = ang_vel;
	
	if(center>1 &&  left>1 && right>1){
		//straight driving
		printf("free path!\n");
		my_vel.linear.x = 2*vel;
		my_vel.angular.z = 0;
	}
	
	else if ((left>1 && center>1 && right<1)||(left<1 && center>1  && right<1)){
		//obstacle on the right
		printf("there's a left turn\n");
		my_vel.linear.x = vel;
		my_vel.angular.z = ang_vel;
	}

	else if ((left>1 && center<1 && right>1)||(left<1 && center<1 && right>1)){
		//should go right
		printf("i should go right\n");
		my_vel.linear.x = vel;
		my_vel.angular.z = -ang_vel;
	}

	else if ((left<1 && center>1 && right>1)||(left>1 && center<1 && right<1)||(left<1 && center<1 && right<1)){
		//check for the nearest obstacle
		
		if(left>right){ //left is free
			printf("take a left turn\n");
			my_vel.linear.x = vel;
			my_vel.angular.z = ang_vel;
		}
		else if(left<right){//right is free
			printf("take a right turn\n");
			my_vel.linear.x = vel;
			my_vel.angular.z = -ang_vel;
		}
	}
	
	pub.publish(my_vel);
	ROS_INFO("Turtle subscriber@[%f, %f, %f]", right, center, left);
	
}

int main (int argc, char **argv)
{
  	ros::init(argc, argv, "turtlebot_subscriber");  
	ros::NodeHandle nh;
	ros::NodeHandle n;
 
	ros::Subscriber sub = nh.subscribe("/base_scan", 1, motoCallback);  
	pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel",1,motoCallback);
	
	ros::ServiceServer service=n.advertiseService("/userint", move);
	ros::ServiceClient client =nh.serviceClient<std_srvs::Empty>("/reset_positions", move);
	std_srvs::Empty srv;
	
	client.waitForExistence();
	client.call(srv);
	ros::spin();
	return 0;
}







