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
float vel=2.0;
float ang_vel=0.0;

bool move(my_srv::Userint::Request &req, my_srv::Userint::Response &res){
	my_srv::Userint comm_srv;
	std_srvs::Empty srv;
	geometry_msgs::Twist my_vel;
		
	my_vel.linear.x = vel;
	my_vel.angular.z = ang_vel;
	
	printf("increase or decrease the velocity as you like! Be careful when near to the turn.\n");
	if(req.drive=="a"){
		res.speed= vel+0.1*vel;
	}
	else if(req.drive=="d"){
		res.speed= vel-0.1*vel;
	}
	if(req.drive=="r"){
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
	float min_r= msg->ranges[0];
	float min_c = msg->ranges[241];
	float min_l= msg->ranges[481];
	
	
	
	//checking and saving the lower value for each section
	for(i=0; i<720; i++){
		while(i<=240){ 
			float right= msg->ranges[i];
			i++;
			if(min_r > right)
				min_r= right;
		}
		while(i<=480){
			float center= msg->ranges[i];
			i++;
			if(min_c > center)
				min_c= center;
		}
		while(i<=720){
			float left= msg->ranges[i];
			i++;
			if(min_l > left)
				min_l= left;
		}
		geometry_msgs::Twist my_vel;
		my_vel.linear.x = vel;
		my_vel.angular.z = ang_vel;
		pub.publish(my_vel);
		ROS_INFO("Turtle subscriber@[%f, %f, %f]", min_r, min_c, min_l);
		if (min_c<1){
			printf("WARNING! you're going to front crush \n");
			vel = 0.0;
			if (min_r < min_l && min_c<0.85){
				//if(min_l>0.85){
					printf("there's a left turn\n");
					vel=0.5;
					ang_vel=1.0;
				//}else{
					my_vel.linear.x=0.0;
				//}
			}
			else if(min_l < min_r && min_c<0.85){
				printf("there's a right turn\n");
				//if(min_r>0.85){
					printf("turning\n");
					ang_vel=-1.0;
					vel=0.5;
				//}else{
					my_vel.linear.x=0.0;
				//}
					
			}
		}else{
			printf("it's ok");
		}	
	}
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







