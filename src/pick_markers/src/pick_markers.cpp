#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Int8.h>
visualization_msgs::Marker marker;
ros::Publisher marker_pub;
/*void odom_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
   float position_x = msg->pose.pose.position.x;
   float position_y = msg->pose.pose.position.y;
   if (position_x == -1.00 && position_y == 1.00)
   {
    ROS_INFO("%f %f", msg->pose.pose.position.x,msg->pose.pose.position.y);
   }

} */

void state_callback(const std_msgs::Int8::ConstPtr& msg)
{
  int state = msg->data;

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
  
  else if (state == 1)
  {
   ROS_INFO("%i", msg->data);
   marker.action = visualization_msgs::Marker::DELETE;
   marker.lifetime = ros::Duration();

 
   marker_pub.publish(marker);
   sleep(5);
   
  }
  
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

}

int main( int argc, char** argv )
{
   ros::init(argc, argv, "pick_markers");
   ros::NodeHandle n;
   ros::Rate r(1);
   marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1) ;
   ros::Subscriber state_sub = n.subscribe("state", 1000, state_callback);

  

   // Set the frame ID and timestamp.  See the TF tutorials for information on these.
   marker.header.frame_id = "/map";
   marker.header.stamp = ros::Time::now();
 
   // Set the namespace and id for this marker.  This serves to create a unique ID
   // Any marker sent with the same namespace and id will overwrite the old one
   marker.ns = "basic_shapes";
   marker.id = 0;
  
   // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, an CYLINDER
   uint32_t shape = visualization_msgs::Marker::CUBE;
   marker.type = shape;
  
   // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
   marker.action = visualization_msgs::Marker::DELETEALL;
   marker_pub.publish(marker);

  
   // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
   /*marker.pose.position.x = -1.00;
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
   sleep(1);*/

   //ros::Subscriber odom_sub = n.subscribe("odom", 1000, odom_callback);



   /*marker.action = visualization_msgs::Marker::DELETE;
   marker.lifetime = ros::Duration(5);
   marker_pub.publish(marker);
   sleep(5);*/


   
   ros::spin();
   return 0;
  

}
  
