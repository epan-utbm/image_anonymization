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
#include <opencv2/core/core.hpp>
#include <iostream>
#include <sensor_msgs/CameraInfo.h>

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
    topic.push_back(std::string("/BB2_9211442/left/camera_info"));
    //topic.push_back(std::string("/BB2_9211442/right/image_raw"));
    rosbag::View view(bag1,rosbag::TopicQuery(topic));
    
    
    bag2.open(argv[2], rosbag::bagmode::Append);
    
    
    
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
	
	if(m.getTopic()=="/BB2_9211442/left/camera_info"){
	

		sensor_msgs::CameraInfo::Ptr info_msg=m.instantiate<sensor_msgs::CameraInfo>();
		
	std_msgs::Header header;
	header=info_msg->header;
		
        
        sleep(hz);
	buf=to_string(a);
	
       
	
      
        a++;
       
       
        
        bag2.write("/BB2_9211442/left/camera_info", header.stamp, info_msg);
        
       
    }
    
}
    bag2.close();
    bag1.close();
    return 0;
}

