#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QThread>
#include "Astar.h"


class WorkThread : public QThread
{
	Q_OBJECT

public:
	WorkThread(QObject *parent);
	~WorkThread();
signals:
	void sgn_result(STATE_POINT* path);
public slots:
	void slot_init(QList<QPoint>* wallPoint,QPoint startPoint,QPoint endPoint,int step_size,int side_len );
	void slot_reset();
protected:
     void run();

private:
	DIRECTION direct;
    QPoint m_startPoint;
    QPoint m_endPoint;
    Astar* star;
    QList<QPoint>* m_wallPoint;
};

#endif // WORKTHREAD_H
