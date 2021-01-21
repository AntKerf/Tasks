#include "Cluster.h"


Clusterize::Clusterize() : // default settings
	points_(NULL),
	image_(cv::Mat(500, 500, CV_8UC(3), cv::Scalar(255, 255, 255))),//new white 500*500 image
	currentFile_(),
	nameImg_()
{
	try {
		WCHAR path[500];
		GetModuleFileNameW(NULL, path, 500); //find .exe path //work in windows only 
		rootDir_ = boost::filesystem::path(path).parent_path(); //directory with .exe 
		//default directory for save images 
		saveDir_ = (boost::filesystem::system_complete(rootDir_.string() + "/Clusterize Images/"));
		//if config exists
		if (boost::filesystem::is_regular_file(rootDir_.string() + "/ClusterConfig.xml"))
		{
			readConfig(rootDir_.string() + "/ClusterConfig.xml");
		}
		else // create new config file;
		{
			createConfig(rootDir_.string() + "/ClusterConfig.xml");
		}
	}
	catch (Exception& ex) {
		cout << "Init error: " << ex.what() << endl;
	}
	catch (...) {
		cout << "Init error: " << endl;
	}
}

Clusterize::Clusterize(int& argc, char* argv[]) noexcept(false) : Clusterize()
{

	for (int i = 0; i < argc; i++) {

		if (strcmp(argv[i], "-i") == 0) {
			//image init
			if (++i < argc)
			{
				setImage(argv[i]);
			}
			else throw std::invalid_argument("Image not found");
		}
		else if (strcmp(argv[i], "-p") == 0) {
			//point init
			float x;
			float y;
			if (++i < argc && (x = atof(argv[i])))
				if (++i < argc && (y = atof(argv[i])))
					addPoint(x, y);
				else throw std::invalid_argument("Invalid point y argument");
			else throw std::invalid_argument("Invalid point x argument");
		}
		else if (strcmp(argv[i], "-r") == 0) {
			//init radius
			float r_tmp;
			if (++i < argc && (r_tmp = atof(argv[i])))
				setRadius(r_tmp);
			else throw std::invalid_argument("Invalid radius argument");
		}
		else if (strcmp(argv[i], "-s") == 0) {
			//init save path
			if (++i < argc)
				setSaveDir(argv[i]);
			else throw std::invalid_argument("Save directory not found");
		}
		else if (strcmp(argv[i], "-d") == 0) {
			//display win with image 
			FLAG_canDisplay_ = true;
		}
		else if (strcmp(argv[i], "-h") == 0) {
			printHelp();
		}
	}
	combine();
	draw(FLAG_canDisplay_);
	save();
}
Clusterize::~Clusterize() 
{
	//freeing resources
	image_.release();
	destroyAllWindows();
}
void Clusterize::useDisplay(bool state)
{
	FLAG_canDisplay_ = state;
}

void Clusterize::useDrawingText(bool state)
{
	FLAG_hasText_ = state;
}

void Clusterize::useDrawingIcons(bool state)
{
	FLAG_hasIcon_ = state;
}

void Clusterize::useDrawingPoints(bool state)
{
	FLAG_hasPoints_ = state;
}

void Clusterize::useDrawingLines(bool state)
{
	FLAG_hasLines_ = state;
}

void Clusterize::printHelp()
{
	cout << "=================Clusterizer 0.1=====================" << endl;
	cout << "-i 'file path'\t:Select jpg image." << endl;
	cout << "-p x y\t\t:Add point." << endl;
	cout << "-s 'dir path'\t:Specify the save path." << endl;
	cout << "-r x\t\t:Set the point union radius." << endl;
	cout << "-d \t\t:Display win with image." << endl;
	cout << "=====================================================" << endl;
}

int Clusterize::getHeight()
{
	return image_.rows;
}

int Clusterize::getWidth()
{
	return image_.cols;
}

string Clusterize::getSaveDir()
{
	return	saveDir_.string();
}

string Clusterize::getNameImage()
{
	return nameImg_.string();
}

string Clusterize::getCurrentFile()
{
	return currentFile_.string();
}

list<CvPoint> Clusterize::getPoints()
{
	return points_;
}

deque<deque<CvPoint>> Clusterize::getClusters()
{
	return clusters_;
}

deque<CvPoint> Clusterize::getClustersCentres()
{
	return clustersCentres_;
}

void Clusterize::setImage(const string& filename)
{
	if (boost::filesystem::is_regular_file(filename) && _is_jpg(filename))//if real and jpg file
	{
		image_ = imread(filename, IMREAD_COLOR); // Read the file
		nameImg_ = boost::filesystem::path(filename).filename();
		currentFile_ = boost::filesystem::path(filename);
	}
	else throw std::invalid_argument("Path to image is invalid");
}

