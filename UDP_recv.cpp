#include "opencv2/highgui/highgui.hpp"                                      
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
using boost::asio::ip::udp;
using namespace cv;
using namespace std;
#define IP "127.0.0.1"
#define PORT 2000
#define BYTES 30000
Mat pngimage;
int main()
{
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::udp::socket udp_socket(io_service);  
		boost::asio::ip::udp::endpoint local_point(boost::asio::ip::address::from_string(IP),PORT);
		udp_socket.open(local_point.protocol());  
		udp_socket.bind(local_point); 
		for(;;)
		{
			std::vector<uchar> img(BYTES);
			boost::asio::ip::udp::endpoint send_point;
			udp_socket.receive_from(boost::asio::buffer(img,BYTES), send_point);
			pngimage = imdecode(Mat(img),CV_LOAD_IMAGE_COLOR);
			imshow("jpgopencvclient",pngimage);
			waitKey(10);
		}
		udp_socket.close();
	}
	catch(std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
