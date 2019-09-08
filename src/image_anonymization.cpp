// Copyright (C) 2019  Zhi Yan

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

// ROS
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>

image_transport::Publisher image_pub_;

void rgbImageCallback(const sensor_msgs::ImageConstPtr& image) {
  /*** ros image to opencv image ***/
  cv_bridge::CvImageConstPtr cv_ptr;
  try {
    cv_ptr = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
  } catch(cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  
  image_pub_.publish(cv_ptr->toImageMsg());
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "image_anonymization");

  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber rgb_image_sub = it.subscribe("camera/rgb/image_raw", 100, rgbImageCallback);
  image_pub_ = it.advertise("anonymized_image", 1);
  
  ros::spin();
  return 0;
}
