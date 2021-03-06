#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Int8.h>
// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
int main(int argc, char** argv){
  std_msgs::Int8 state;

  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle n;
  ros::Publisher state_pub = n.advertise<std_msgs::Int8>("state", 100);
  state.data = 0;
  state_pub.publish(state);

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = 2;
  goal.target_pose.pose.position.y = 0.2;
  goal.target_pose.pose.orientation.w = 1.0;
  state.data = 0;
  state_pub.publish(state);
   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Going to pick up point: %f,%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
   {
     ROS_INFO("Hooray, the robot has reached the first goal, pick up point: %f,%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
     state.data = 1;
     state_pub.publish(state);
   sleep(5);
   }

  else
    ROS_INFO("The robot failed to pick up");

  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for pick up");
  }


 // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = 3;
  goal.target_pose.pose.position.y= 1.0;
  goal.target_pose.pose.orientation.w = 2.0;

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
  else
    ROS_INFO("The robot failed to pick up");

  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for drop off");
  }


  return 0;
}
