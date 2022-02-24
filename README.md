# HomeServiceRobot
## Setup

```
cd ~/HomeServiceRobot
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
![test_slam](https://user-images.githubusercontent.com/83933967/127612690-f5e1f459-c3ac-4a6b-8a70-32bb584765cd.gif)


## 2. Localization and Navigation
<br/>
2.1 Navigation stack

```
./src/scripts/test_navigation.sh 
```
Using Amcl to localize the robot on the saved map

![test_navigation](https://user-images.githubusercontent.com/83933967/127612735-72640e72-3d7e-435c-8f8c-c2b2ecdf2bc4.gif)
<br/>
2D Nav Goal to send the goal
<br/>
2.2 Pick Object

```
./test_pick_objects.sh
```
Sending multiple goals for the robot to reach

![pick_object](https://user-images.githubusercontent.com/83933967/127612792-7c0ef6d3-cf43-430c-be00-cd2d14e8f70d.gif)

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
For the markers part, I have built a pick_markers.cpp. The initial state is set of 0, the marker would display on the first goal.
```
  if (state == 0)
  {
   ROS_INFO("%i", msg->data);
   marker.action = visualization_msgs::Marker::ADD;
   marker.pose.position.x = 2.00;
   marker.pose.position.y = 0.200;
   marker.pose.position.z = 0;
   
     
   // Set the scale of the marker -- 1x1x1 here means 1m on a side
   marker.scale.x = 0.2;
   marker.scale.y = 0.2;
   marker.scale.z = 0.2;
   
   // Set the color -- be sure to set alpha to something non-zero!
   marker.color.r = 0.0f;
   marker.color.g = 1.0f;
   marker.color.b = 0.0f;
   marker.color.a = 1.0;
   marker.lifetime = ros::Duration();

 
   marker_pub.publish(marker);
   sleep(1);
   
  }
```
Then, after the robot had arrived at the pick up zone. The State would become 1, and delete the virtual object. 
```
  else if (state == 1)
  {
   ROS_INFO("%i", msg->data);
   marker.action = visualization_msgs::Marker::DELETE;
   marker.lifetime = ros::Duration();

 
   marker_pub.publish(marker);
   sleep(5);
   
  }
```
After 5 second, the robot would go to the drop off zone.
```
  else if (state == 2)
  {
   ROS_INFO("%i", msg->data);
   marker.action = visualization_msgs::Marker::ADD;
   marker.pose.position.x = 3.0;
   marker.pose.position.y = 1.00;
   marker.pose.position.z = 0;
     
   // Set the scale of the marker -- 1x1x1 here means 1m on a side
   marker.scale.x = 0.2;
   marker.scale.y = 0.2;
   marker.scale.z = 0.2;
   
   // Set the color -- be sure to set alpha to something non-zero!
   marker.color.r = 0.0f;
   marker.color.g = 1.0f;
   marker.color.b = 0.0f;
   marker.color.a = 1.0;
   marker.lifetime = ros::Duration();

 
   marker_pub.publish(marker);
   sleep(1);
   
  }
```
## Final result
![home_service](https://user-images.githubusercontent.com/83933967/127612821-749c6ba0-8c0e-4b2e-9b6a-8fcceb6d4914.gif)
