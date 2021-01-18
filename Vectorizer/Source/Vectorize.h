#pragma once
//boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
//opencv
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/types_c.h>
//stl
#include <vector>

class Vectorize
{

private:
	//variable
	cv::Mat image_;

public:

	Vectorize();
	Vectorize(int argc, char* argv[]);
	~Vectorize();

	//methods
	void thinningObjects();
	void thinningObjects(cv::Mat& image);
	void display();

private:
	void thinning(const cv::Mat& in, cv::Mat& out);
	void thinningIteration(cv::Mat& img, int iter);
};