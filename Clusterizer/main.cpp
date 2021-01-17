

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
		"1: Set image path\n" <<
		"2: Display image\n" <<
		"3: Add point\n" <<
		"4: Settings\n" <<
		"5: Save\n" <<
		"6: Menu\n" <<
		"7: Exit\n"
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
		//settingsMenu(cl);
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
