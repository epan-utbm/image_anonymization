/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2016 Ravi Chalasani (ravich3141@gmail.com)
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the author nor other contributors may be
*     used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/


/*
  bumblebee_xb3 subscribes to image_raw output of the
  camera1394 driver, which receives data in format7_mode rgb8 mode
  from a Bumblebee XB3 camera.

  The received image is deinterlaced and three stereo cameras are set
  up: stereocam_LC, stereocam_CR and stereocam_LR.

  references: camera1394stereo, image_deinterlacer_xb3 
*/

#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <image_transport/camera_publisher.h>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/CameraInfo.h>
#include <camera_info_manager/camera_info_manager.h>

using boost::shared_ptr;
using std::string;
using image_transport::ImageTransport;
using camera_info_manager::CameraInfoManager;

class BumblebeeXB3StereoCamera
{
private:
  ros::NodeHandle base_nh_;
  ros::NodeHandle camera_nh_[2];

  shared_ptr<ImageTransport> it_;

  image_transport::CameraPublisher image_pub_[2];

  shared_ptr<CameraInfoManager> camera_info_mgr_[2];
  sensor_msgs::CameraInfo camera_info_[2];

  sensor_msgs::Image image_msg_[2];

public:
  BumblebeeXB3StereoCamera(ros::NodeHandle base_nh,
                           const string& stereo_camera_name):
    base_nh_(base_nh),
    it_(new ImageTransport(base_nh_))
  {
    const string camera_name[2] = {"left", "right"};
    for(int i=0; i<2; i++)
    {
      camera_nh_[i] = ros::NodeHandle(base_nh_, stereo_camera_name+"/"+camera_name[i]);
      image_pub_[i] = it_->advertiseCamera(stereo_camera_name+"/"+camera_name[i]+"/image_raw", 1);

      ROS_INFO_STREAM("camera_nh_["<<i<<"].getNamespace = " << camera_nh_[i].getNamespace());

      camera_info_mgr_[i] = shared_ptr<CameraInfoManager>(new CameraInfoManager(camera_nh_[i]));
      camera_info_mgr_[i]->setCameraName("bb_xb3_" + stereo_camera_name + "_" + camera_name[i]);

      if(camera_info_mgr_[i]->isCalibrated())
      {
        camera_info_[i] = camera_info_mgr_[i]->getCameraInfo();
        camera_info_[i].header.frame_id = stereo_camera_name;
      }
      else
      {
        camera_info_[i].header.frame_id = stereo_camera_name;
      }
    }
  }

  void publish(sensor_msgs::Image& image_msg_left, sensor_msgs::Image& image_msg_right)
  {
    image_msg_left.header.frame_id  = camera_info_[0].header.frame_id;
    camera_info_[0].header.stamp    = image_msg_left.header.stamp;

    image_msg_right.header.frame_id = camera_info_[1].header.frame_id;
    camera_info_[1].header.stamp    = image_msg_right.header.stamp;

    image_pub_[0].publish(image_msg_left, camera_info_[0]);
    image_pub_[1].publish(image_msg_right, camera_info_[1]);
  }
};

class BumblebeeXB3
{
private:
  enum CameraSelector {RIGHT=0, CENTER=1, LEFT=2};
  ros::NodeHandle camera1394_nh_;
  ros::NodeHandle camera_nh_[3];

  shared_ptr<ImageTransport> it_;

  image_transport::CameraPublisher image_pub_[3];

  shared_ptr<CameraInfoManager> camera_info_mgr_[3];
  sensor_msgs::CameraInfo camera_info_[3];

  sensor_msgs::Image image_msg_[3];

