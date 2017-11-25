#ifndef ASTAR_H
#define ASTAR_H

#include <map>
#include <vector>
using namespace std;

typedef enum direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
}DIRECTION;

typedef enum Control{
	CONTROL_FGH,
	CONTROL_RATE,
	CONTROL_PROCESSESS
}CONTROL;

//数据结构影响搜索速度 ,有待优化
//1、后找到的点先找到
//2、F值小的要先找到
typedef struct state{
    int x;
    int y;
    bool moveabel;
	DIRECTION parentDirection;
	char parentDirectionicon;
    state* parent;
    int F;
    int G;
    int H;
    int step;
    bool operator==(const state& other)
    {
        return x==other.x && y==other.y;
    }
	void directionico()
	{
		switch (parentDirection)
		{
		case UP:
			parentDirectionicon = 'A';
			break;

		case DOWN:
			parentDirectionicon = 'V';
			break;
		case LEFT:
			parentDirectionicon = '<';
			break;
		case RIGHT:
			parentDirectionicon = '>';
			break;
		default:
			parentDirectionicon = ' ';	
			break;
		}
	
	
	}
}STATE_POINT;

class cmp_key
{
public:
    bool operator()(const STATE_POINT* k1, const STATE_POINT* k2)const
    {

        if(k1->F < k2->F)
            return true;
        if(k1->F == k2->F && k1->step > k2->step)
            return true;
        return false;
        
    }
};
#define MY_SORT_MAP (map<STATE_POINT*,int,cmp_key>) 
#define SORT_MAP




typedef enum type{
	NEW_SEARCH_POINT,
	MIN_F_POINT,
	CLOSE_POINT
}POINT_TYPE;

typedef void (* SearchCallback)(void* arg,STATE_POINT* point,POINT_TYPE type);



class Astar
{
public:
    Astar(void);
    ~Astar(void);

	void setCallback(SearchCallback callback,void* arg){ m_callback = callback;callarg = arg;}
    void setStep(int step){this->m_step = step;}
    void setSideLen(int len){this->side_len = len;}
    void setOutWall(int len);//no use
    void AddtoObstacleList(STATE_POINT* point);
	void reset();
	void setPause(bool pause){this->m_pause = pause;}
	void setControl(CONTROL type,bool enable);
    STATE_POINT* createPoint(int x,int y,STATE_POINT* parent,bool move);

    STATE_POINT* FindWay(STATE_POINT* startPoint,STATE_POINT* destPoint);
private:
    MY_SORT_MAP* openList;
	map<STATE_POINT*,int> closeList;
	map<STATE_POINT*,int> obstacleList;
    int m_step;
    int stepAll;
    int side_len;
	bool m_pause;
	bool m_fastrate;
	bool m_processess;
    vector<DIRECTION> m_direction;
    STATE_POINT* destPoint;
	SearchCallback m_callback;
	void* callarg;

	//从src到dst的方向
	DIRECTION direction(STATE_POINT* dst,STATE_POINT* src);
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
}POINT_INT;

class TestAstar
{
public:
    TestAstar();

    ~TestAstar();
    void run();
private:
    void show();
    Astar* star;
    POINT_INT start,direction;
};

#endif // ASTAR_H
