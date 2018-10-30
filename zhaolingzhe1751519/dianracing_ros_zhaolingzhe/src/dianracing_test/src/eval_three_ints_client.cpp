#include "ros/ros.h"
#include "dianracing_test/EvalThreeInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "eval_three_ints_client");
    if(argc != 4)
    {
        ROS_INFO("usage:eval_three_ints_client A B C");
        return 1;
    }

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<dianracing_test::EvalThreeInts>("eval_three_ints");

    dianracing_test::EvalThreeInts srv;
    srv.request.a = atoll(argv[1]);
    srv.request.b = atoll(argv[2]);
    srv.request.c = atoll(argv[3]);
    if(client.call(srv))
    {
        ROS_INFO("Result: %ld",(long)srv.response.result);
    }
    else
    {
        ROS_ERROR("Failed to call service eval_three_ints");
        return 1;
    }
    return 0;
}