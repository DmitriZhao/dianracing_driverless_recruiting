#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "zhaolingzhe_publisher");
    ros::NodeHandle n;
    ros::Publisher dianracing_pub = n.advertise<std_msgs::String>("dianracing",1000);
    ros::Rate loop_rate(10);
    int count = 1;
    while (ros::ok())
    {
        std_msgs::String msg;

        std::stringstream ss;
        ss << "driverless gogogo! No." << count;
        msg.data=ss.str();

        ROS_INFO("%s", msg.data.c_str());

        dianracing_pub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }
    return 0;
}
