#ifndef ASTAR_H
#define ASTAR_H

#include <map>
#include <vector>
using namespace std;


//数据结构影响搜索速度 ,有待优化
//1、后找到的点先找到
//2、F值小的要先找到
typedef struct state{
    int x;
    int y;
    bool moveabel;
    state* parent;
    int F;
    int G;
    int H;
    int step;
    bool operator==(const state& other)
    {
        return x==other.x && y==other.y;
    }
}STATE_POINT;
struct cmp_key
{
    bool operator()(const POINT* k1, const POINT* k2)const
    {
        if(k1->F < k2->F)
            return true;
        if(k1->F == k2->F && k1->step > k2->step)
            return true;
        return false;
        
    }
};
#define (map<STATE_POINT*,int,cmp_key>) MY_SORT_MAP
#define SORT_MAP

typedef enum direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
}DIRECTION;

class Astar
{
public:
    Astar(void);
    ~Astar(void);
    void setStep(int step){this->m_step = step;}
    void setSideLen(int len){this->side_len = len;}
    void setOutWall(int len);//no use
    void AddtoObstacleList(STATE_POINT* point);

    STATE_POINT* createPoint(int x,int y,STATE_POINT* parent,bool move);

    STATE_POINT* FindWay(STATE_POINT* startPoint,STATE_POINT* destPoint);
private:
    MY_SORT_MAP openList,closeList,obstacleList;
    int m_step;
    int stepAll;
    int side_len;
    vector<DIRECTION> m_direction;
    STATE_POINT* destPoint;
    STATE_POINT* atOpenList(STATE_POINT* point);
    STATE_POINT* atCloseList(STATE_POINT* point);
    STATE_POINT* atObstacleList(STATE_POINT* point);
    STATE_POINT* getMinFOpenlist();
    void runStepOne(vector<STATE_POINT*>& enablePoint,STATE_POINT* current,int step);
    int runStepOne(STATE_POINT* current,STATE_POINT* nextpoint,DIRECTION direction);
    int hanmanH(STATE_POINT& nowPoint);

};

typedef struct point
{
    int x;
    int y;
}POINT;

class TestAstar
{
public:
    TestAstar();

    ~TestAstar();
    void run();
private:
    void show();
    Astar* star;
    POINT start,direction;
};

#endif // ASTAR_H
