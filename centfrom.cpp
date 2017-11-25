#include "centfrom.h"
#include <QTimer>
//#include <qRegisterMetaType>
#include <QToolTip>
#include <windows.h>
CentFrom::CentFrom(QWidget *parent)
	: QWidget(parent),
	working(false),
	thread(NULL),
	timer(new QTimer(this)),
	m_todo(NO_USE),
	m_pause(false),
	m_FGH(false)
{
	m_steplen = 80;
	m_side = 800;
	ui.setupUi(this);
    this->setStyleSheet("background-color:red;");
 
    m_startPoint = QPoint(m_steplen,3*m_steplen);
    m_endPoint = QPoint(7*m_steplen,4*m_steplen);

    m_wallPoint = QList<QPoint>();
    m_wayPoint = QVector<QPoint>();

	OpenPoint = QVector<STATE_POINT>();
	ClosePoint = QVector<STATE_POINT>();
    connect(this, SIGNAL(sgn_brush()), this, SLOT(update()));
	qRegisterMetaType<STATE_POINT>("STATE_POINT");
	qRegisterMetaType<STATE_POINT>("POINT_TYPE");
	qRegisterMetaType<STATE_POINT>("CONTROL");
	
	setMouseTracking(true);
	thread = new WorkThread(this);
	connect(this,SIGNAL(sgn_init(QList<QPoint>* ,QPoint ,QPoint,int ,int )),thread,SLOT(slot_init(QList<QPoint>* ,QPoint ,QPoint,int,int )));
	connect(this,SIGNAL(sgn_reset()),thread,SLOT(slot_reset()));
	connect(this,SIGNAL(sgn_pause(bool)),thread,SLOT(slot_pause(bool)));
	connect(this,SIGNAL(sgn_control(CONTROL,const QString &)),thread,SLOT(slot_control(CONTROL,const QString &)));
	connect(thread, SIGNAL(sgn_result(STATE_POINT*)), this, SLOT(slot_result(STATE_POINT*)));
	connect(thread, SIGNAL(sgn_point(STATE_POINT,POINT_TYPE)), this, SLOT(slot_point(STATE_POINT,POINT_TYPE)));
}


CentFrom::~CentFrom()
{

}

void CentFrom::slot_FGH(const QString & text)
{
	bool enable = false;
	if(text == QString::fromLocal8Bit("是"))
		enable = true;
	this->m_FGH = enable;

}
void CentFrom::slot_rate(const QString &text)
{
	this->emit sgn_control(CONTROL_RATE, text);
}
void CentFrom::slot_processess(const QString &text)
{
	this->emit sgn_control(CONTROL_PROCESSESS, text);
}
void CentFrom::slot_usefind(const QString &text)
{
	this->emit sgn_control(CONTROL_FUNCTION, text);
}
void CentFrom::slot_setstepSize(const QString &text)
{
	QString box = text.split('x').first();

	m_steplen = m_side/box.toInt();
	slot_reset();
	m_wallPoint.clear();
	m_wayPoint.clear();
	this->emit sgn_brush();
}

void CentFrom::slot_select_wall()
{
    m_todo = SELECT_WALL;

}
void CentFrom::slot_select_start()
{
    m_todo = SELECT_START;
}
void CentFrom::slot_select_end()
{
    m_todo = SELECT_END;

}
void CentFrom::slot_start_findway()
{
	usetime = 0;
	working = true;

	connect(timer, SIGNAL(timeout()), this, SLOT(slot_send_time()));
	timer->start(1000);
	if(!thread)
	{
		thread = new WorkThread(this);
		connect(this,SIGNAL(sgn_init(QList<QPoint>* ,QPoint ,QPoint,int ,int )),thread,SLOT(slot_init(QList<QPoint>* ,QPoint ,QPoint,int,int )));
		connect(this,SIGNAL(sgn_reset()),thread,SLOT(slot_reset()));
		connect(this,SIGNAL(sgn_pause(bool)),thread,SLOT(slot_pause(bool)));
		connect(this,SIGNAL(sgn_control(CONTROL,const QString &)),thread,SLOT(slot_control(CONTROL,const QString &)));
		connect(thread, SIGNAL(sgn_result(STATE_POINT*)), this, SLOT(slot_result(STATE_POINT*)));
		connect(thread, SIGNAL(sgn_point(STATE_POINT,POINT_TYPE)), this, SLOT(slot_point(STATE_POINT,POINT_TYPE)));
	}
	this->emit sgn_init(&m_wallPoint,m_startPoint,m_endPoint,m_steplen,m_side);

    thread->start();


}

