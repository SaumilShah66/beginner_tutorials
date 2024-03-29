/**
 *MIT License
 *
 *Copyright (c) 2019 Saumil Shah
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

/*
 * @file  talker.cpp
 * @author Saumil Shah
 * @date  28th Oct 2019
 *
 */

#include <sstream>
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
// #include ""
#include "first_pkg/stringService.h"
#include "tf/transform_broadcaster.h"

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */

/**
 * @brief Main function implementation to publish to 'chatter' topic and
 * print the message and run a service simultaneously to change the sring message
 * @param argc Number of parameters passed in command line
 * @param argv Character pointer array pointing to the passed arguments
 * @return 0 When the execution is successful
 * @return 1 When fatal error stream is reached
 */
int main(int argc, char **argv) {
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "talker");

  /// Inializing tf broadcaster
  tf::TransformBroadcaster br;
  tf::Transform transform;

  transform.setOrigin(tf::Vector3(1.0, 1.0, 1.0));
  tf::Quaternion q;
  q.setRPY(1.57, 1.57, 1.57);
  transform.setRotation(q);

  int frequencyVal = 10;  // Frequency in Hz
  std::string name = "Saumil";  // Default name
  bool status = true;

  std::stringstream sss;
  std_msgs::String msgs;

  /// If no arguments are passed
  if (argc == 1) {
    ROS_ERROR_STREAM(
        "You have passed no arguments.Will have to set default values.");
    ROS_INFO_STREAM("Setting name " << name);
    ROS_INFO_STREAM("Setting frequency " << frequencyVal);
  }

  /// If only one argument is passed
  if (argc == 2) {
    ROS_DEBUG_STREAM("1 argument passed");
    ROS_WARN_STREAM(
        "You have not specified frequency. Default frequency will be taken.");
    name = argv[1];
    ROS_INFO_STREAM("Setting name " << name);
    ROS_INFO_STREAM("Setting frequency " << frequencyVal);
  }

  /// If two arguments are passed
  if (argc == 3) {
    ROS_DEBUG_STREAM("2 arguments passed");
    /// Second argument is frequency. Value of frequency cannot be negative or zero
    if (atoi(argv[2]) <= 0) {
      ROS_FATAL_STREAM("Passed argument of frequency cannot work");
      ros::shutdown();
    } else {
      frequencyVal = atoi(argv[2]);
    }
    name = argv[1];
    ROS_INFO_STREAM("Setting name " << name);
    ROS_INFO_STREAM("Setting frequency " << frequencyVal);
  }

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;
  // ros::ServiceClient client = n.serviceClient < first_pkg::stringService
  //     > ("change_string_output");
  // first_pkg::stringService srv;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher chatter_pub = n.advertise < std_msgs::String
      > ("chatter", 1000);

  ros::Rate loop_rate(frequencyVal);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok()) {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::String msg;

    std::stringstream ss;
    // Generate the stream
    ss << "Hello...This is " << name << " " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    // srv.request.choice = status;
    // if(client.call(srv)){
    // msg.data = srv.response.name;
    // ROS_INFO_STREAM(msg);
    // }
    // else{
    // ROS_ERROR_STREAM("Falied service");
    // }
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
    // auto tfMsg = tf::StampedTrasform(transform, ros::Time::now(), "world", "talk");
    br.sendTransform(
        tf::StampedTransform(transform, ros::Time::now(), "world", "talk"));
  }

  return 0;
}

