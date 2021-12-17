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

The controller node contains the main function of the code. It implements the functionality of increasing and decreasing the speed of the robot with the function `bool move(my_srv::Userint::Request &req, my_srv::Userint::Response &res)` . The function takes as parameters the two messages of the service created `Userint.srv`

### The Grabber ###

The robot is equipped with a grabber, capable of picking up a token which is in front of the robot and within 0.4 metres of the robot's centre. To pick up a token, call the `R.grab` method:

```python
success = R.grab()
```

The `R.grab` function returns `True` if a token was successfully picked up, or `False` otherwise. If the robot is already holding a token, it will throw an `AlreadyHoldingSomethingException`.

To drop the token, call the `R.release` method.

Cable-tie flails are not implemented.

### Vision ###

To help the robot find tokens and navigate, each token has markers stuck to it, as does each wall. The `R.see` method returns a list of all the markers the robot can see, as `Marker` objects. The robot can only see markers which it is facing towards.

Each `Marker` object has the following attributes:

* `info`: a `MarkerInfo` object describing the marker itself. Has the following attributes:
  * `code`: the numeric code of the marker.
  * `marker_type`: the type of object the marker is attached to (either `MARKER_TOKEN_GOLD`, `MARKER_TOKEN_SILVER` or `MARKER_ARENA`).
  * `offset`: offset of the numeric code of the marker from the lowest numbered marker of its type. For example, token number 3 has the code 43, but offset 3.
  * `size`: the size that the marker would be in the real game, for compatibility with the SR API.
* `centre`: the location of the marker in polar coordinates, as a `PolarCoord` object. Has the following attributes:
  * `length`: the distance from the centre of the robot to the object (in metres).
  * `rot_y`: rotation about the Y axis in degrees.
* `dist`: an alias for `centre.length`
* `res`: the value of the `res` parameter of `R.see`, for compatibility with the SR API.
* `rot_y`: an alias for `centre.rot_y`
* `timestamp`: the time at which the marker was seen (when `R.see` was called).

For example, the following code lists all of the markers the robot can see:

```python
markers = R.see()
print "I can see", len(markers), "markers:"

for m in markers:
    if m.info.marker_type in (MARKER_TOKEN_GOLD, MARKER_TOKEN_SILVER):
        print " - Token {0} is {1} metres away".format( m.info.offset, m.dist )
    elif m.info.marker_type == MARKER_ARENA:
        print " - Arena marker {0} is {1} metres away".format( m.info.offset, m.dist )
```

[sr-api]: https://studentrobotics.org/docs/programming/sr/
