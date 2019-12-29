// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROS2_OUSTER__OUSTER_DRIVER_HPP_
#define ROS2_OUSTER__OUSTER_DRIVER_HPP_

#include <memory>

#include "ros2_ouster/lifecycle_interface.hpp"

#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_srvs/srv/empty.hpp"
#include "ouster_msgs/srv/get_metadata.hpp"

#include "ros2_ouster/OS1/OS1_sensor.hpp"
#include "ros2_ouster/conversions.hpp"

namespace ros2_ouster
{
/**
 * @class ros2_ouster::OusterDriver
 * @brief A lifecycle interface implementation of a Ouster OS-1 Lidar
 * driver in ROS2.
 */
class OusterDriver : public lifecycle_interface::LifecycleInterface
{
public:
  /**
   * @brief A constructor for ros2_ouster::OusterDriver
   * @param options Node options for lifecycle node interfaces
   */
  OusterDriver(const rclcpp::NodeOptions & options);

  /**
   * @brief A destructor for ros2_ouster::OusterDriver
   */
  ~OusterDriver();

  /**
   * @brief lifecycle node's implementation of configure step
   * which will configure ROS interfaces and allocate resources 
   */
  void onConfigure() override;

  /**
   * @brief lifecycle node's implementation of activate step
   * which will activate ROS interfaces and start processing information
   */
  void onActivate() override;

  /**
   * @brief lifecycle node's implementation of deactivate step
   * which will deactivate ROS interfaces and stop processing information
   */
  void onDeactivate() override;

  /**
   * @brief lifecycle node's implementation of error step
   * which will handle errors in the lifecycle node system
   */
  void onError() override;

  /**
   * @brief lifecycle node's implementation of shutdown step
   * which will shut down the lifecycle node
   */
  void onShutdown() override;

  /**
   * @brief lifecycle node's implementation of cleanup step
   * which will deallocate resources
   */
  void onCleanup() override;

private:
  /**
  * @brief Timer callback to process the UDP socket
  */
  void processData();

  /**
  * @brief service callback to reset the lidar
  * @param request_header Header of rmw request
  * @param request Shared ptr of the Empty request
  * @param response Shared ptr of the Empty response
  */
  void resetService(
    const std::shared_ptr<rmw_request_id_t>/*request_header*/,
    const std::shared_ptr<std_srvs::srv::Empty::Request> request,
    std::shared_ptr<std_srvs::srv::Empty::Response> response);

  /**
  * @brief service callback to get metadata from lidar
  * @param request_header Header of rmw request
  * @param request Shared ptr of the GetMetadata request
  * @param response Shared ptr of the GetMetadata response
  */
  void getMetadata(
    const std::shared_ptr<rmw_request_id_t>/*request_header*/,
    const std::shared_ptr<ouster_msgs::srv::GetMetadata::Request> request,
    std::shared_ptr<ouster_msgs::srv::GetMetadata::Response> response);

  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Image>::SharedPtr _range_im_pub;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Image>::SharedPtr _intensity_im_pub;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Image>::SharedPtr _noise_im_pub;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Imu>::SharedPtr _imu_pub;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::PointCloud2>::SharedPtr _pc_pub;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr _reset_srv;
  rclcpp::Service<ouster_msgs::srv::GetMetadata>::SharedPtr _metadata_srv;

  SensorInterface::SharedPtr _sensor;
  rclcpp::TimerBase::SharedPtr _process_timer;
};

}  // namespace ros2_ouster

#endif  // ROS2_OUSTER__OUSTER_DRIVER_HPP_
