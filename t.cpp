#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;


int main()
{
	VideoCapture cap("/home/gym/1.rgb");
	if(!cap.isOpened())
	{
		return -1;
	}
	Mat frame;

	bool stop = false;
	while(!stop)
	{
		cap>>frame;
		imshow("当前视频",frame);
		if(waitKey(3000) >=0)
			stop = true;
	}
	return 0;
}
