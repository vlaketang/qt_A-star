#ifndef ASTAR_H
#define ASTAR_H

#include <map>
#include <vector>
using namespace std;


//���ݽṹӰ�������ٶ� ,�д��Ż�
//1�����ҵ��ĵ����ҵ�
//2��FֵС��Ҫ���ҵ�
typedef struct state{
    int x;
    int y;
    bool moveabel;
    state* parent;
    int F;
    int G;
    int H;
    int step;
	char buffn2[1000]; //����
	char buffn3[6000]; //����
	char buffn4[4000]; //����
	char buffn5[5000]; //����
	char buffn6[5000]; //����
	char buffn7[5000]; //����
	char buffc4[5000]; //����
	char buffc5[5000]; //����
	char buffc6[5000]; //����
	char buffc7[5000]; //����
	char buffb4[5000]; //����
	char buffb5[5000]; //����
	char buffb6[5000]; //����
	char buffab7[5000]; //����
	char buffa4[5000]; //����
	char buffa5[5000]; //����
	char buffa6[5000]; //����
	char buffa7[5000]; //����
    bool operator==(const state& other)
    {
        return x==other.x && y==other.y;
    }
}STATE_POINT;

struct cmp_key
{
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
	void reset();
    STATE_POINT* createPoint(int x,int y,STATE_POINT* parent,bool move);

    STATE_POINT* FindWay(STATE_POINT* startPoint,STATE_POINT* destPoint);
private:
    MY_SORT_MAP* openList;
	map<STATE_POINT*,int> closeList;
	map<STATE_POINT*,int> obstacleList;
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
