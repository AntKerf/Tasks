

#include "Cluster.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

/*
Initialization is available via cmd arguments :
-i ".../pic.jpg"		// initialize the bitmap
- p 10 20				// add a point with coordinates x = 10 y = 20
- r 10					// Set the radius of union between points // default 10
- s ".../img/"			// Set the path to save // the default Clusterize Image folder in the .exe folder
- d						// display raster window (opencv)
- h						// show commands
*/

// function for console control
void menu(Clusterize&);
void menuSettings(Clusterize&);


int main(int argc, char* argv[])
{
	Clusterize cl;
	try {
		if (argc > 1)							//opened via console arguments
			cl = Clusterize(argc, argv);
		else {									//opened via .exe without arguments
			menu(cl);
		}
	}
	catch (exception ex) {
		cout << "\nError: " << ex.what() << endl << endl;
		menu(cl);
	}
	system("pause");
	return 0;
}

void menu(Clusterize& cl)
{
	int option = 0;
	cout <<
		"\n1: Set image path" <<
		"\n2: Display image" <<
		"\n3: Add point" <<
		"\n4: Settings" <<
		"\n5: Save" <<
		"\n6: Menu" <<
		"\n7: Exit"
		<< endl;

	cin >> option;
	switch (option)
	{
	case 1:
	{
		string path;
		cout << "Enter path to image "
			"('D:/MyPicture/Example.jpg')\n"
			"Your path to image: ";
		cin >> path;
		cl.setImage(std::move(path));
		menu(cl);
		break;
	}
	case 2:
		cl.draw(true);
		menu(cl);
		break;
	case 3:
	{
		CvPoint p;
		cout << "Enter x:";
		cin >> p.x;
		cout << "Enter y:";
		cin >> p.y;
		cl.addPoint(std::move(p));
		cl.reCombine();
		menu(cl);
		break;
	}
	case 4:
	{
		menuSettings(cl);
		break;
	}
	case 5:
	{
		string answer;
		cout << "Specify the save path ?(y\\n)\n"
			"Current save dir: " << cl.getSaveDir() << endl;
		cin >> answer;
		if (answer == "y") {
			string path;
			cout << "Enter path to save directory: ";
			cin >> path;
			cl.setSaveDir(std::move(path));
			cl.save();
		}
		else if (answer == "n")
		{
			cl.save();
		}
		else
		{
			cout << "Your answer is not recognized :/" << endl;;
		}
		menu(cl);
		break;
	}
	case 6:
		menu(cl);
		break;
	case 7:
		cout << "Good Luck! I`am turn off." << endl;;
		return;
		break;
	default:
		menu(cl);
		break;
	}
}

void menuSettings(Clusterize& cl)
{
	int option = 0;

	cout << std::boolalpha <<
		"\n1: Fast Compute: " << cl.getFlagFastCompute() <<
		"\n2: Merging clusters: " << cl.getFlagMerge() <<
		"\n3: Auto Display: " << cl.getFlagDisplay() <<
		"\n4: Text: " << cl.getFlagText() <<
		"\n5: Icons: " << cl.getFlagIcon() <<
		"\n6: Points: " << cl.getFlagPoints() <<
		"\n7: Lines:" << cl.getFlagLines() <<
		"\n8: Text Scale: " << cl.getTextScale() <<
		"\n9: Radius : " << cl.getRadius() <<
		"\n10: Menu" << endl;

	cin >> option;

	switch (option)
	{
	case 1:
	{
		cl.useFastCompute(!cl.getFlagFastCompute());
		menuSettings(cl);
		break;
	}
	case 2:
	{
		cl.useMerge(!cl.getFlagMerge());
		menuSettings(cl);
		break;
	}
	case 3:
	{
		cl.useDisplay(!cl.getFlagDisplay());
		menuSettings(cl);
		break;
	}
	case 4:
	{
		cl.useDrawingText(!cl.getFlagText());
		menuSettings(cl);
		break;
	}
	case 5:
	{
		cl.useDrawingLines(!cl.getFlagLines());
		menuSettings(cl);
		break;
	}
	case 6:
	{
		cl.useDrawingPoints(!cl.getFlagPoints());
		menuSettings(cl);
		break;
	}
	case 7:
	{
		cl.useDrawingLines(!cl.getFlagLines());
		menuSettings(cl);
		break;
	}
	case 8:
	{
		int _scale;
		cout << "Enter TextScale: ";
		cin >> _scale;
		cl.setTextScale(_scale);
		menuSettings(cl);
		break;
	}
	case 9:
	{
		int _radius;
		cout << "Enter radius:";
		cin >> _radius;
		cl.setRadius(_radius);
		menuSettings(cl);
		break;
	}
	case 10:
	{
		menu(cl);
		break;
	}
	default:
		break;
	}
}