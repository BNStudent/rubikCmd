#ifndef RUBIKCMD_H
#define RUBIKCMD_H

cv::Mat matOriginal;
cv::Mat matProcessed;
cv::Mat matProcessed_color;
	
std::vector<cv::Vec4i> vecLines;
std::vector<cv::Vec4i>::iterator itrLines;

char checkforEscKey = 0;
int threshold  = 100;

std::vector<cv::Point> approx;
std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;
std::vector<cv::Point>::iterator itrPoint;

std::vector<cv::Point> middlePoints;
std::vector<cv::Point>::iterator itrmiddlePoints;


IplImage* GetThresImage(IplImage* imgHSV);
cv::Mat redFilter(const cv::Mat& src);
cv::Mat greenFilter(const cv::Mat& src);
cv::Mat blueFilter(const cv::Mat& src);
cv::Mat yellowFilter(const cv::Mat& src);
//cv::Mat orangeFilter(const cv::Mat& src);
//cv::Mat whiteFilter(const cv::Mat& src);

void Process(cv::Mat& matProcess, cv::Mat& mat_color,bool iscolorDetect);


#endif