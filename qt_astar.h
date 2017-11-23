#ifndef QT_ASTAR_H
#define QT_ASTAR_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_astar.h"

#include "centfrom.h"

class Qt_Astar : public QMainWindow
{
	Q_OBJECT

public:
	Qt_Astar(QWidget *parent = 0);
	~Qt_Astar();

public slots:
	void slot_set_time(QString showtime){ui.label_time->setText(showtime);}
private:
	Ui::Qt_AstarClass ui;
};

#endif // QT_ASTAR_H
