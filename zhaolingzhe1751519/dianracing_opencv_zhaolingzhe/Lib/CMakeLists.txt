include(CMakeToolsHelpers OPTIONAL)

cmake_minimum_required(VERSION 2.8)

#添加opencv头文件的搜索路径  
INCLUDE_DIRECTORIES(/usr/include/opencv)
INCLUDE_DIRECTORIES(/usr/include/opencv2)
INCLUDE_DIRECTORIES(/usr/include)

# 查找路径下的所有源文件
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library(OpencvLib ${DIR_LIB_SRCS})

# 添加链接库
TARGET_LINK_LIBRARIES(OpencvLib /usr/lib/libopencv_core.so)
TARGET_LINK_LIBRARIES(OpencvLib /usr/lib/libopencv_highgui.so)
TARGET_LINK_LIBRARIES(OpencvLib /usr/lib/libopencv_imgcodecs.so)
TARGET_LINK_LIBRARIES(OpencvLib /usr/lib/libopencv_imgproc.so)