void Clusterize::setRadius(const int radius)
{
	radius_ = radius;
}

void Clusterize::setSaveDir(const string& path)
{
	saveDir_ = boost::filesystem::system_complete(
		boost::filesystem::path(path).append("/"));
}

void Clusterize::setNameImage(const string& filename)
{
	nameImg_ = filename;
}

void Clusterize::setTextScale(const int scale)
{
	textScale_ = scale;
}

void Clusterize::useFastCompute(bool state)
{
	FLAG_fastCompute_ = state;
}

void Clusterize::useMergingClusters(bool state)
{
	FLAG_hasMerge_ = state;
}

//combine points into a cluster
void Clusterize::combine() noexcept(false)
{

	if (!image_.empty()) {			//image validate
		if (clusters_.empty()) {	//init clusters
			deque<list<CvPoint>::iterator> itForDelete; //points to be removed from list
			deque<CvPoint> newCluster;
			while (!points_.empty())
			{
				newCluster.push_back(points_.front());
				points_.pop_front();
				for (size_t i = 0; i < newCluster.size(); i++) {
					for (auto itPoint = points_.begin(); itPoint != points_.end(); itPoint++)
					{
						//compute distance between points
						if (sqrtf(
							powf(newCluster[i].x - itPoint->x, 2) +
							powf(newCluster[i].y - itPoint->y, 2))
							<= radius_)
						{
							newCluster.push_back(*itPoint);
							itForDelete.push_back(itPoint);
						}
					}
					for (size_t i = 0; i < itForDelete.size(); i++)
					{
						points_.erase(itForDelete[i]);
					}
					itForDelete.clear();
				}
				clusters_.push_back(newCluster);
				newCluster.clear();
			}
		}
		else { //otherwise, map the point to an existing cluster, if clusters already exist	
			for (size_t i = 0; i < points_.size(); i++)
			{
				identifyPoint(points_.front());
				points_.pop_front();
			}
		}
		if (!FLAG_fastCompute_)
		{
			_computeClustersCenter();
		}
	}
	else throw std::logic_error("Image not set.");
}

//parsing a cluster into points and combine()
void Clusterize::reCombine()
{
	for (auto cluster : clusters_)
	{
		for (auto point : cluster)
		{
			points_.push_back(point);
		}
	}
	clusters_.clear();
	if (FLAG_hasMerge_)
		combine();
	else
	{
		for (auto point : points_)
			identifyPoint(point);
		points_.clear();
	}

}

void Clusterize::_computeClustersCenter()
{
	clustersCentres_.clear();
	CvPoint calculatedCenter;
	for (size_t i = 0; i < clusters_.size(); i++)			//all clusters
	{
		calculatedCenter = cvPoint(0, 0);
		for (size_t j = 0; j < clusters_[i].size(); j++)	//all points
		{
			calculatedCenter.x += clusters_[i][j].x;
			calculatedCenter.y += clusters_[i][j].y;
		}
		calculatedCenter.x /= clusters_[i].size();
		calculatedCenter.y /= clusters_[i].size();

		clustersCentres_.push_back(calculatedCenter);
	}
}

float Clusterize::_computeMinDistanceToCluster(const deque<CvPoint>& cluster, const CvPoint& p)
{
	//without STL
	/*
	float min_radius = radius_ + 1;

	for (size_t i = 0; i < cluster.size(); i++)	//all points in clusters
	{
		float compare_radius = (sqrtf(
			powf(cluster[i].x - p.x, 2) +
			powf(cluster[i].y - p.y, 2)));

		if (compare_radius <= min_radius)
			min_radius = compare_radius;

	}
	return min_radius;
	*/

	//with STL
	auto near_point = min_element(cluster.begin(), cluster.end(),
		[&](const CvPoint& a, const CvPoint& b) //lambda
		{
			return ((sqrtf(powf(a.x - p.x, 2) + powf(a.y - p.y, 2))) <
				(sqrtf(powf(b.x - p.x, 2) + powf(b.y - p.y, 2))));

		});
	return (sqrtf(powf(near_point->x - p.x, 2) + powf(near_point->y - p.y, 2)));
}

