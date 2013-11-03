// rubikCmd.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include"rubikCmd.h"

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	cv::VideoCapture capturewebcam(0);

	/*capturewebcam.set(CV_CAP_PROP_FRAME_WIDTH, 240);
	capturewebcam.set(CV_CAP_PROP_FRAME_HEIGHT, 320);*/

	if(capturewebcam.isOpened() == false)
	{
		std::cout<<"Error: webcam is not available now.\n\n";
		return 1;
	}

	cv::namedWindow("Original");
	cv::namedWindow("Processed");
	int pointCount;

	while(checkforEscKey != 27)
	{
		if(capturewebcam.read(matOriginal) == NULL)
		{
			std::cout<<"Error: Frame can not be read.\n\n";
			break;
		}
		pointCount = 0;

		//cv::cvtColor(matOriginal,matOriginal, CV_BGR2GRAY);

		//cv::inRange(matOriginal,cv::Scalar(20, 100, 100),cv::Scalar(30, 255, 255),matProcessed);

		//cv::GaussianBlur(matProcessed,matProcessed,cv::Size(3,3),1.5);

		/**************HOUGH TRANSFORM************/
		//cv::Canny(matOriginal,matProcessed,5,200,3);
		////cv::imshow("Processed",matProcessed);

		//cv::cvtColor(matProcessed,matProcessed_color, CV_GRAY2BGR);
		//cv::imshow("Processed",matProcessed_color);

		//cv::HoughLinesP(matProcessed,vecLines, 1, CV_PI/45, 50, 10, 5);

		//for(itrLines = vecLines.begin();itrLines != vecLines.end();itrLines++)
		//{
		//	cv::line(matOriginal,cv::Point((int)(*itrLines)[0],(int)(*itrLines)[1]),cv::Point((int)(*itrLines)[2],(int)(*itrLines)[3]),cv::Scalar(0,0,255),1,8);
		//}

		
		/**************HOUGH TRANSFORM************/


		/*************CONTOURS*************/

		approx.clear();
		contours.clear();

		matProcessed = redFilter(matOriginal.clone());
		Process(matProcessed,matProcessed_color,true);

		matProcessed = blueFilter(matOriginal.clone());
		Process(matProcessed,matProcessed_color,true);

		cv::cvtColor(matOriginal,matProcessed, CV_BGR2GRAY);
		Process(matProcessed,matProcessed_color,false);

		matProcessed = yellowFilter(matOriginal.clone());
		Process(matProcessed,matProcessed_color,true);

		/*************CONTOURS*************/

		
		/************INRANGE***************/

		/*matProcessed = redFilter(matOriginal);
		cv::imshow("Processed",matProcessed);*/

		/************INRANGE***************/




		//cv::HoughCircles(matProcessed,vecCircles,CV_HOUGH_GRADIENT,matProcessed.rows/4,100,50,10,400);
		
		/*for(itrCircles=vecCircles.begin(); itrCircles != vecCircles.end();itrCircles++)
		{
			std::cout<<"Ball Position: x = " << (*itrCircles)[0] 
								 << ", y = " << (*itrCircles)[1]  
								 << ", r = " << (*itrCircles)[2]  << "\n";


			cv::circle(matOriginal,cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),3,cv::Scalar(0,255,0),CV_FILLED);
			cv::circle(matOriginal,cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),(int)(*itrCircles)[2],cv::Scalar(0,0,255),3);
		}*/
		cv::imshow("Original",matOriginal);
		//cv::imshow("Processed",matProcessed);

		checkforEscKey =cv::waitKey(1);
	}
	capturewebcam.release();
	cvDestroyAllWindows();
	return 0;
}

void Process(cv::Mat& matProcess, cv::Mat& mat_color,bool iscolorDetect)
{
			//cv::cvtColor(matOriginal,matProcessed, CV_BGR2GRAY);
		
		//cv::Laplacian(matProcessed,matProcessed_color,CV_16S);
		
	if(iscolorDetect)
	{
		cv::GaussianBlur(matProcess,matProcess,cv::Size(3,3),1.5);
		cv::threshold(matProcess,mat_color,0,255,CV_THRESH_BINARY);
		cv::Canny(mat_color, mat_color, 0, 50, 3, false);
	}
	else
	{
		cv::cvtColor(matOriginal,matProcessed, CV_BGR2GRAY);
		cv::GaussianBlur(matProcess,matProcess,cv::Size(3,3),1.5);
		cv::threshold(matProcess,mat_color,120,255,CV_THRESH_TOZERO);
		cv::Canny(mat_color, mat_color, 0, 50, 3, false);
	}
		//cv::imshow("Processed",matProcessed_color);
		cv::findContours(mat_color.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < contours.size(); i++)
		{
			// Approximate contour with accuracy proportional
			// to the contour perimeter
			cv::approxPolyDP(
				contours[i], 
				approx, 
				cv::arcLength(cv::Mat(contours[i]), true) * 0.02, 
				true
			);

			// Skip small or non-convex objects 
			if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
				continue;

	

			if (approx.size() == 4 || approx.size() == 5)
			{
				int totalx = 0;
				int totaly = 0;
				int size = contours[i].size();
				for(itrPoint = contours[i].begin(); itrPoint != contours[i].end(); itrPoint++)
				{				
					totalx += (*itrPoint).x;
					totaly += (*itrPoint).y;
				}
			


				cv::circle(matOriginal, cv::Point(totalx/size,totaly/size),3,cv::Scalar(0,0,255));
				
				cv::drawContours(matOriginal,contours,i,cv::Scalar(0,255,0),2,8,hierarchy);
				std::cout<<contours[i].size()<<std::endl;
			}
		}
}

