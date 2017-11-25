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

	static void PointCallback(void* arg,STATE_POINT* point,POINT_TYPE type);
signals:
	void sgn_result(STATE_POINT* path);
	void sgn_point(STATE_POINT point,POINT_TYPE type);
public slots:
	void slot_init(QList<QPoint>* wallPoint,QPoint startPoint,QPoint endPoint,int step_size,int side_len );
	void slot_reset();
	void slot_pause(bool pause);
	void slot_control(CONTROL control, const QString &text);

     void run();

private:
	DIRECTION direct;
    QPoint m_startPoint;
    QPoint m_endPoint;
    Astar* star;
    QList<QPoint>* m_wallPoint;
};

#endif // WORKTHREAD_H
