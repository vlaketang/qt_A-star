#include "qt_astar.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt_Astar w;
	w.show();
	return a.exec();
}
