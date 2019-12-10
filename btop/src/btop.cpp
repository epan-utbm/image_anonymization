#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <opencv2/highgui/highgui.hpp>
#include"cv_bridge/cv_bridge.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace cv;
int main(int argc,char **argv) 
{
    ros::init(argc, argv, "rosbag_recode_node");
    ros::NodeHandle nh;
    rosbag::Bag bag1;
    bag1.open(argv[1], rosbag::bagmode::Read);
    std::vector<std::string> topic;
    topic.push_back(std::string("/BB2_9211442/right/image_raw"));
    rosbag::View view(bag1,rosbag::TopicQuery(topic));
 
    int a=1,b=1;
  
    string buf;
    char cmd[255];
  
    
    foreach(rosbag::MessageInstance const m,view){
	
	if(m.getTopic()=="/BB2_9211442/right/image_raw"){
	

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
        
        cv::Mat frame;
	frame=cv_ptr->image;
      
	
        if (frame.empty()) {
            std::cout << "error, no image" << std::endl;
            return -1;
        }
	buf=to_string(a);
	imwrite(buf+"r.jpg",frame);
	sprintf(cmd,"python3 ~/yoloface/yoloface.py --image %dr.jpg --output-dir ~/data/right/",a);
       
        system(cmd);
	a++;
       
        cv::waitKey(100);
 
       
    }
       
    
    
}
    
    bag1.close();
    return 0;
}



