cv::Mat redFilter(const cv::Mat& src)
{
    assert(src.type() == CV_8UC3);

	cv::Mat redOnly;
	cv::Mat imageHsv;
	cv::cvtColor(src, imageHsv, CV_BGR2HSV_FULL);
	int rotation = 128 - 255; // 255 = red
	cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	cv::inRange(imageHsv, cv::Scalar(114, 135, 135), cv::Scalar(142, 255, 255), redOnly);
    return redOnly;

	//cv::Mat imageHsv(src.rows,src.cols,8,3);
	//cv::cvtColor(src, imageHsv, CV_BGR2HSV);
	//cv::Mat threshed(imageHsv.rows,imageHsv.cols,8,1);
	//cv::inRange(imageHsv, cv::Scalar(0, 165, 159), cv::Scalar(4, 254, 220), threshed);
	//cv::imshow("Processed",threshed);
	////int rotation = 128 - 255; // 255 = red
	////cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	//
 //   return threshed;
}

cv::Mat yellowFilter(const cv::Mat& src)
{
 //   assert(src.type() == CV_8UC3);

	//cv::Mat yellowOnly;
	//cv::Mat imageHsv; 
	//cv::cvtColor(src, imageHsv, CV_BGR2HSV_FULL);
	////int rotation = 128 - 255; // 255 = red
	////cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	//cv::inRange(imageHsv,  cv::Scalar(40,41,133),cv::Scalar(65,150,255), yellowOnly);
 //   return yellowOnly;
	cv::Mat imageHsv(src.rows,src.cols,8,3);
	cv::cvtColor(src, imageHsv, CV_BGR2HSV);
	cv::Mat threshed(imageHsv.rows,imageHsv.cols,8,1);
	cv::inRange(imageHsv, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), threshed);
	cv::imshow("Processed",threshed);
	//int rotation = 128 - 255; // 255 = red
	//cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	
    return threshed;
}

cv::Mat greenFilter(const cv::Mat& src)
{
 //   assert(src.type() == CV_8UC3);

	//cv::Mat greenOnly;
	//cv::Mat imageHsv;
	//cv::cvtColor(src, imageHsv, CV_BGR2HSV);
	//int rotation = 128 - 255; // 255 = red
	////cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	//cv::inRange(imageHsv, cv::Scalar(114, 135, 135), cv::Scalar(142, 255, 255), greenOnly);
 //   return greenOnly;
	cv::Mat imageHsv(src.rows,src.cols,8,3);
	cv::cvtColor(src, imageHsv, CV_BGR2HSV);
	cv::Mat threshed(imageHsv.rows,imageHsv.cols,8,1);
	cv::inRange(imageHsv, cv::Scalar(235, 118, 149), cv::Scalar(243, 255, 150), threshed);
	cv::imshow("Processed",threshed);
	//int rotation = 128 - 255; // 255 = red
	//cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	
    return threshed;
}

cv::Mat blueFilter(const cv::Mat& src)
{
	cv::Mat imageHsv(src.rows,src.cols,8,3);
	cv::cvtColor(src, imageHsv, CV_BGR2HSV);
	cv::Mat threshed(imageHsv.rows,imageHsv.cols,8,1);
	cv::inRange(imageHsv, cv::Scalar(109, 140, 68), cv::Scalar(127, 255, 255), threshed);
	cv::imshow("Processed",threshed);
	//int rotation = 128 - 255; // 255 = red
	//cv::add(imageHsv, cv::Scalar(rotation, 0, 0), imageHsv);
	
    return threshed;
}

IplImage* GetThresImage(IplImage* imgHSV)
{
	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(170,160,60), cvScalar(180,256,256), imgThresh); 
	return imgThresh;
}