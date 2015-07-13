#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PORT 2000
#define IP INADDR_ANY
#define blocksize 8000

using namespace std; 

struct recvbuf
{
	unsigned char buf[blocksize];
	int flag;
} data;

static const string WINDOW="Image show";

int main(int argc,char* argv[])
{
	int sockfd,n;
	struct sockaddr_in addr;
	//char message[MAXMSG];
	//upd socket
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	int nRecvBuf=1024*1024*2;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	if(sockfd<0)
	{
		cout<<"socket created failed"<<endl;
		return -1;
	}
	else 
		cout<<"socket success"<<endl;
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=htonl(IP);
	if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		cout<<"bind failed"<<endl;
		return -1;
	}
	else 
		cout<<"bind success\n";
	//cv::Mat frame(320,240,CV_8UC3,cv::Scalar::all(0));
	//IplImage img=IplImage(frame);
	while(true)
	{
		socklen_t size=sizeof(addr);
		unsigned char* img=new unsigned char[260000];
		int count=0;
		cv::Mat frame;
		for(int i=0;i<31;i++)
		{
			n=recvfrom(sockfd,(char*)(&data),blocksize+4,0,(struct sockaddr*)&addr,&size);
			if(n<0)
			{
				cout<<"recvform failed"<<endl;
				return -1;
			}
			count+=data.flag;
			for(int k=0;k<blocksize;k++)
				img[i*blocksize+k]=data.buf[k];
			if(data.flag==2)
			{
				cout<<"receive 1 end "<<count<<endl;
				if(count==32)
				{
					IplImage* IMG;
					IMG=cvCreateImageHeader(cvSize(320,240),IPL_DEPTH_8U,3);
					cvSetData(IMG,img,320*3);
					frame=IMG;
					cout<<"receive 1 frame"<<endl;
				}
			}
		}
		delete img;
		cout<<"a"<<endl;
		cv::imshow(WINDOW,frame);
		cv::waitKey(10);
	}
	return 0;
}
