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
//std, stl
#include <iostream>
#include <list>
#include <deque>

using namespace cv;
using namespace std;

class Clusterize
{
private:

	Mat image_;
	list<CvPoint> points_;
	deque<deque<CvPoint>> clusters_;
	deque<CvPoint> clustersCentres_; //for icon & text position // not used for fast calculations

	boost::filesystem::path saveDir_;
	boost::filesystem::path nameImg_;
	boost::filesystem::path currentFile_;
	boost::filesystem::path rootDir_; //directory with .exe in which to look for the config

	//settings will be taken from config
	//initialized in case the config is deleted

	//in the fast compute mode the cluster centers are not calculated.
	//points are connected to the first suitable cluster (not the fact that the nearest one)
	bool FLAG_fastCompute_ = false;
	//if clusters are connected, then they are merged into one // 
	//this flag can turn it off / on
	bool FLAG_hasMerge_ = true;
	//this flag controls the display of the window
	bool FLAG_canDisplay_ = false;
	//this flag controls the rendering of the count points in cluster
	bool FLAG_hasText_ = true;
	//rendering cluster icons
	bool FLAG_hasIcon_ = true;
	//rendering point
	bool FLAG_hasPoints_ = true;
	//rendering lines
	bool FLAG_hasLines_ = false;
	int textScale_ = 5;
	int radius_ = 10;

public:
	//read config & init variables
	Clusterize();
	//execution of console arguments // init from arguments
	Clusterize(int& argc, char* argv[]) noexcept(false);

	//getters
	int getHeight();
	int getWidth();
	string getSaveDir();
	string getNameImage();
	string getCurrentFile();
	list<CvPoint> getPoints();
	deque<deque<CvPoint>> getClusters();
	deque<CvPoint> getClustersCentres();

	//setters
	void setImage(const string& filename);
	void setRadius(const int radius);
	void setSaveDir(const string& path);
	void setNameImage(const string& filename);
	void setTextScale(const int scale);

	void useFastCompute(bool state=false);
	void useMergingClusters(bool state=true);
	void useDisplay(bool state=false);
	void useDrawingText(bool state=true);
	void useDrawingIcons(bool state=true);
	void useDrawingPoints(bool state=true);
	void useDrawingLines(bool state=false);

	//methods
	void printHelp();
	//combine the points from points_ to clusters
	void combine() noexcept(false);
	void reCombine();
	//defines which cluster the point belongs to
	void identifyPoint(const CvPoint& p);
	void identifyPoint(float x, float y);
	//add point to queue points which used in combine/recombine(
	void addPoint(float x, float y);
	void addPoint(const CvPoint& p);
	void clearClusters();
	void clearPoint();
	//draw clusters on image_
	void draw();
	//display if true; draw() anyway
	void draw(bool);
	//save image to saveDir_ with name nameImg_ (.jpg)
	void save();

private:
	//ucalculates cluster centers, not used for quick calculations
	void _computeClustersCenter();
	//returns the minimum distance from a point to a cluster point
	float _computeMinDistanceToCluster(const deque<CvPoint>& cluster, const CvPoint& p);
	//read config file "ClusterConfig.xml" in .exe directory
	void readConfig(string path);
	//creates a config if it is not found or invalid
	void createConfig(string path);

	//UICallbacks
	static bool _is_jpg(string filename);
	static void _myMouseCallback(int event, int x, int y, int flags, void* param);
	static void _radiusCallback(int pos, void* param);
	static void _fontCallback(int pos, void* param);
	static void _clearCallback(int pos, void* param);
	static void _textCallback(int pos, void* param);
	static void _pointsCallback(int pos, void* param);
	static void _iconsCallback(int pos, void* param);
	static void _linesCallback(int pos, void* param);
	static void _mergeCallback(int pos, void* param);
	static void _fastCallback(int pos, void* param);
};

