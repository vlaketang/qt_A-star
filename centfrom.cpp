#include "centfrom.h"
#include <QTimer>
//#include <qRegisterMetaType>
#include <QToolTip>
CentFrom::CentFrom(QWidget *parent)
	: QWidget(parent),
	working(false),
	thread(NULL),
	timer(new QTimer(this)),
	m_todo(NO_USE)
{
	m_steplen = 40;
	m_side = 800;
	ui.setupUi(this);
    this->setStyleSheet("background-color:red;");
 
    m_startPoint = QPoint(3*m_steplen,9*m_steplen);
    m_endPoint = QPoint(14*m_steplen,10*m_steplen);

    m_wallPoint = QList<QPoint>();
    m_wayPoint = QVector<QPoint>();

	OpenPoint = QVector<STATE_POINT>();
	ClosePoint = QVector<STATE_POINT>();
    connect(this, SIGNAL(sgn_brush()), this, SLOT(update()));
	qRegisterMetaType<STATE_POINT>("STATE_POINT");
	qRegisterMetaType<STATE_POINT>("POINT_TYPE");
	

	setMouseTracking(true);
}


CentFrom::~CentFrom()
{

}
void CentFrom::slot_setstepSize(const QString &text)
{
	QString box = text.split('x').first();

	m_steplen = m_side/box.toInt();
    m_startPoint = QPoint(2*m_steplen,2*m_steplen);
    m_endPoint = QPoint(4*m_steplen,4*m_steplen);
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
		connect(thread, SIGNAL(sgn_result(STATE_POINT*)), this, SLOT(slot_result(STATE_POINT*)));
		connect(thread, SIGNAL(sgn_point(STATE_POINT,POINT_TYPE)), this, SLOT(slot_point(STATE_POINT,POINT_TYPE)));
	}
	this->emit sgn_init(&m_wallPoint,m_startPoint,m_endPoint,m_steplen,m_side);

    thread->start();


}

void CentFrom::slot_point(STATE_POINT point ,POINT_TYPE type)
{
	printf("point back,x:%d,y:%d, %d\n",point.x,point.y,type);
	switch (type)
	{
	case MIN_F_POINT:
		MinFPoint = point;
		break;
	case NEW_SEARCH_POINT:
		OpenPoint.append(point);
		if(ClosePoint.contains(point))

		break;

	case CLOSE_POINT:
		if(OpenPoint.contains(point))
		{
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
            }
            path = path->parent;
        }
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
			printf("stop thread\n");
			thread->exit();
		}
		working = false;
	}
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
	printf("121212");

    int x,y;
    QPainter* painter = new QPainter(this);
   // painter->drawRect(0,0,700,700);

    for(int i = 0; i<= m_side/m_steplen;i++)
    {
        painter->drawLine(QPoint(i*m_steplen,0),QPoint(i*m_steplen,m_side));
        painter->drawLine(QPoint(0,i*m_steplen),QPoint(m_side,i*m_steplen));
    }

    QBrush brushstart(QColor(255,0,0));
    painter->setBrush(brushstart);
    x = m_startPoint.x();
    y = m_startPoint.y();
    painter->drawRect(x,y,m_steplen,m_steplen);

    QBrush brushend(QColor(0,0,255));
    painter->setBrush(brushend);

    x = m_endPoint.x();
    y = m_endPoint.y();
    painter->drawRect(x,y,m_steplen,m_steplen);


    QBrush brushway(QColor(105,139,105));
    painter->setBrush(brushway);

    for(int i = 0;i<m_wayPoint.size();i++)
    {
        x=m_wayPoint[i].x();
        y = m_wayPoint[i].y();
        painter->drawRect(x,y,m_steplen,m_steplen);

		QBrush brushway(QColor(0,255,0));
		painter->setBrush(brushway);
		painter->drawText(x,y+(m_steplen/2),QString::number(i));
    }





	
    QBrush brushopen(QColor(152,251,152));
    painter->setBrush(brushopen);
	painter->drawRect(10*m_steplen,0,m_steplen,m_steplen);
	for(int i=0;i<OpenPoint.size();i++)
	{
		x= OpenPoint[i].x;
		y= OpenPoint[i].y;
		painter->drawRect(x,y,m_steplen,m_steplen);

		


		QBrush brush(QColor(0,0,0));
		painter->setBrush(brush);
		painter->drawText(x,y+(m_steplen/3),QString::number(OpenPoint[i].F/m_steplen));
		painter->drawText(x,y+m_steplen,QString::number(OpenPoint[i].G/m_steplen));
		painter->drawText(x+3*(m_steplen/5),y+m_steplen,QString::number(OpenPoint[i].H/m_steplen));
	}

	QBrush brushmin(QColor(47,79,79));
    painter->setBrush(brushmin);
	x = MinFPoint.x;
    y = MinFPoint.y;
	painter->drawRect(10*m_steplen,m_steplen,m_steplen,m_steplen);
    painter->drawRect(x,y,m_steplen,m_steplen);

	//»ÒÉ«
    QBrush brushclose(QColor(190,190,190));
    painter->setBrush(brushclose);
	painter->drawRect(10*m_steplen,2*m_steplen,m_steplen,m_steplen);
	for(int i=0;i<ClosePoint.size();i++)
	{
		x= ClosePoint[i].x;
		y= ClosePoint[i].y;
		painter->drawRect(x,y,m_steplen,m_steplen);

		QBrush brush(QColor(0,0,0));
		painter->setBrush(brush);
		painter->drawText(x,y+(m_steplen/3),QString::number(ClosePoint[i].F/m_steplen));
		painter->drawText(x,y+m_steplen,QString::number(ClosePoint[i].G/m_steplen));
		painter->drawText(x+3*(m_steplen/5),y+m_steplen,QString::number(ClosePoint[i].H/m_steplen));
	}

	//DarkSeaGreen4
	QBrush brush(QColor(105,139,105));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
	painter->drawRect(10*m_steplen,3*m_steplen,m_steplen,m_steplen);
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