void Clusterize::readConfig(string path)
{
	try {
		boost::property_tree::ptree Config;
		boost::property_tree::read_xml(path, Config); //read xml in .exe directory
		Config = Config.get_child("SettingsList");
		FLAG_fastCompute_ = Config.get<bool>("fastCompute");
		FLAG_hasMerge_ = Config.get<bool>("merge");
		FLAG_canDisplay_ = Config.get<bool>("dislay");
		FLAG_hasText_ = Config.get<bool>("text");
		FLAG_hasIcon_ = Config.get<bool>("icon");
		FLAG_hasPoints_ = Config.get<bool>("points");
		FLAG_hasLines_ = Config.get<bool>("lines");
		radius_ = Config.get<int>("radius");
		textScale_ = Config.get<int>("textScale");
	}
	//if bad config file - recreate to default
	catch (boost::wrapexcept<boost::property_tree::ptree_bad_path> ex)
	{
		createConfig(path);
	}
}

void Clusterize::createConfig(string path)
{
	boost::property_tree::ptree Config;
	boost::property_tree::ptree Settings;
	Settings.add<bool>("fastCompute", FLAG_fastCompute_);
	Settings.add<bool>("merge", FLAG_hasMerge_);
	Settings.add<bool>("display", FLAG_canDisplay_);
	Settings.add<bool>("text", FLAG_hasText_);
	Settings.add<bool>("icon", FLAG_hasIcon_);
	Settings.add<bool>("points", FLAG_hasPoints_);
	Settings.add<bool>("lines", FLAG_hasLines_);
	Settings.add<int>("radius", radius_);
	Settings.add<int>("textScale", textScale_);
	Config.add_child("SettingsList", Settings);
	auto write_settings = boost::property_tree::xml_writer_make_settings<string>('\t', 1);
	boost::property_tree::write_xml(path, Config,
		std::locale(), write_settings);
}

void Clusterize::identifyPoint(const CvPoint& p)
{
	//find min distance between point and all points in clusters
	auto near_cluster = min_element(clusters_.begin(), clusters_.end(),
		[&](const auto& a, const auto& b) //lambda
		{
			float aR = _computeMinDistanceToCluster(a, p);
			float bR = _computeMinDistanceToCluster(b, p);
			return aR < bR;
		});
	if (near_cluster != clusters_.end() && _computeMinDistanceToCluster(*near_cluster, p) <= radius_)
		near_cluster->push_back(p);
	else
	{
		deque<CvPoint> newCluster{ p };
		clusters_.push_back(newCluster);
	}
	if (!FLAG_fastCompute_)
		_computeClustersCenter();
}

void Clusterize::identifyPoint(float x, float y)
{
	CvPoint newPoint = cvPoint(x, y);
	identifyPoint(newPoint);
}

void Clusterize::addPoint(float x, float y)
{
	points_.push_back(cvPoint(x, y));
}

void Clusterize::addPoint(const CvPoint& p)
{
	points_.push_back(p);
}

void Clusterize::clearClusters()
{
	clusters_.clear();
}

void Clusterize::clearPoint()
{
	if (!points_.empty())
		points_.clear();
}

void Clusterize::draw()
{
	// clear for draw fresh data
	if (!currentFile_.empty())
		image_ = imread(currentFile_.string(), IMREAD_COLOR);
	else
		image_ = Mat(cv::Mat(500, 500, CV_8UC(3), cv::Scalar(255, 255, 255)));


	for (size_t i = 0; i < clusters_.size(); i++)
	{
		RNG rng(clusters_.size() - i);
		Vec3b color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));//random color
		if (FLAG_hasLines_) {

			for (size_t j = 0; j < clusters_[i].size() - 1; j++)
			{
				line(image_, clusters_[i][j], clusters_[i][j + 1],
					color + Vec3b(rng.uniform(50, 100), rng.uniform(50, 100), rng.uniform(50, 100)));
			}
		}

		if (FLAG_hasIcon_) { //draw cluster icon
			if (FLAG_fastCompute_)														//icon on last point of cluster
				circle(image_, clusters_[i][0], radius_ + clusters_[i].size() / 2,
					color - Vec3b(rng.uniform(0, 100), rng.uniform(0, 100), rng.uniform(0, 100)));
			else																	//icon on cluster center 
				circle(image_, clustersCentres_[i], radius_ + clusters_[i].size() / 2,
					color - Vec3b(rng.uniform(0, 100), rng.uniform(0, 100), rng.uniform(0, 100)));
		}

		if (FLAG_hasPoints_) { //draw points
			for (auto point : clusters_[i])
			{
				image_.at<Vec3b>(point) = color;
			}
		}

		if (FLAG_hasText_) { //draw count points in cluster
			if (FLAG_fastCompute_)
			{
				putText(image_,
					to_string(clusters_[i].size()), //count points in cluster
					clusters_[i][0],				//text on last point cluster
					FONT_HERSHEY_DUPLEX,
					(textScale_ / 10.0),
					color - Vec3b(rng.uniform(50, 100), rng.uniform(50, 100), rng.uniform(50, 100)),
					0);
			}
			else {
				putText(image_,
					to_string(clusters_[i].size()), //count points in cluster
					clustersCentres_[i],			//text on cluster center 
					FONT_HERSHEY_DUPLEX,
					(textScale_ / 10.0),
					color - Vec3b(rng.uniform(50, 100), rng.uniform(50, 100), rng.uniform(50, 100)),
					0);
			}
		}
	}
	//display after update
	if (FLAG_canDisplay_) {
		imshow("Display window", image_); // Show our image inside it.
		displayStatusBar("Display window", "Clusters: " + to_string(clusters_.size()), 2000); //display count clusters into status bar
	}
}

