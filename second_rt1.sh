#you will need to be in the my_ros_ws directory
sudo roscore &
sudo catkin_make &
rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world &
rosrun assignment_two mycontroller_node &
rosrun my_srv client &