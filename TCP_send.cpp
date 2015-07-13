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
using boost::asio::ip::tcp;
using namespace cv;
using namespace std;

int main(int args, char *argv[])
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
		tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 3200);
		tcp::socket socket(io_service);
		boost::system::error_code ignored_error;
		socket.connect(end_point);
		while (true)
		{
			cap>>frame;
			if (frame.empty())
				break;
			char c=(char)waitKey(100);
			if (c==27)
			{
				break;
			}
			vector<uchar> buff;//buffer for coding
			vector<int> param = vector<int>(2);
			param[0]=CV_IMWRITE_JPEG_QUALITY;
			param[1]=95;//default(95) 0-100
			imencode(".jpg",frame,buff,param);
			std::ostringstream ss;
			ss<<buff.size();
			string headlength=ss.str();
			headlength.resize(16);
			std::size_t length = boost::asio::write(socket, boost::asio::buffer(headlength), boost::asio::transfer_all(), ignored_error);
			std::size_t lengthbody = boost::asio::write(socket, boost::asio::buffer(string(buff.begin(),buff.end())), boost::asio::transfer_all(), ignored_error);
			cout<<"send image finished"<<endl;
		}
		socket.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}	 
	return 0;
}
