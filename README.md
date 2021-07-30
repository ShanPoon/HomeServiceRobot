# Project5_HomeServiceRobot
## Setup

```
cd ~/Project5_HomeServiceRobot
catkin_make
source devel/setup.bash
```
## 1. Mapping
```
./src/scripts/test_slam.sh 
```
Using gmapping for mapping our world
<br/>
Using teleop_twist_keyboard to control our robot for driving through the world

## 2. Localization and Navigation
<br/>
2.1 Navigation stack

```
./src/scripts/test_navigation.sh 
```
Using Amcl to localize the robot on the saved map
<br/>
2D Nav Goal to send the goal
<br/>
2.2 Pick Object

```
./test_pick_objects.sh
```
Sending multiple goals for the robot to reach

## 3. Home Service Functions
3.1 Creating the virtual object
it would show the virtual object on the pickup zone, after 5 seconds, it would disappear and appear at the drop off zone
```
./test_add_markers.sh
```

3.2 Home service
First, initialize the node "pick_objects" and state = 0, and the message of goal would be published by this node. 
```
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle n;
  ros::Publisher state_pub = n.advertise<std_msgs::Int8>("state", 100);
  state.data = 0;
  state_pub.publish(state);
```
Next, checking the robot reached the goal or not. If it is reached the first goal, it would publish state = 1 to change the state of virtual object.
```
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
   {
     ROS_INFO("Hooray, the robot has reached the first goal, pick up point: %f,%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
     state.data = 1;
     state_pub.publish(state);
   sleep(5);
   }
```
Finally, after it has reached the first goal. It would send the second goal
```
   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Going to the second goal, drop off point: %f,%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("Hooray, the robot has reached the second goal, drop off point: %f,%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
     state.data = 2;
     state_pub.publish(state);
     sleep(5);
  }
```
