#!/bin/bash


function yolol(){
     for l in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$l --output-dir ~/data/yolol/
     done

}
function yolor(){
     for r in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$r --output-dir ~/data/yolor/
     done

}
function yoloc(){
     for c in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$c --output-dir ~/data/yoloc/
     done

}
function yolo(){
     for c in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$c --output-dir ~/test2
     done

}

function chang(){
     a=1
     cd $1
     for p in `ls $1`
     do
     mv $p $a".jpg"
     a=`expr $a + 1`
     done
}



function read_dir(){
        for file in `ls $1`      
        do
            if [ -d $1"/"$file ]  
            then
                read_dir $1"/"$file
            else
               echo $1"/"$file
               
               roslaunch center.launch bag:=$1"/"$file         
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg ~/data/c          
               roslaunch left.launch bag:=$1"/"$file
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg ~/data/l
               roslaunch right.launch bag:=$1"/"$file
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg ~/data/r         
               #Get the picture sequence of the three groups of front cameras 
               

               yoloc ~/data/c
               yolol ~/data/l
               yolor ~/data/r                         
               #Face coding sike processing on three sets of picture sequences 

                                                         
               rosrun BtoP BtoP $1"/"$file                
               rosrun btop btop $1"/"$file            
               #Obtain a sequence of pictures for the two rear cameras, and then perform face mosaic processing


               cd alpr-unconstrained
               make
	       bash run.sh -i ~/data/left -o ~/data/left2 -c ~/data/left2/results.csv
               rm ~/data/left/*
               bash run.sh -i ~/data/right -o ~/data/right2 -c ~/data/right2/results.csv
               rm ~/data/right/*
               bash run.sh -i ~/data/yolol -o ~/data/okl -c ~/data/okl/results.csv
               bash run.sh -i ~/data/yoloc -o ~/data/okc -c ~/data/okc/results.csv
               bash run.sh -i ~/data/yolor -o ~/data/okr -c ~/data/okr/results.csv
               rm ~/data/yolol/*
               rm ~/data/yoloc/*
               rm ~/data/yolor/*
	       chang ~/data/okc
               chang ~/data/okl
               chang ~/data/okr
               cd ..                                  
               #After processing the face mosaic of the five groups of pictures, process the license plate mosaic of the five groups of pictures    


               echo $1"/"$file
               echo ~/_$file
               rosrun PtoB PtoB $1"/"$file ~/_$file
               rosrun ptob ptob $1"/"$file ~/_$file
               #Re-record 5 groups of processed pictures into a rosbag

               rm ~/data/r/*
               rm ~/data/l/*
               rm ~/data/c/*
	       rm *.jpg
               rm ~/data/left2/*
               rm ~/data/right2/*
               rm ~/data/okc/*
               rm ~/data/okl/*
               rm ~/data/okr/*
               #Delete intermediate data
            fi
        done
    }   
read_dir $1

              
