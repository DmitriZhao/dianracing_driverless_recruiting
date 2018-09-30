#include "OpencvTest.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void OpencvTest::runTest()
{
    Mat src = imread("./Lib/dianracing.jpg");

    Mat resu;
    GaussianBlur(src, resu, Size(15,15) ,0 ,0);

    imshow("origin", src);
    imshow("gaussianBlur", resu);
    imwrite("dianracing_result.png",resu);
    cout << "Exported \"dianracing_result.png\"" << endl;
    waitKey(0);

}

OpencvTest::OpencvTest()
{
    cout << "OpencvTest Object Constructed" << endl;
}

OpencvTest::~OpencvTest()
{
    cout << "OpencvTest Object Destructed" << endl;
}
