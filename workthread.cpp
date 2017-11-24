#include "workthread.h"
#include <windows.h>
WorkThread::WorkThread(QObject *parent)
	: QThread(parent)
{
	star = new Astar();

}

WorkThread::~WorkThread()
{

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
void WorkThread::run()
{


    for(int i=0;i<m_wallPoint->size();i++)
    {
        STATE_POINT* point = star->createPoint(m_wallPoint->at(i).x(),m_wallPoint->at(i).y(),NULL,false);
        star->AddtoObstacleList(point);
    }
    STATE_POINT* startpoint = star->createPoint(m_startPoint.x(),m_startPoint.y(),NULL,false);
    STATE_POINT* endpoint = star->createPoint(m_endPoint.x(),m_endPoint.y(),NULL,false);

    STATE_POINT* path = star->FindWay(startpoint,endpoint);
    this->emit sgn_result(path);

}