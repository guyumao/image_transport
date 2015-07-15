#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#define PORT 2000
#define IP INADDR_ANY
#define blocksize 8000

using namespace std; 

//static const string WINDOW="Image show";

struct recvbuf
{
	unsigned char buf[blocksize];
	int flag;
} data;

int main(int argc,char* argv[])
{
	int sockfd,n;
	struct sockaddr_in addr;
	
	//UDP SOCKET
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		cout<<"socket created failed"<<endl;
		return -1;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=htonl(IP);

	//OPENCV VIDEO
	cv::VideoCapture cap(0);
	if(!cap.isOpened())
	{
		cout<<"open device failed"<<endl;
		return -1;
	}
	//else cout<<"OK"<<endl;
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	cv::Mat frame=cv::Mat::zeros(240,320,CV_8UC3);
	//cv::namedWindow(WINDOW);

	while(true)
	{	
		cap>>frame;
		if(frame.empty()) break;
		socklen_t size=sizeof(addr);
		//IplImage img=IplImage(frame);
		unsigned char* img=frame.data;
		for(int i=0;i<31;i++)
		{
			for(int k=0;k<blocksize;k++)
				data.buf[k]=img[i*blocksize+k];
			if(i==30) 
			{
				data.flag=2;
				cout<<"sent 1 frame"<<endl;
			}
			else data.flag=1;
			n=sendto(sockfd,(char*)(&data),sizeof(data),0,(struct sockaddr*)&addr,size);
		}
		if(n<0)
		{
			cout<<"sending failed"<<endl;
			return -1;
		}

		//cv::imshow(WINDOW,frame);
		//cv::waitKey(30);
	}
	close(sockfd);
	//cv::destroyWindow(WINDOW);
	return 0;
}
