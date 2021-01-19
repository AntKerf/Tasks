#include "Vectorize.h"


using namespace std;
using namespace cv;

Vectorize::Vectorize() :
	image_(Mat(500, 500, CV_8UC1(3), Scalar(255, 255, 255))),//new white 500*500 image
	originalImage_(Mat(500, 500, CV_8UC1(3), Scalar(255, 255, 255)))//new white 500*500 image

{
	try {
		WCHAR path[500];
		GetModuleFileNameW(NULL, path, 500); //find .exe path //work in windows only 
		boost::filesystem::path rootDir_ = boost::filesystem::path(path).parent_path(); //directory with .exe 
		//default directory for save images 
		saveDir_ = (boost::filesystem::system_complete(rootDir_.string() + "/Vectorize Images/"));
	}
	catch (Exception& ex) {
		cout << "Init error: " << ex.what() << endl;
	}
	catch (...) {
		cout << "Init error: " << endl;
	}
}

Vectorize::Vectorize(int argc, char* argv[]) : Vectorize()
{
	bool _canDisplay = false;
	for (int i = 0; i < argc; i++) {

		if (strcmp(argv[i], "-i") == 0) {
			//image init
			if (++i < argc)
			{
				setImage(argv[i]);
			}
			else throw std::invalid_argument("Image not found");
		}

		else if (strcmp(argv[i], "-o") == 0) {
			hasObject_ = true;
		}
		else if (strcmp(argv[i], "-c") == 0) {
			hasContour_ = true;
		}
		else if (strcmp(argv[i], "-s") == 0) {
			hasSkelet_ = true;
		}
		else if (strcmp(argv[i], "-!o") == 0) {
			hasObject_ = false;
		}
		else if (strcmp(argv[i], "-!c") == 0) {
			hasContour_ = false;
		}
		else if (strcmp(argv[i], "-!s") == 0) {
			hasSkelet_ = false;
		}
		else if (strcmp(argv[i], "-d") == 0) {
			_canDisplay = true;
		}
		else if (strcmp(argv[i], "-saveDir") == 0) {
			if (++i < argc)
				setSaveDir(argv[i]);
			else throw std::invalid_argument("Save directory not found");
		}
		else if (strcmp(argv[i], "-h") == 0) {
			// printHelp();
		}
	}
	draw();
	if (_canDisplay)
		display();
	save();
}

Vectorize::~Vectorize()
{
	//freeing resources
	image_.release();
	originalImage_.release();
	destroyAllWindows();
}

void Vectorize::setImage(std::string filename)
{
	if (boost::filesystem::is_regular_file(filename))//if real and jpg file
	{
		originalImage_ = imread(filename, CV_8UC1(3)); // Read the file
		threshold(originalImage_, originalImage_, 127, 255, THRESH_BINARY);//only black & white
		name_ = boost::filesystem::path(filename).filename();
	}
	else throw std::invalid_argument("Path to image is invalid");
}

void Vectorize::setSaveDir(std::string saveDirectory)
{
	saveDir_ = boost::filesystem::system_complete(
		boost::filesystem::path(saveDirectory).append("/"));
}

void Vectorize::useSkelet(bool state)
{
	hasSkelet_ = state;
}

void Vectorize::useContour(bool state)
{
	hasContour_ = state;
}


void Vectorize::useObject(bool state)
{
	hasObject_ = state;
}

void Vectorize::display()
{
	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.

	createButton("Object", _objectCallback, this, QT_CHECKBOX, hasObject_);
	createButton("Contour", _contourCallback, this, QT_CHECKBOX, hasContour_);
	createButton("Skelet", _skeletCallback, this, QT_CHECKBOX, hasSkelet_);

	imshow("Display window", image_);
	isDisplayed_ = true;
	while (getWindowProperty("Display window", WND_PROP_VISIBLE)) {
		int key = waitKey(1000); // Wait for a keystroke in the window
		if (key == 27)
			break;
	}
	destroyWindow("Display window");
	isDisplayed_ = false;
}

void Vectorize::save()
{
	if (!boost::filesystem::is_directory(saveDir_))  //create new dir if current dir not found 
		boost::filesystem::create_directory(saveDir_);

	if (name_.empty())//set data\time to image name
	{
		string temp_name = saveDir_.string() + "Vector_" + boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()) + ".jpg";
		if (cv::imwrite(temp_name, image_))
			cout << "Image saved: " << temp_name << endl;
		else
			cout << "Save error" << endl;
	}
	else if (cv::imwrite(saveDir_.string() + name_.string(), image_))
		cout << "Image saved: " << saveDir_.string() + name_.string() << endl;
	else
		cout << "Save error" << endl;
}

