#include <fstream>                                                              
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <ctime>
#include <string>
#include <cstdio>
#include <sstream>

#define BYTES 30000
#define IP "127.0.0.1"
#define PORT 2000
using boost::asio::ip::udp;
using namespace cv;
using namespace std;
int main()
{
	cv::VideoCapture cap(0);
	if(!cap.isOpened())
	{
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	Mat frame;
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::udp::socket udp_socket(io_service);  
		boost::asio::ip::udp::endpoint end_point(boost::asio::ip::address::from_string(IP),PORT);
		udp_socket.open(end_point.protocol());  
		while(true)
		{
			cap>>frame;
			if (frame.empty())
				break;
			char c=(char)waitKey(100);
			if (c==27)
			{
				break;
			}
			vector<uchar> buff;
			vector<int> param = vector<int>(2);
			param[0]=CV_IMWRITE_JPEG_QUALITY;
			param[1]=95;
			imencode(".jpg",frame,buff,param);
			udp_socket.send_to(boost::asio::buffer(buff,BYTES),end_point);
			cout<<"send 1 image"<<endl;
		}
	}
	catch(std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}








