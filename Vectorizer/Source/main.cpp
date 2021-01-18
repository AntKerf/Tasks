#include <iostream>
#include <boost/filesystem.hpp>
#include "Vectorize.h"

using namespace std;
// function for console control
void menu(Vectorize&);

int main(int argc, char* argv[])
{
	Vectorize vr;
	vr.thinningObjects();
	vr.display();
	return 0;
}

void menu(Vectorize& vr)
{
	//TODO: need console control
}