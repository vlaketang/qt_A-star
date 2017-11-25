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
	connect(ui.pushButton_stop,SIGNAL(clicked()),ui.widget,SLOT(slot_stop()));

	connect(ui.pushButton_pause,SIGNAL(clicked()),ui.widget,SLOT(slot_pause()));
	connect(ui.pushButton_run,SIGNAL(clicked()),ui.widget,SLOT(slot_run()));

	connect(ui.comboBox_FGH,SIGNAL(currentTextChanged(const QString &)),ui.widget,SLOT(slot_FGH(const QString &)));
	connect(ui.comboBox_rate,SIGNAL(currentTextChanged(const QString &)),ui.widget,SLOT(slot_rate(const QString &)));
	connect(ui.comboBox_processess,SIGNAL(currentTextChanged(const QString &)),ui.widget,SLOT(slot_processess(const QString &)));

	connect(ui.comboBox_find,SIGNAL(currentTextChanged(const QString &)),ui.widget,SLOT(slot_usefind(const QString &)));
	connect(ui.comboBox_processess,SIGNAL(currentTextChanged(const QString &)),this,SLOT(slot_enable(const QString &)));
	connect(ui.widget,SIGNAL(sgn_time(QString)),this,SLOT(slot_set_time(QString)));
	connect(ui.comboBox_side,SIGNAL(currentTextChanged(const QString &)),ui.widget,SLOT(slot_setstepSize(const QString &)));

	ui.comboBox_rate->setEnabled(false);
	ui.comboBox_FGH->setEnabled(false);

}

Qt_Astar::~Qt_Astar()
{

}
void Qt_Astar::slot_enable(const QString& text)
{
	if(text == QString::fromLocal8Bit("ÊÇ"))
	{
		ui.comboBox_rate->setEnabled(true);
		ui.comboBox_FGH->setEnabled(true);	

	}else
	{
		ui.comboBox_rate->setEnabled(false);
		ui.comboBox_FGH->setEnabled(false);
	
	}
}