#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <opencv2/highgui/highgui.hpp>
#include"cv_bridge/cv_bridge.h"
#include "std_msgs/String.h"
#include <sstream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace cv;
int main(int argc,char **argv) 
{
    ros::init(argc, argv, "rosbag_recode_node2");
    ros::NodeHandle nh;
    rosbag::Bag bag1,bag2;
    bag1.open(argv[1], rosbag::bagmode::Read);
    std::vector<std::string> topic;
    topic.push_back(std::string("/BBX3_7140017/camera/image_raw"));
    //topic.push_back(std::string("/BB2_9211442/right/image_raw"));
    rosbag::View view(bag1,rosbag::TopicQuery(topic));
    
    
    bag2.open(argv[2], rosbag::bagmode::Write);
    
    
    
    int a=1,num;
    ros::Time t1,t2;
    string buf;
    char cmd[255];
  
    t1=view.getBeginTime();
    t2=view.getEndTime();
    
    
    ros::Duration d1=t2-t1;
    num=view.size();
    double secs =d1.toSec();
    double hz=secs/double(num);
   //ros::Rate r(10);
    foreach(rosbag::MessageInstance const m,view){
	
	if(m.getTopic()=="/BBX3_7140017/camera/image_raw"){
	

		sensor_msgs::Image::Ptr image_msg=m.instantiate<sensor_msgs::Image>();
		cv_bridge::CvImagePtr cv_ptr;
		try{
			cv_ptr=cv_bridge::toCvCopy(image_msg,sensor_msgs::image_encodings::BGR8);
		}
		catch(cv_bridge::Exception& e){
			ROS_ERROR("cv_bridge exception: %s",e.what());
		}
	std_msgs::Header header;
	header=cv_ptr->header;
		
        cv::Mat frame3,frame4,frame5,fin_img;
	//std::cout << hz << std::endl;
        //r.sleep();
	//ros::Duration(0.1).sleep();
       sleep(hz);
	buf=to_string(a);
	
       
	
       // cv::imshow("image", frame);
        
 	
        frame3=imread("/home/z/data/okc/"+buf+".jpg",CV_LOAD_IMAGE_COLOR);
        frame4=imread("/home/z/data/okl/"+buf+".jpg",CV_LOAD_IMAGE_COLOR);
        frame5=imread("/home/z/data/okr/"+buf+".jpg",CV_LOAD_IMAGE_COLOR);
       
        //cv::waitKey(100);
        //header.frame_id = "image_frame"; 
       // header.stamp = ros::Time::now();
        a++;
        std::vector<cv::Mat> channels;
	channels.push_back(frame3);
	channels.push_back(frame4);
	channels.push_back(frame5);
        merge(channels, fin_img);
        cv::imshow("B", fin_img);
        sensor_msgs::ImagePtr image_msgs3 = cv_bridge::CvImage(header, "bgr8", fin_img).toImageMsg();
        //sensor_msgs::ImagePtr image_msgs4 = cv_bridge::CvImage(header, "bgr8", frame4).toImageMsg();
        //sensor_msgs::ImagePtr image_msgs5 = cv_bridge::CvImage(header, "bgr8", frame5).toImageMsg();
        
        bag2.write("/BBX3_7140017/camera/center/image_raw", header.stamp, image_msgs3);
        //bag2.write("/BBX3_7140017/camera/left/image_raw", header.stamp, image_msgs4);
        //bag2.write("/BBX3_7140017/camera/right/image_raw", header.stamp, image_msgs5);
 
       
    }
    
}
    bag2.close();
    bag1.close();
    return 0;
}

