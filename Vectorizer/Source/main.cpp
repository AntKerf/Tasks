#include <iostream>
#include <boost/filesystem.hpp>
#include "Vectorize.h"

using namespace std;

// function for console control
void menu(Vectorize&);
void menuSettings(Vectorize&);

int main(int argc, char* argv[])
{
	Vectorize vr;
	try {
		if (argc > 1)							//opened via console arguments
			vr = Vectorize(argc, argv);
		else {									//opened via .exe without arguments
			menu(vr);
		}
	}
	catch (exception ex) {
		cout << "\nError: " << ex.what() << endl << endl;
		menu(vr);
	}
	catch (...)
	{
		cout << "Uknown error" << endl;
		menu(vr);
	}
	return 0;
}

void menu(Vectorize& vr)
{
	int option = 0;
	cout <<
		"1: Set image path\n" <<
		"2: Display image\n" <<
		"3: Settings\n" <<
		"4: Save\n" <<
		"5: Menu\n" <<
		"6: Exit\n"
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
		vr.setImage(std::move(path));
		vr.draw();
		menu(vr);
		break;
	}
	case 2:
		vr.display();
		menu(vr);
		break;
	case 3:
	{
		menuSettings(vr);
		break;
	}
	case 4:
	{
		string answer;
		cout << "Specify the save path ?(y\\n)\n"
			"Current save dir: " << vr.getSaveDir() << endl;
		cin >> answer;
		if (answer == "y") {
			string path;
			cout << "Enter path to save directory: ";
			cin >> path;
			vr.setSaveDir(std::move(path));
			vr.save();
		}
		else if (answer == "n")
		{
			vr.save();
		}
		else
		{
			cout << "Your answer is not recognized :/" << endl;;
		}
		menu(vr);
		break;
	}
	case 5:
	{
		menu(vr);
		break;
	}
	case 6:
	{
		cout << "Good Luck! I`am turn off." << endl;;
		return;
		break;
	}
	default:
		menu(vr);
		break;
	}
}

void menuSettings(Vectorize& vr)
{
	int option = 0;
	cout << std::boolalpha <<
		"\n1: use Skelet:" << vr.getOptionSkelet() <<
		"\n2: use Contour:" << vr.getOptionContour() <<
		"\n3: use Object:" << vr.getOptionObject() <<
		"\n4: Menu"
		<< endl;
	cin >> option;

	switch (option)
	{
	case 1:
	{
		vr.useSkelet(!vr.getOptionSkelet());
		menuSettings(vr);
		break;
	}
	case 2:
	{
		vr.useContour(!vr.getOptionContour());
		menuSettings(vr);
		break;
	}
	case 3:
	{
		vr.useObject(!vr.getOptionObject());
		menuSettings(vr);
		break;
	}
	case 4:
	{
		menu(vr);
		break;
	}
	default:
		menuSettings(vr);
		break;
	}
}