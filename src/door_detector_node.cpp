#include <algorithm>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_srvs/srv/trigger.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

namespace door_detector {

class DoorDetectorNode : public rclcpp::Node {
public:
  DoorDetectorNode()
      : Node("door_detector_node"), door_open_(false), door_threshold_(1.20f) {
    sub_scan_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/hardware/base/scan", 10,
        std::bind(&DoorDetectorNode::laserCallback, this, _1));

    srv_check_door_ = this->create_service<std_srvs::srv::Trigger>(
        "check", std::bind(&DoorDetectorNode::checkDoorService, this, _1, _2));

    RCLCPP_INFO(this->get_logger(), "DoorDetectorNode initialized");
  }

private:
  // ROS interfaces
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_scan_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr srv_check_door_;

  // State
  bool door_open_;
  float door_threshold_;

  void laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    const int range = msg->ranges.size();
    if (range == 0)
      return;

    int center = range / 2;
    int span = std::max(1, range / 20);
    int start = std::max(0, center - span);
    int end = std::min(range - 1, center + span);

    float sum = 0.0f;
    int count = 0;

    for (int i = start; i <= end; ++i) {
      float d = msg->ranges[i];
      if (d > 0.0f && d < 4.0f) {
        sum += d;
        count++;
      }
    }

    if (count == 0) {
      door_open_ = true;
      return;
    }

    float avg = sum / count;
    door_open_ = (avg > door_threshold_);
  }

  void
  checkDoorService(const std::shared_ptr<std_srvs::srv::Trigger::Request>,
                   std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
    response->success = door_open_;
    response->message = door_open_ ? "Door is open" : "Door is closed";
  }
};

} // namespace door_detector

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<door_detector::DoorDetectorNode>());
  rclcpp::shutdown();
  return 0;
}