void CentFrom::slot_point(STATE_POINT point ,POINT_TYPE type)
{

	//printf("point back,x:%d,y:%d, %d\n",point.x,point.y,type);
	switch (type)
	{
	case MIN_F_POINT:
		if(!point.parent)
			return;
		MinFPoint = point;
		for(int i=0; i < OpenPoint.size();)
		{
			if(OpenPoint[i].x == point.x && OpenPoint[i].y == point.y)
			{
				OpenPoint.remove(i);
			}
			i++;
		}
		break;
	case NEW_SEARCH_POINT:
		if((point.x ==  m_endPoint.x() && point.y ==  m_endPoint.y()) ||
				(point.x ==  m_startPoint.x() && point.y ==  m_startPoint.y()))
			return;
		OpenPoint.append(point);
		break;

	case CLOSE_POINT:
		if((point.x ==  m_endPoint.x() && point.y ==  m_endPoint.y()) ||
			(point.x ==  m_startPoint.x() && point.y ==  m_startPoint.y()))
			return;
		for(int i=0; i < OpenPoint.size();)
		{
			if(OpenPoint[i].x == point.x && OpenPoint[i].y == point.y)
			{
				OpenPoint.remove(i);
			}
			i++;
		}
		ClosePoint.append(point);
		break;
	default:
		break;
	}
	this->emit sgn_brush();
}

void CentFrom::slot_result(STATE_POINT* path)
{
    if(path)
    {
        path = path->parent;
        while(path)
        {
            if(path->parent)
            {
                QPoint way;
                way.setX(path->x);
                way.setY(path->y);
                m_wayPoint.append(way);
				for(int i=0; i < ClosePoint.size();)
				{
					if(ClosePoint[i].x == path->x && ClosePoint[i].y == path->y)
					{
						ClosePoint.remove(i);
					}
					i++;
				}


            }
            path = path->parent;
        }
		memset(&MinFPoint,0,sizeof(STATE_POINT));
		MinFPoint.x = -1;
		MinFPoint.y = -1;

        this->emit sgn_brush();
    }
	usetime = 0;
	timer->stop();
	disconnect(timer, SIGNAL(timeout()), this, SLOT(slot_send_time()));
	working = false;
	this->emit sgn_reset();
}
void CentFrom::slot_send_time()
{
	usetime+=1;
	QString showtime("");
	int hours = usetime/(60*60);

	int minute =(usetime%(60*60))/60;
	int second = ((usetime%(60*60*60))%(60*60))%60;
	showtime = QString::number(hours) + ":" + QString::number(minute) +":"+QString::number(second);

	this->emit sgn_time(showtime);
}
void CentFrom::slot_reset()
{
	this->emit sgn_reset();

    m_wayPoint.clear();
	OpenPoint.clear();
	ClosePoint.clear();
	memset(&MinFPoint,0,sizeof(STATE_POINT));
	MinFPoint.x = -1;
	MinFPoint.y = -1;
	this->emit sgn_brush();
	this->emit sgn_time(QString("00:00:00"));
	
}
void CentFrom::slot_stop()
{
	if(working)
	{
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), this, SLOT(slot_send_time()));
		if(thread)
		{
			//printf("stop thread\n");
			thread->exit();
		}
		working = false;
	}
}

void CentFrom::slot_pause()
{
	this->emit sgn_pause(true);
}
void CentFrom::slot_run()
{
	this->emit sgn_pause(false);
}


