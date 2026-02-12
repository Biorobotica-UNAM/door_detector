# door_detector
This repository provides a ROS2 package to determine whether a door is open or closed using LiDAR readings.

## Description
The node subscribes to `/hardware/base/scan` a 2D LiDAR topic. It compares the average distance in the central region of the scan to a 0.75 meters threshold. If the average distance is greater than the threshold, the door is considered opened.

Additionally, it provides a service to query the current door state.

## Subscription
* `/hardware/base/scan` [`sensor_msg::msg::LaserScan`]

## Server
* `/hardware/door_detector/check` [`std_srvs::srv::Trigger`]
Returns:
 * `success = true` → Door is open
 * `success = false` → Door is closed 
