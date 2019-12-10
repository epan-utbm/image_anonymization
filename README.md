# image_anonymization
YOLO-based image anonymization

README for using the image_anonymization

This is a bash script that calls several ROS function packages, python smart recognition packages and launch files. The purpose is to mosaic the face and license plate of the video information of rosbag and re-record it back to a rosbag.

----------------------------------------------------------------------
git clone 
  
----------------------------------------------------------------------


1,Put the four packages BtoP and btop(bag to picture after mosaic on the face),PtoB and ptob(picture to bag) into ~ / catkin_ws / src.Then

----------------------------------------------------------------------
  ~/catkin_ws$ catkin_make
  
----------------------------------------------------------------------
If an error is reported, check the environment configuration to see if OpenCV, roscpp, etc. are configured



2,Put the folder data into ~/,This is where the intermediate data is stored. All paths in the source code start from ~/data/, so the folder data must be placed in the home directory.



3,For the ros package bumblebee_xb3,you can follow this link https://github.com/yzrobot/bumblebee_xb3
Because the three front cameras are special, when you get the picture sequence of the three front cameras, you must go through this package.Uesd in center.launch, left.launch, right.launch



4,Put the yoloface into ~/,Because all places that use yoloface use absolute paths ~/yoloface/.Then configure yoloface,follow this link https://github.com/sthanhng/yoloface
My yoloface source code has been changed from the original version. The original version only detects faces. After detecting the faces, I also perform mosaic processing. So replace utils.py in path ~/yoloface/utils.py with my utils.py



5,Configuration alpr-unconstrained,follow this link https://github.com/sergiomsilva/alpr-unconstrained
Put the alpr-unconstrained into ~/image_anonymization/
Like yoloface ,my alpr-unconstrained source code has been changed.So replace  ~/image_anonymization/alpr-unconstrained/src/drawing_utils.py with my drawing_utils.py



6,After everything is configured, use

----------------------------------------------------------------------
~/privacy_coding$ bash go.sh path

----------------------------------------------------------------------
Inside the path is the rosbag that needs to be processed. This code will recursively process all rosbags in the path.
Please use absolute path



This source code is written for utbm laboratory data. If you use it with other rosbag, you need to change the topic of btop, BtoP, ptob, PtoB, launch file.

Image Anonymization

Light-weight real-time high-accuracy image anonymization.
Face Detection

We use the weights and the config from: https://github.com/sthanhng/yoloface

Download https://github.com/sthanhng/yoloface/tree/master/model-weights and put it into https://github.com/epan-utbm/image_anonymization/blob/master/launch/image_anonymization.launch#L19


Download https://github.com/sthanhng/yoloface/blob/master/cfg/yolov3-face.cfg and put it into https://github.com/epan-utbm/image_anonymization/blob/master/launch/image_anonymization.launch#L20

License Plate Detection

https://github.com/sergiomsilva/alpr-unconstrained

