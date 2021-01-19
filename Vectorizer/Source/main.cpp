#include <iostream>
#include <boost/filesystem.hpp>
#include "Vectorize.h"

using namespace std;
// function for console control
void menu(Vectorize&);

int main(int argc, char* argv[])
{
	Vectorize vr;
	vr.setImage("D:/Users/Documents/GitHub/Tasks/Vectorizer/Example/someObj.jpg");
	vr.useObject(true);
	vr.useSkelet(true);
	vr.draw();
	vr.display();
	return 0;
}

void menu(Vectorize& vr)
{
	//TODO: need console control
}