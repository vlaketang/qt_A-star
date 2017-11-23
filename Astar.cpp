#include "astar.h"
#include <stdlib.h>

Astar::Astar(void)
{

    openList = map<STATE_POINT*,int>();
    closeList = map<STATE_POINT*,int>();
    obstacleList = map<STATE_POINT*,int>();
    m_direction.push_back(UP);
    m_direction.push_back(DOWN);
    m_direction.push_back(LEFT);
    m_direction.push_back(RIGHT);
}

//len:内部空间的大小
/************************
如下例子的len=4
******
*xxxx*
*xxxx*
*xxxx*
*xxxx*
******
************************/
void Astar::setOutWall(int len)
{
    for(int i= 0; i <=len+1;i++)
    {
        STATE_POINT* point = createPoint(i,0,NULL,false);
        AddtoObstacleList(point);
        point = createPoint(i,len+1,NULL,false);
        AddtoObstacleList(point);
        if(i != 0 ||i !=len+1)
        {
            point = createPoint(0,i,NULL,false);
            AddtoObstacleList(point);
            point = createPoint(len+1,i,NULL,false);
            AddtoObstacleList(point);
        }
    }

}

STATE_POINT* Astar::createPoint(int x,int y,STATE_POINT* parent, bool move)
{
    STATE_POINT* start = new STATE_POINT();
    memset(start,0,sizeof(STATE_POINT));
    start->x = x;
    start->y = y;
    start->parent = parent;
    return start;
}

STATE_POINT* Astar::FindWay(STATE_POINT* startPoint,STATE_POINT* destPoint)
{
    this->destPoint = destPoint;
    openList.insert(make_pair(startPoint,0));
    int runtime = 0;
    while(1)
    {
        runtime++;
        STATE_POINT* dpoint = atOpenList(destPoint);

        if(dpoint)
        {
            printf("find runtime:%d\n",runtime);
            return dpoint;
        }

        STATE_POINT* pointMinF = getMinFOpenlist();

        if(!pointMinF)
        {
            printf("not find :%d\n",runtime);
            return NULL;
        }
		printf("run time %d\n",runtime);
        vector<STATE_POINT*> enablePoint = vector<STATE_POINT*>();
        runStepOne(enablePoint,pointMinF,runtime);
        vector<STATE_POINT*>::iterator it = enablePoint.begin();
        for(;it != enablePoint.end();++it)
        {
            if(!atCloseList(*it) && !atObstacleList(*it))
            {
                STATE_POINT* point = atOpenList(*it);
                if(point && point->G > (*it)->G + m_step)
                {
                    point->parent = pointMinF;

                    point->G = (*it)->G + m_step;
                    point->F = point->G + point->H;

                }
                openList.insert(make_pair(*it,0));

            }else
            {
                closeList.insert(make_pair(*it,0));
            }
        }
        closeList.insert(make_pair(pointMinF,0));
    }
}

Astar::~Astar(void)
{

	map<STATE_POINT*,int>::iterator it = openList.begin();
	while(it!=openList.end())
	{
		if(it->first)
			delete it->first;
		it++;
	}
	openList.clear();

	it = closeList.begin();
	while(it!=closeList.end())
	{
		if(it->first)
			delete it->first;
		it++;
	}
	closeList.clear();

	it = obstacleList.begin();
	while(it!=obstacleList.end())
	{
		if(it->first)
			delete it->first;
		it++;
	}
	obstacleList.clear();

}

void Astar::AddtoObstacleList(STATE_POINT* point)
{
    obstacleList.insert(make_pair(point,0));
}

