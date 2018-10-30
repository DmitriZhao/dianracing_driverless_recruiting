#include "ros/ros.h"
#include "dianracing_test/EvalThreeInts.h"

bool eval(dianracing_test::EvalThreeInts::Request  &req,
          dianracing_test::EvalThreeInts::Response &res)
{
    res.result = (req.a+req.b)*req.c;
    ROS_INFO("Request:  (%ld+%ld)*%ld",(long)req.a,(long)req.b,(long)req.c);
    ROS_INFO("Response: [%ld]",(long)res.result);
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "eval_three_ints_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("eval_three_ints", eval);
    ROS_INFO("Ready to evaluate.");
    ros::spin();

    return 0;
}