void CentFrom::mousePressEvent(QMouseEvent *event)
{
    QPoint press;

    press.setX((event->pos().x()/m_steplen)*m_steplen);
    press.setY((event->pos().y()/m_steplen)*m_steplen);

	if(m_todo != NO_USE)
		event->accept();

    switch(m_todo)
    {
    case SELECT_WALL:

        if(event->button() == Qt::LeftButton)
        {
            if(!m_wallPoint.contains(press))
                m_wallPoint.append(press);
            else
                return;

        }else if(event->button() == Qt::RightButton)
        {
            if(m_wallPoint.contains(press))
                m_wallPoint.removeOne(press);
            else
               return;
        }
        break;
    case SELECT_START:
        m_startPoint = press;
        break;
    case SELECT_END:
        m_endPoint = press;
        break;
    }
	if(m_todo != NO_USE)
		this->emit sgn_brush();
}
void CentFrom::paintEvent(QPaintEvent *event)
{
	//printf("121212");

    int x,y;
    QPainter* painter = new QPainter(this);
   // painter->drawRect(0,0,700,700);

    for(int i = 0; i<= m_side/m_steplen;i++)
    {
        painter->drawLine(QPoint(i*m_steplen,0),QPoint(i*m_steplen,m_side));
        painter->drawLine(QPoint(0,i*m_steplen),QPoint(m_side,i*m_steplen));
    }

	//绘制起点
    QBrush brushstart(QColor(255,0,0));
    painter->setBrush(brushstart);
    x = m_startPoint.x();
    y = m_startPoint.y();
    painter->drawRect(x,y,m_steplen,m_steplen);

	//绘制目的地
    QBrush brushend(QColor(0,0,255));
    painter->setBrush(brushend);

    x = m_endPoint.x();
    y = m_endPoint.y();
    painter->drawRect(x,y,m_steplen,m_steplen);




	//寻路结果
    for(int i = 0;i<m_wayPoint.size();i++)
    {
		QBrush brushway(QColor(0,255,0));
		painter->setBrush(brushway);

        x=m_wayPoint[i].x();
        y = m_wayPoint[i].y();
        painter->drawRect(x,y,m_steplen,m_steplen);

		if(m_FGH)
		{
			QBrush brushstep(QColor(105,139,105));
			painter->setBrush(brushstep);
			painter->drawText(x,y+(m_steplen/2),QString::number(i));
		}
    }





	
	//将要查询的节点
	for(int i=0;i<OpenPoint.size();i++)
	{
		QBrush brushopen(QColor(152,251,152));
		painter->setBrush(brushopen);
		x= OpenPoint[i].x;
		y= OpenPoint[i].y;
		painter->drawRect(x,y,m_steplen,m_steplen);

		if(m_FGH)
		{
			QBrush brush(QColor(0,0,0));
			painter->setBrush(brush);
			//绘制F
			painter->drawText(x,y+m_steplen/3,QString::number(OpenPoint[i].F/m_steplen));
			//绘制G
			painter->drawText(x,y+m_steplen,QString::number(OpenPoint[i].G/m_steplen));
			//绘制H
			painter->drawText(x+2*(m_steplen/3),y+m_steplen,QString::number(OpenPoint[i].H/m_steplen));
			//绘制step
			painter->drawText(x+2*(m_steplen/3),y+m_steplen/3,QString::number(OpenPoint[i].step));
			//绘制父节点方向
			painter->drawText(x+m_steplen/3,y+m_steplen/3,QString(OpenPoint[i].parentDirectionicon));
		}
	}

	//绘制当前查询节点
	if(MinFPoint.x != -1 && MinFPoint.y != -1)
	{
		QBrush brushmin(QColor(47,79,79));
		painter->setBrush(brushmin);
		x = MinFPoint.x;
		y = MinFPoint.y;
		painter->drawRect(x,y,m_steplen,m_steplen);

		if(m_FGH)
		{
			QBrush brush(QColor(0,0,0));
			painter->setBrush(brush);
			painter->drawText(x+1,y+m_steplen/3,QString::number(MinFPoint.F/m_steplen));
			painter->drawText(x+1,y+m_steplen,QString::number(MinFPoint.G/m_steplen));
			painter->drawText(x+2*(m_steplen/3),y+m_steplen,QString::number(MinFPoint.H/m_steplen));
			painter->drawText(x+2*(m_steplen/3),y+m_steplen/3,QString::number(MinFPoint.step));
			painter->drawText(x+m_steplen/3,y+m_steplen/3,QString(MinFPoint.parentDirectionicon));
		}
	}
	//灰色 已经查询过的节点
	for(int i=0;i<ClosePoint.size();i++)
	{
	    QBrush brushclose(QColor(190,190,190));
		painter->setBrush(brushclose);
		x= ClosePoint[i].x;
		y= ClosePoint[i].y;
		painter->drawRect(x,y,m_steplen,m_steplen);

			if(m_FGH)
		{
			QBrush brush(QColor(0,0,0));
			painter->setBrush(brush);
			painter->drawText(x+1,y+(m_steplen/3),QString::number(ClosePoint[i].F/m_steplen));
			painter->drawText(x+1,y+m_steplen,QString::number(ClosePoint[i].G/m_steplen));
			painter->drawText(x+3*(m_steplen/5),y+m_steplen,QString::number(ClosePoint[i].H/m_steplen));
		}
	}

	//DarkSeaGreen4
	QBrush brush(QColor(105,139,105));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    for(int i=0;i<m_wallPoint.size();i++)
    {
        x = m_wallPoint.at(i).x();
        y = m_wallPoint.at(i).y();
        painter->drawRect(x,y,m_steplen,m_steplen);
    }


}

void CentFrom::mouseMoveEvent(QMouseEvent * event)
{

	int x = event->pos().x()/m_steplen + 1;
	int y = event->pos().y()/m_steplen + 1;
	QString pStr = QString::number(x) + "X" + QString::number(y);
	QToolTip::showText(mapToGlobal(event->pos()) ,pStr);
}