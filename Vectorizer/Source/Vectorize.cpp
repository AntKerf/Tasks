#include "Vectorize.h"


using namespace std;
using namespace cv;

Vectorize::Vectorize() :
	image_(Mat(500, 500, CV_8UC1(3), Scalar(255, 255, 255)))//new white 500*500 image

{
	image_ = imread("D:/Users/Documents/GitHub/Tasks/Vectorizer/Example/OneObj.jpg", CV_8UC1);
	threshold(image_, image_, 127, 255, THRESH_BINARY);
}

Vectorize::Vectorize(int argc, char* argv[]) : Vectorize()
{
    for (int i = 0; i < argc; i++) {

        if (strcmp(argv[i], "-i") == 0) {
            //image init
            if (++i < argc)
            {
                //setImage(argv[i]);
            }
            else throw std::invalid_argument("Image not found");
        }

        else if (strcmp(argv[i], "-h") == 0) {
           // printHelp();
        }
    }
}

Vectorize::~Vectorize()
{
}

void Vectorize::display()
{
	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.
//	createTrackbar("level", "Display window", &levels_, contours.size(), on_levelTrackbar, this);
//	createTrackbar("Erode", "Display window", &erode_iterations_, 50, on_erodeTrackbar, this);
    imshow("Display window", image_);
	while (getWindowProperty("Display window", WND_PROP_VISIBLE)) {
		int key = waitKey(1000); // Wait for a keystroke in the window
		if (key == 27)
			break;
	}
	destroyWindow("Display window");
}

void Vectorize::thinningObjects()
{
    thinning(~image_, image_);
    image_ = ~image_;
}

void Vectorize::thinningObjects(cv::Mat& image)
{
    thinning(~image, image_);
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
