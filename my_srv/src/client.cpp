#include "ros/ros.h"
#include "my_srv/Userint.h"
#include <cstdlib>
#include <ostream>
#include <string>

char command;


int main(int argc, char **argv)
{
	ros::init(argc, argv, "interface_server");
	
	while(1){
		ros::NodeHandle n;
		ros::ServiceClient client= n.serviceClient<my_srv::Userint>("/userint");
		my_srv::Userint comm_srv;
		
		std::cout<<"increase (a) or decrease (d) the velocity:/n insert your choice: ";
		std::cin>>command;

		comm_srv.request.drive =command;
		if(client.call(comm_srv)){
			std::cout<<"the car velocity is: " << comm_srv.response.speed << std::endl;
		
		}else {
		ROS_ERROR("Failed to call service speed");
		return 1;
		}
	}
	return 0;
}


