#include "centfrom.h"
#include <QTimer>
//#include <qRegisterMetaType>
#define STEP_SIZE 20
#define SIDE_LEN 700
CentFrom::CentFrom(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    this->setStyleSheet("background-color:red;");
 
    m_startPoint = QPoint(2*STEP_SIZE,2*STEP_SIZE);

    m_endPoint = QPoint(10*STEP_SIZE,10*STEP_SIZE);

    m_wallPoint = QList<QPoint>();
    m_wayPoint = QVector<QPoint>();
    connect(this, SIGNAL(sgn_brush()), this, SLOT(update()));
	timer = new QTimer(this);

	qRegisterMetaType<STATE_POINT>("STATE_POINT");


}


CentFrom::~CentFrom()
{

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
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_send_time()));
	timer->start(1000);

	thread = new WorkThread(this);
	connect(this,SIGNAL(sgn_init(QList<QPoint>* ,QPoint ,QPoint )),thread,SLOT(slot_init(QList<QPoint>* ,QPoint ,QPoint)));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(thread, SIGNAL(sgn_result(STATE_POINT*)), this, SLOT(slot_result(STATE_POINT*)));
	this->emit sgn_init(&m_wallPoint,m_startPoint,m_endPoint);
    thread->start();




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
}
void CentFrom::slot_send_time()
{
	usetime+=1;
	QString showtime("");
	int hours = usetime/(60*60*60);

	int minute =(usetime%(60*60*60))/(60*60);
	int second = ((usetime%(60*60*60))%(60*60))%60;
	showtime = QString::number(hours) + ":" + QString::number(minute) +":"+QString::number(second);

	this->emit sgn_time(showtime);
}
void CentFrom::slot_reset()
{

    m_wayPoint.clear();
	this->emit sgn_brush();
	this->emit sgn_time(QString("00:00:00"));
	
}

void CentFrom::mousePressEvent(QMouseEvent *event)
{
    QPoint press;

    press.setX((event->pos().x()/STEP_SIZE)*STEP_SIZE);
    press.setY((event->pos().y()/STEP_SIZE)*STEP_SIZE);


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

   this->emit sgn_brush();
}
void CentFrom::paintEvent(QPaintEvent *event)
{

    int x,y;
    QPainter* painter = new QPainter(this);
   // painter->drawRect(0,0,700,700);

    for(int i = 0; i<= SIDE_LEN/STEP_SIZE;i++)
    {
        painter->drawLine(QPoint(i*STEP_SIZE,0),QPoint(i*STEP_SIZE,SIDE_LEN));
        painter->drawLine(QPoint(0,i*STEP_SIZE),QPoint(SIDE_LEN,i*STEP_SIZE));
    }

    QBrush brush(QColor(0,100,100));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    for(int i=0;i<m_wallPoint.size();i++)
    {
        x = m_wallPoint.at(i).x();
        y = m_wallPoint.at(i).y();
        painter->drawRect(x,y,BODY_SIZE,BODY_SIZE);
    }
    QBrush brushstart(QColor(255,0,0));
    painter->setBrush(brushstart);
    x = m_startPoint.x();
    y = m_startPoint.y();
    painter->drawRect(x,y,BODY_SIZE,BODY_SIZE);

    QBrush brushend(QColor(0,0,255));
    painter->setBrush(brushend);

    x = m_endPoint.x();
    y = m_endPoint.y();
    painter->drawRect(x,y,BODY_SIZE,BODY_SIZE);


    QBrush brushway(QColor(0,255,0));
    painter->setBrush(brushway);

    for(int i = 0;i<m_wayPoint.size();i++)
    {
        x=m_wayPoint[i].x();
        y = m_wayPoint[i].y();
        painter->drawRect(x,y,BODY_SIZE,BODY_SIZE);
    }

    //painter->drawEllipse(x,y,BODY_SIZE,BODY_SIZE);
}