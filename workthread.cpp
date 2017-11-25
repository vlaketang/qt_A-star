#include "workthread.h"
#include <windows.h>
WorkThread::WorkThread(QObject *parent)
	: QThread(parent),
	m_wallPoint(NULL)
{
	star = new Astar();
	star->setCallback(PointCallback,this);
}

WorkThread::~WorkThread()
{

}
void WorkThread::PointCallback(void* arg,STATE_POINT* point,POINT_TYPE type)
{
	WorkThread *pObj = (WorkThread*)arg;

	STATE_POINT back = {0};
	back.x = point->x;

    back.y = point->y;
    back.moveabel= point->moveabel;

	back.parentDirection = point->parentDirection;
	back.parentDirectionicon = point->parentDirectionicon;

    back.parent= point->parent;

    back.F= point->F;
    back.G= point->G;
    back.H= point->H;
    back.step= point->step;
	pObj->emit sgn_point(back,type);
}





void WorkThread::slot_init(QList<QPoint>* wallPoint, QPoint startPoint,QPoint endPoint,int step_size,int side_len )
{

    star->setStep(step_size);
    star->setSideLen(side_len);
	m_wallPoint = wallPoint;
	m_startPoint = startPoint;
	m_endPoint  = endPoint;

}
void WorkThread::slot_reset()
{
	star->reset();
}
void WorkThread::slot_pause(bool pause)
{
	star->setPause(pause);
}
void WorkThread::slot_control(CONTROL control, const QString &text)
{

	bool enable = false;
	if(control == CONTROL_RATE && text == QString::fromLocal8Bit("¿ì"))
	{
		enable = true;
	}
	if(control != CONTROL_RATE && text == QString::fromLocal8Bit("ÊÇ"))
		enable = true;
	star->setControl(control,enable);
}

void WorkThread::run()
{

	if(m_wallPoint)
	{
		QList<QPoint>::iterator it = m_wallPoint->begin();

		while(it!= m_wallPoint->end())
		{
			STATE_POINT* point = star->createPoint(it->x(),it->y(),NULL,false);
			star->AddtoObstacleList(point);		
			it++;
		}
	}
    STATE_POINT* startpoint = star->createPoint(m_startPoint.x(),m_startPoint.y(),NULL,false);
    STATE_POINT* endpoint = star->createPoint(m_endPoint.x(),m_endPoint.y(),NULL,false);

    STATE_POINT* path = star->FindWay(startpoint,endpoint);
    this->emit sgn_result(path);

}