  BumblebeeXB3StereoCamera stereocam_LC_, stereocam_CR_, stereocam_LR_;

public:
  BumblebeeXB3(ros::NodeHandle camera1394_nh):
    camera1394_nh_(camera1394_nh),
    it_(new ImageTransport(camera1394_nh_)),
    stereocam_LC_(camera1394_nh_, "stereo_camera_LC"),
    stereocam_CR_(camera1394_nh_, "stereo_camera_CR"),
    stereocam_LR_(camera1394_nh_, "stereo_camera_LR")
  {
    const string camera_name[3] = {"right", "center", "left"};
    for(int i=0; i<3; i++)
    {
      camera_nh_[i] = ros::NodeHandle(camera1394_nh_, camera_name[i]);
      image_pub_[i] = it_->advertiseCamera(camera_name[i]+"/image_raw", 1);

      camera_info_mgr_[i] = shared_ptr<CameraInfoManager>(new CameraInfoManager(camera_nh_[i]));
      camera_info_mgr_[i]->setCameraName("bb_xb3_" + camera_name[i]);

      if(camera_info_mgr_[i]->isCalibrated())
      {
        camera_info_[i] = camera_info_mgr_[i]->getCameraInfo();
      }
      else
      {
        camera_info_[i].header.frame_id = "bb_xb3_" + camera_name[i];
      }
    }
  }

  void imageCallback(const sensor_msgs::ImagePtr& ros_img_ptr)
  {
    /*
      Deinterlace the image obtained from camera1394
      
      Quote below from the Bumblebee XB3 documentation provided by ptgrey

      """
      Note that the Bumblebee XB3 outputs image data formatted as
      pixel (byte) interleaved stereo images using Format_7. Pixel
      interleaved images use a raw 24bit-per-pixel format. Each sensor
      represents an RGB color channel, where red is from the right
      camera, blue from the left and green from the middle. Also each
      of these is a (GBRG) bayered image. Future firmware versions may
      support line (row) interleaved images, where the rows from each
      of the cameras are interleaved to speed processing.
      source: 2014 version of the "Bumblebee XB3 Getting Started Manual"
      """
     */

    cv_bridge::CvImagePtr cv_img_ptr(new cv_bridge::CvImage);

    try
    {
      cv_img_ptr = cv_bridge::toCvCopy(ros_img_ptr, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // cv::split splits an rgb image into 3 separate images
    std::vector<cv::Mat> split_images_; // TODO: should this be made a (thread unsafe) class variable?
    cv::split(cv_img_ptr->image, split_images_);

    cv_bridge::CvImage cv_img;
    for(int i=0; i<3; i++)
    {
      /*
        First convert cv::Mat to CvImage and then to sensor_msgs::Image
        (see the CvBridge tutorial)
      
        CvBridge will recognize Bayer pattern encodings as having OpenCV
        type 8UC1 (8-bit unsigned, one channel), but it will not perform
        conversions to or from Bayer pattern. This is done instead by the
        image_proc/stereo_image_proc package. 
      */
      cv_img = cv_bridge::CvImage(cv_img_ptr->header,
                                  sensor_msgs::image_encodings::BAYER_GBRG8,
                                  split_images_[i]);

      cv_img.toImageMsg(image_msg_[i]);

      image_msg_[i].header.frame_id = camera_info_[i].header.frame_id;
      camera_info_[i].header.stamp  = ros_img_ptr->header.stamp;

      image_pub_[i].publish(image_msg_[i], camera_info_[i]);
    }

    stereocam_LC_.publish(image_msg_[LEFT],   image_msg_[CENTER]);
    stereocam_CR_.publish(image_msg_[CENTER], image_msg_[RIGHT]);
    stereocam_LR_.publish(image_msg_[LEFT],   image_msg_[RIGHT]);
  }
}; // end of class BumblebeeXB3

int main(int argc, char **argv) {
  ros::init(argc, argv, "bumblebee_xb3");
  ros::NodeHandle nh;
  ros::NodeHandle camera1394_nh("camera"); // TODO get "camera" from a param/argv

  BumblebeeXB3 bb_xb3(camera1394_nh);

  ros::Subscriber camera1394_rgb8_sub = nh.subscribe("camera/image_raw", 
                                                     1, 
                                                     &BumblebeeXB3::imageCallback, 
                                                     &bb_xb3);
  ros::spin();
  return 0;
}
