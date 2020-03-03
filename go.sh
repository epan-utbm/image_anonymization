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
function yololeft(){
     for left in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$left --output-dir ~/data/left/
     done

}
function yoloright(){
     for right in `ls $1`
     do
     python3 ~/yoloface/yoloface.py --image $1"/"$right --output-dir ~/data/right/
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
function bayer(){
     for b in `ls $1`
     do
     python rbg2bayer.py $1"/"$b
     done
}
function tongjibb2(){
     for jpg in `ls $1`
     do
     echo $jpg >>/home/z/dat/$2'BB2.dat'
     done
}
function tongjibb3(){
     for jp in `ls $1`
     do
     echo $jp >>/home/z/dat/$2'BB3.dat'
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
               rosrun BtoP BtoP $1"/"$file
               mv /home/z/*.jpg /home/z/data/left0

               rosrun btop btop $1"/"$file
               mv /home/z/*.jpg /home/z/data/right0

               roslaunch /home/z/play/center.launch bag:=$1"/"$file
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg /home/z/data/c

               roslaunch /home/z/play/left.launch bag:=$1"/"$file
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg /home/z/data/l

               roslaunch /home/z/play/right.launch bag:=$1"/"$file
               killall -9 roslaunch
               sleep 5s
               mv /home/z/.ros/*.jpg /home/z/data/r

               chang /home/z/data/c
               sleep 5s
               rosrun data data $1"/"$file
               rm ~/data/c/*
               mv /home/z/*.jpg /home/z/data/c

               yololeft ~/data/left0
               yoloright ~/data/right0
               yoloc ~/data/c
               yolol ~/data/l
               yolor ~/data/r

               cd /home/z/alpr-unconstrained
               make
	       bash /home/z/alpr-unconstrained/run.sh -i ~/data/left -o ~/data/left2 -c ~/data/left2/results.csv
               bash /home/z/alpr-unconstrained/run.sh -i ~/data/right -o ~/data/right2 -c ~/data/right2/results.csv
               bash /home/z/alpr-unconstrained/run.sh -i ~/data/yolol -o ~/data/okl -c ~/data/okl/results.csv
               bash /home/z/alpr-unconstrained/run.sh -i ~/data/yoloc -o ~/data/okc -c ~/data/okc/results.csv
               bash /home/z/alpr-unconstrained/run.sh -i ~/data/yolor -o ~/data/okr -c ~/data/okr/results.csv

               tongjibb2 /home/z/data/left2 $file
               tongjibb3 /home/z/data/okc $file

	       chang /home/z/data/okc
               chang /home/z/data/okl
               chang /home/z/data/okr
               chang /home/z/data/right2
               chang /home/z/data/left2

               cd
               echo $1"/"$file
               echo ~/_$file
               rosrun PtoB PtoB $1"/"$file ~/2_$file
               rosrun ptob ptob $1"/"$file ~/3_$file
               rosrun info1 info1 $1"/"$file ~/2_$file
               rosrun info2 info2 $1"/"$file ~/2_$file
               rm ~/data/yolol/*
               rm ~/data/yoloc/*
               rm ~/data/yolor/*
               rm ~/data/right/*
               rm ~/data/left/*
               rm ~/data/r/*
               rm ~/data/l/*
               rm ~/data/c/*
	       rm /home/z/*.jpg
               rm ~/data/left2/*
               rm ~/data/right2/*
               rm ~/data/okc/*
               rm ~/data/okl/*
               rm ~/data/okr/*
               rm ~/data/right0/*
               rm ~/data/left0/*
            fi
        done
    }   
read_dir $1

              
