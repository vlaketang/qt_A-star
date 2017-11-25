#ifndef CENTFROM_H
#define CENTFROM_H

#include <QWidget>
#include "ui_centfrom.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "astar.h"
#include "WorkThread.h"

namespace Ui {
class CentFrom;
}




typedef enum toDo{
	NO_USE,
    SELECT_WALL,
    SELECT_START,
    SELECT_END
}e_TODO;


class CentFrom : public QWidget
{
	Q_OBJECT

public:
	CentFrom(QWidget *parent = 0);
	~CentFrom();
    int run();
	void reset();
	
signals:
    void sgn_brush();
	void sgn_time(QString showtime);
	void sgn_init(QList<QPoint>* wallPoint,QPoint startPoint,QPoint endPoint,int step_size,int side_len );
	void sgn_reset();
	void sgn_pause(bool pause);
	void sgn_control(CONTROL control, const QString &text);
public slots:
    void slot_select_wall();
    void slot_select_start();
    void slot_select_end();
    void slot_start_findway();
	void slot_reset();
	void slot_stop();
	void slot_send_time();
	void slot_setstepSize(const QString & text);
	void slot_result(STATE_POINT* path);
	void slot_point(STATE_POINT point ,POINT_TYPE type);
	void slot_pause();
	void slot_run();
	void slot_FGH(const QString & text);
	void slot_rate(const QString &text);
	void slot_processess(const QString &text);
	void slot_usefind(const QString &text);


protected:
	void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
private:
	Ui::CentFrom ui;
    DIRECTION direct;
    QPoint m_startPoint;
    QPoint m_endPoint;

	STATE_POINT MinFPoint; //��ǰѰ·�ڵ�


	QVector<STATE_POINT> OpenPoint; //��ҪѰ·�Ľڵ�

	QVector<STATE_POINT> ClosePoint;//��Ѱ���Ľڵ�
	 



    QList<QPoint> m_wallPoint; //�ϰ��ڵ�
    e_TODO m_todo;
    QVector<QPoint> m_wayPoint; //Ѱ·���
	int usetime;
	WorkThread* thread;
	QTimer *timer;
	bool working;
	int m_steplen;
	int m_side;
	bool m_pause;
	bool m_FGH;
};

#endif // CENTFROM_H
