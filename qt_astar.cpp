#include "qt_astar.h"

Qt_Astar::Qt_Astar(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    connect(ui.pushButton_selectstart,SIGNAL(clicked()),ui.widget,SLOT(slot_select_start()));
    connect(ui.pushButton_selectend,SIGNAL(clicked()),ui.widget,SLOT(slot_select_end()));
    connect(ui.pushButton_start,SIGNAL(clicked()),ui.widget,SLOT(slot_start_findway()));
    connect(ui.pushButton_selectwall,SIGNAL(clicked()),ui.widget,SLOT(slot_select_wall()));
	connect(ui.pushButton_reset,SIGNAL(clicked()),ui.widget,SLOT(slot_reset()));
	connect(ui.widget,SIGNAL(sgn_time(QString)),this,SLOT(slot_set_time(QString)));
}

Qt_Astar::~Qt_Astar()
{

}