STATE_POINT* Astar::atOpenList(STATE_POINT* point)
{
    int runtime = 0;
    map<STATE_POINT*,int>::iterator it = openList.begin();
    for(;it != openList.end();++it)
    {
        runtime++;
        if(*point == *(it->first))
        {
			printf("openlist have:%d atOpenList echo search runtime:%d\n",openList.size(),runtime);
            return it->first;
        }
    }
	printf("openlist have:%d atOpenList echo search runtime:%d\n",openList.size(),runtime);
    return NULL;
}
STATE_POINT* Astar::atCloseList(STATE_POINT* point)
{
    int runtime = 0;
    map<STATE_POINT*,int>::iterator it = closeList.begin();
    for(;it != closeList.end();++it)
    {
        runtime++;
        if(*point == *(it->first))
        {
            return it->first;
        }
    }
    return NULL;
}
STATE_POINT* Astar::atObstacleList(STATE_POINT* point)
{
    int runtime = 0;
    map<STATE_POINT*,int>::iterator it = obstacleList.begin();
    for(;it != obstacleList.end();++it)
    {
        runtime++;
        if(*point == *(it->first))
        {

            return it->first;
        }
    }

    return NULL;

}
STATE_POINT* Astar::getMinFOpenlist()
{

#ifndef SORT_MAP
    if(openList.size() == 0)
        return NULL;
    int runtime = 0;
    map<STATE_POINT*,int>::iterator it = openList.begin();

    STATE_POINT* minF = it->first;
    map<STATE_POINT*,int>::iterator min = it;
    for(;it != openList.end();)
    {
		printf("%d ",it->first->F);
        runtime++;
        if(minF->F >= it->first->F)
        {
            minF = it->first;
            min = it;
        }
        it++;
    }
    openList.erase(min);
	printf("\n runtime:%d minF:%d\n",runtime,minF->F);
    return minF;
#else
    STATE_POINT* min = openList.begin()->first;
    openList.erase(openList.begin());
    return min;

#endif
}
void Astar::runStepOne(vector<STATE_POINT*>& enablePoint,STATE_POINT* current,int step)
{
    for(int i = 0 ;i < m_direction.size();++i)
    {
        STATE_POINT* point = new STATE_POINT;
        point->step = step;
        int ret =  runStepOne(current,point,m_direction.at(i));
        if(ret>0)
        {
            enablePoint.push_back(point);
        }else
            delete point;
    }

}
int Astar::runStepOne(STATE_POINT* current,STATE_POINT* nextpoint,DIRECTION direction)
{
    nextpoint->x = current->x;
    nextpoint->y = current->y;
    switch (direction)
    {
    case UP:
        nextpoint->y -= this->m_step;
        break;

    case DOWN:
        nextpoint->y += this->m_step;
        break;
    case LEFT:
        nextpoint->x -= this->m_step;
        break;
    case RIGHT:
        nextpoint->x += this->m_step;
        break;
    default:
        break;
    }
    if(nextpoint->x < 0 ||
		nextpoint->x > side_len-this->m_step ||
       nextpoint->y < 0 ||
       nextpoint->y > side_len-this->m_step ||
       atObstacleList(nextpoint))
    {
        return -1;
    }
    nextpoint->parent = current;
    nextpoint->G = current->G + this->m_step;
    nextpoint->H = hanmanH(*nextpoint);
    nextpoint->F = nextpoint->G + nextpoint->H;
    return 1;
}
int Astar::hanmanH(STATE_POINT& nowPoint)
{
    int x = abs(nowPoint.x - destPoint->x);
    int y = abs(nowPoint.y - destPoint->y);
	printf("H=%d+%d\n",x,y);
    return x+y;
}


#define SIDE_LEN 14
char outwall[SIDE_LEN][SIDE_LEN] ={
     '*','*','*','*','*','*','*','*','*','*','*','*','*','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','x','x','x','x','x','x','x','x','x','x','x','x','*'
    ,'*','*','*','*','*','*','*','*','*','*','*','*','*','*'
};


POINT gwall[] = {{1,2},{2,2},{3,4},{3,3},{3,9},{4,7},{9,9},{4,10},{5,10},{6,10},{7,10},{8,10}};


TestAstar::TestAstar()
{

    show();
    star = new Astar();
    star->setStep(1);
    star->setSideLen(SIDE_LEN);
    start.x = 1;
    start.y = 1;


    direction.x = 4;
    direction.y = 12;


    outwall[start.x][start.y] = 'S';
    outwall[direction.x][direction.y] = 'D';

    for(int i = 0;i<sizeof(gwall)/sizeof(POINT);i++)
    {
        STATE_POINT* point = star->createPoint(gwall[i].x,gwall[i].y, NULL,false);
        star->AddtoObstacleList(point);
        outwall[gwall[i].x][gwall[i].y] = 'W';
    }

    show();


}
TestAstar::~TestAstar()
{
}



void TestAstar::show()
{
    printf("  ");
    for(int i = 0;i<SIDE_LEN;i++)
    {
        printf("%d ",i%10);
    }
    printf("\n");
    for(int i = 0;i<SIDE_LEN;i++)
    {

        printf("%d ", i%10);
        for(int j = 0;j<SIDE_LEN ; j++)
        {
            printf("%c ",outwall[i][j]);
        }
        printf("\n");
    }
    printf("------------------\n");
}
void TestAstar::run()
{
    STATE_POINT* startpoint = star->createPoint(start.x,start.y, NULL,true);
    STATE_POINT* end = star->createPoint(direction.x,direction.y,NULL,true);
    STATE_POINT* path = star->FindWay(startpoint,end);


    if(path)
    {
        path = path->parent;
        while(path)
        {
            if(path->parent)
                outwall[path->x][path->y] = 'A' + path->step;
            path = path->parent;
        }
        show();
    }else
    {
        printf("not find way\n ");
    }

    return;
}