void Vectorize::thinningObjects()
{
	Mat _prev = Mat::zeros(originalImage_.size(), CV_8UC1);
	thinning(~originalImage_, _prev);

	if (hasObject_)
		add(image_, _prev, image_);
	else
		addWeighted(image_, 0.5, ~_prev, 0.5, 1, image_);
}

void Vectorize::contouringObjects()
{
	//Extract the contours so that
	vector<vector<Point> > _contours;
	vector<Vec4i> _hierarchy;

	findContours(~originalImage_, _contours, _hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	Mat _cnt_img = Mat::zeros(originalImage_.size(), CV_8UC1);
	drawContours(_cnt_img, _contours, -1, Scalar(255, 255, 255),
		3, LINE_AA, _hierarchy, _contours.size());

	addWeighted(image_, 0.5, ~_cnt_img, 0.5, 1, image_);
}

void Vectorize::draw()
{

	if (hasObject_)
		image_ = originalImage_.clone(); // draw object on result image
	else
		image_ = (Mat(500, 500, CV_8UC1(3), Scalar(255, 255, 255)));

	if (hasContour_)
	{
		contouringObjects();
	}

	if (hasSkelet_)
	{
		thinningObjects();
	}

	if (isDisplayed_)
		imshow("Display window", image_);
}

void Vectorize::thinningIteration(cv::Mat& img, int iter)
{
	CV_Assert(img.channels() == 1);
	CV_Assert(img.depth() != sizeof(uchar));
	CV_Assert(img.rows > 3 && img.cols > 3);

	cv::Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

	int nRows = img.rows;
	int nCols = img.cols;

	if (img.isContinuous()) {
		nCols *= nRows;
		nRows = 1;
	}

	int x, y;
	uchar* pAbove;
	uchar* pCurr;
	uchar* pBelow;
	uchar* nw, * no, * ne;    // north (pAbove)
	uchar* we, * me, * ea;
	uchar* sw, * so, * se;    // south (pBelow)

	uchar* pDst;

	// initialize row pointers
	pAbove = NULL;
	pCurr = img.ptr<uchar>(0);
	pBelow = img.ptr<uchar>(1);

	for (y = 1; y < img.rows - 1; ++y) {
		// shift the rows up by one
		pAbove = pCurr;
		pCurr = pBelow;
		pBelow = img.ptr<uchar>(y + 1);

		pDst = marker.ptr<uchar>(y);

		// initialize col pointers
		no = &(pAbove[0]);
		ne = &(pAbove[1]);
		me = &(pCurr[0]);
		ea = &(pCurr[1]);
		so = &(pBelow[0]);
		se = &(pBelow[1]);

		for (x = 1; x < img.cols - 1; ++x) {
			// shift col pointers left by one (scan left to right)
			nw = no;
			no = ne;
			ne = &(pAbove[x + 1]);
			we = me;
			me = ea;
			ea = &(pCurr[x + 1]);
			sw = so;
			so = se;
			se = &(pBelow[x + 1]);

			int A = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) +
				(*ea == 0 && *se == 1) + (*se == 0 && *so == 1) +
				(*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
				(*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
			int B = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
			int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
			int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				pDst[x] = 1;
		}
	}

	img &= ~marker;
}

void Vectorize::_objectCallback(int pos, void* param)
{
	auto vr = (Vectorize*)param;
	vr->hasObject_ = pos;
	vr->draw();
}

void Vectorize::_contourCallback(int pos, void* param)
{
	auto vr = (Vectorize*)param;
	vr->hasContour_ = pos;
	vr->draw();
}

void Vectorize::_skeletCallback(int pos, void* param)
{
	auto vr = (Vectorize*)param;
	vr->hasSkelet_ = pos;
	vr->draw();
}

/**
 * Function for thinning the given binary image
 *
 * Parameters:
 * 		src  The source image, binary with range = [0,255]
 * 		dst  The destination image
 */
void Vectorize::thinning(const cv::Mat& in, cv::Mat& out)
{
	out = in.clone();
	out /= 255;         // convert to binary image

	cv::Mat prev = cv::Mat::zeros(out.size(), CV_8UC1);
	cv::Mat diff;

	do {
		thinningIteration(out, 0);
		thinningIteration(out, 1);
		cv::absdiff(out, prev, diff);
		out.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	out *= 255;
}
