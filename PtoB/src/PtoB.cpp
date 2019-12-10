#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <opencv2/highgui/highgui.hpp>
#include"cv_bridge/cv_bridge.h"
#include "std_msgs/String.h"
#include <sstream>
#include <unistd.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace cv;
int main(int argc,char **argv) 
{
    ros::init(argc, argv, "rosbag_recode_node1");
    ros::NodeHandle nh;
    rosbag::Bag bag1,bag2;
    bag1.open(argv[1], rosbag::bagmode::Read);
    std::vector<std::string> topic;
    topic.push_back(std::string("/BB2_9211442/left/image_raw"));
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
    //ros::Rate r(hz);
    foreach(rosbag::MessageInstance const m,view){
   
	
	if(m.getTopic()=="/BB2_9211442/left/image_raw"){



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
		
        cv::Mat frame1,frame2;
	
        //r.sleep();
	//ros::Duration(hz).sleep();
        sleep(hz);
	buf=to_string(a);
	
       
	
       // cv::imshow("image", frame);
        //frame1=imread("/home/z/data/okc/"+buf+".jpg",CV_LOAD_IMAGE_COLOR);
        //frame2=imread("/home/z/data/okl/"+buf+".jpg",CV_LOAD_IMAGE_COLOR);
 	frame1=imread("/home/z/data/left2/"+buf+"l_yoloface.jpg",CV_LOAD_IMAGE_COLOR);
	frame2=imread("/home/z/data/right2/"+buf+"r_yoloface.jpg",CV_LOAD_IMAGE_COLOR);
        
        //cv::imshow("image", frame1);
        //cv::waitKey(100);
        //header.frame_id = "image_frame"; 
       // header.stamp = ros::Time::now();
       a++;
        sensor_msgs::ImagePtr image_msgs1 = cv_bridge::CvImage(header, "bgr8", frame1).toImageMsg();
        sensor_msgs::ImagePtr image_msgs2 = cv_bridge::CvImage(header, "bgr8", frame2).toImageMsg();
        
        bag2.write("/BB2_9211442/left/image_raw", header.stamp, image_msgs1);
        bag2.write("/BB2_9211442/right/image_raw", header.stamp, image_msgs2);
        
 
       
    }
    

       
    
}
    bag2.close();
    bag1.close();
    return 0;
}