void Clusterize::draw(bool display)
{
	FLAG_canDisplay_ = display;

	draw();

	if (display) {
		namedWindow("Display window", CV_GUI_NORMAL); // Create a window for display.

		setMouseCallback("Display window", _myMouseCallback, this);

		createTrackbar("Radius", "Display window", &radius_, image_.cols / 2, _radiusCallback, this); //set MAX value for radius //image_.width()/2 default
		//tools panel
		createTrackbar("Font size", "", &textScale_, 40, _fontCallback, this); //set MAX value for font scale // 40 default
		createButton("Clear", _clearCallback, this, QT_PUSH_BUTTON);
		createButton("Points", _pointsCallback, this, QT_CHECKBOX, FLAG_hasPoints_);
		createButton("Text", _textCallback, this, QT_CHECKBOX, FLAG_hasText_);
		createButton("Clusters", _iconsCallback, this, CV_CHECKBOX, FLAG_hasIcon_);
		createButton("Lines", _linesCallback, this, CV_CHECKBOX, FLAG_hasLines_);
		createButton("Merging clusters", _mergeCallback, this, CV_CHECKBOX, FLAG_hasMerge_);
		createButton("Fast compute", _fastCallback, this, QT_CHECKBOX, FLAG_fastCompute_);

		while (getWindowProperty("Display window", WND_PROP_VISIBLE)) {
			int key = waitKey(1000); // Wait for a keystroke in the window
			if (key == 27)
				break;
		}
		destroyWindow("Display window");
	}
}

void Clusterize::save()
{
	if (!boost::filesystem::is_directory(saveDir_))  //create new dir if current dir not found 
		boost::filesystem::create_directory(saveDir_);

	if (nameImg_.empty())//set data\time to image name
	{
		string temp_name = saveDir_.string() + "Cluster_" + boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()) + ".jpg";
		if (cv::imwrite(temp_name, image_))
			cout << "Image saved: " << temp_name << endl;
		else
			cout << "Save error" << endl;
	}
	else if (cv::imwrite(saveDir_.string() + nameImg_.string(), image_))
		cout << "Image saved: " << saveDir_.string() + nameImg_.string() << endl;
	else
		cout << "Save error" << endl;
}

bool Clusterize::_is_jpg(string filename)
{
	return
		boost::algorithm::iends_with(filename, ".jpg") ||
		boost::algorithm::iends_with(filename, ".jpeg");
}

void Clusterize::_myMouseCallback(int event, int x, int y, int flags, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	switch (flags + event) {
	case CV_EVENT_MOUSEMOVE:
		break;
	case 10: //ctrl + LButtonDown || MButtonFlag + MButtonUP
		if (cl->FLAG_hasMerge_) {
			cl->addPoint(x, y);
			cl->reCombine();
		}
		else {
			cl->identifyPoint(x, y);
		}
		cl->draw();
		return;
		break;
	case CV_EVENT_MBUTTONDOWN:
		/*	cl->deletePoint(x,y);
			cl->draw();
			break;							TODO:add delete point function			*/
	case CV_EVENT_LBUTTONUP:
		break;
	}
}

void Clusterize::_radiusCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->reCombine();
	cl->draw();
}

void Clusterize::_clearCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->clearClusters();
	cl->clearPoint();
	cl->draw();
}

void Clusterize::_pointsCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_hasPoints_ = pos;
	cl->draw();

}

void Clusterize::_textCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_hasText_ = pos;
	cl->draw();

}

void Clusterize::_fontCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->draw();
}

void Clusterize::_iconsCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_hasIcon_ = pos;
	cl->draw();
}

void Clusterize::_linesCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_hasLines_ = pos;
	cl->draw();
}

void Clusterize::_mergeCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_hasMerge_ = pos;
	cl->reCombine();
	cl->draw();
}

void Clusterize::_fastCallback(int pos, void* param)
{
	Clusterize* cl = (Clusterize*)param;
	cl->FLAG_fastCompute_ = pos;
	cl->reCombine();
	cl->draw();
}
