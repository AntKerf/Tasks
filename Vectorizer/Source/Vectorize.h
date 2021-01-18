#pragma once
#include <Windows.h>
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
	cv::Mat originalImage_;
	bool hasSkelet_= true;
	bool hasContour_= false;
	bool hasObject_ = false;
	bool isDisplayed_;

	boost::filesystem::path saveDir_;
	boost::filesystem::path name_;

public:
	Vectorize();
	Vectorize(int argc, char* argv[]);
	~Vectorize();

	//setters
	void setImage(std::string filename);
	void useSkelet(bool state);
	void useContour(bool state);
	void useObject(bool state);

	//methods
	void draw();
	void display();
	void save();

private:
	void thinningObjects();
	void contouringObjects();
	void thinning(const cv::Mat& in, cv::Mat& out);
	void thinningIteration(cv::Mat& img, int iter);

	//UICallbacks
	static void _objectCallback(int pos, void* param);
	static void _contourCallback(int pos, void* param);
	static void _skeletCallback(int pos, void* param);
};