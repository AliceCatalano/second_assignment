ROS assignment
================================
Second assignment of the Research Track 1 course. It's a C++ code to controll the movement of a robot inside a the given path and is given the possibility to the user to increase, decrease the velocity of the robot or reset its position to the starting point.

Installing and running
----------------------
First of all the `roscore` command must be executed in one shell.
Each of these command should be run in the workspace repository, and defferent shell can be used
Download (or even better, fork) the repository from: [CarmineD8/second_assignment](https://github.com/CarmineD8/second_assignment) (github.com). 

After you download and build the workspace, you can run the simulation environment with:
```cpp
rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```

Run the controller node, checking the path for your own repository.
```cpp
rosrun assignment_two mycontroller_node
```
Run the client node, giving the possibility to the user to control the speed
```cpp
rosrun my_srv client
```

## Structure
-----------------------------
There are 3 nodes communicating: the controller_node contained in `mycontroller.cpp`, the stage_ros node contained in `my_world.world` and the client node contained in the `client.cpp`.
The user should run them in this order or running the bash file present in this directory.
The logic of the code and how those three nodes communicate is written in the [flowchart](Flowchart.jpg)

### Controller ###

The controller node contains the main function of the code. It implements the functionality of increasing and decreasing the speed of the robot with the function `bool move(my_srv::Userint::Request &req, my_srv::Userint::Response &res)` . The function takes as parameters the two messages (Request and Response) of the service `Userint.srv`, which structure is:

```cpp
string drive
---
float32 speed
```
The `string drive` contains the input form the user given by the client, and depending form the input given there are different actions:

```cpp
if(req.drive=="a"){
		//increasing the speed
		res.speed= vel+0.1*vel;
	}
	else if(req.drive=="d"){
		//decreasing the speed
		res.speed= vel-0.1*vel;
	}
	if(req.drive=="r"){
		//resetting the position
		vel=res.speed;
		ros::NodeHandle nh;
		ros::ServiceClient client = nh.serviceClient<std_srvs::Empty>("/reset_positions");
		std_srvs::Empty srv;
		client.waitForExistence();
		client.call(srv);
```
In the last condition there's the call for the already given service `/reset_positions` that will stop the code and bring the robot in the initial position.

The function to check on the obstacles and let the robot move in the eviroment is `void motoCallback(const sensor_msgs::LaserScan::ConstPtr& msg)`, which takes as parameter the LaserScan message. The useful element of this message is the `ranges[]` array: it contains 720 elements which are the laser detections in a 180° view.
Of those element 3 strategical positions, one for each side, are taken and the conditions to check for obstacles and taking the turn are applied to them. 
The message `geometry_msgs/Twist.h` is used to pubblish the velocity of the robot to the controller, which is the subscriber.

Then there's the main function which initialize all the client and the services.


