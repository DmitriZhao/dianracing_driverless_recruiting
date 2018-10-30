## Compile

        cd ./dianracing_ros_zhaolingzhe

        catkin_make
        
        source ./devel/setup.bash
        
        source ./devel/setup.zsh    #alternative for zsh

## Function 1

        rosrun dianracing_test zhaolingzhe_publisher
        
        rosrun dianracing_test zhaolingzhe_subscriber #in a new terminal

## Function 2

        rosrun dianracing_test eval_three_ints_server
        
        #in a new terminal, evaluate (1+2)*3
        
        rosrun dianracing_test eval_three_ints_client 1 2 3