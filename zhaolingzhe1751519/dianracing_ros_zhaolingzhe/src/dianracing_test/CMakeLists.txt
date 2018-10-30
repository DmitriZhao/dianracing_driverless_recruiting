cmake_minimum_required(VERSION 2.8.3)
project(dianracing_test)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
 )

## Generate services in the 'srv' folder
add_service_files(
  FILES
  EvalThreeInts.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

## The catkin_package macro generates cmake config files for your package
catkin_package(
  INCLUDE_DIRS
  LIBRARIES dianracing_test
  CATKIN_DEPENDS roscpp std_msgs
  DEPENDS system_lib
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(zhaolingzhe_publisher src/zhaolingzhe_publisher.cpp)
target_link_libraries(zhaolingzhe_publisher ${catkin_LIBRARIES})
add_dependencies(zhaolingzhe_publisher ${catkin_EXPORTED_TARGETS})

add_executable(zhaolingzhe_subscriber src/zhaolingzhe_subscriber.cpp)
target_link_libraries(zhaolingzhe_subscriber ${catkin_LIBRARIES})

add_executable(eval_three_ints_server src/eval_three_ints_server.cpp)
target_link_libraries(eval_three_ints_server ${catkin_LIBRARIES})
add_dependencies(eval_three_ints_server dianracing_test_gencpp)

add_executable(eval_three_ints_client src/eval_three_ints_client.cpp)
target_link_libraries(eval_three_ints_client ${catkin_LIBRARIES})
add_dependencies(eval_three_ints_client dianracing_test_gencpp)