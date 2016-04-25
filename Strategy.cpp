#include <iostream>
#include <vector>
#include <set>
#include <ctime>
#include <ctime>
#include <fstream>
#include "windows.h"
#include "Point.h"
#include "Strategy.h"
#include "node.h"
#include <conio.h>
#include <atlstr.h>
#include <assert.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
vector <Node> v;
set <int> forb_child;
int present_pos=0;
ofstream fout("a.txt");
const unsigned long uppertime=1000;
/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	
		
	AllocConsole();
	int x = -1, y = -1;
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	board[noX][noY]=-1;
	int **temp_board=new int*[M];
	for(int i = 0; i < M; i++){
		temp_board[i] = new int[N];
		for(int j = 0; j < N; j++){
			temp_board[i][j] = _board[i * N + j];
		}
	}
	_cprintf("\n\n\n-----------GO------------\n");
	int *temp_top=new int[N],*temp_temp_top=new int[N];
	refresh_top(N,temp_top,top);
	//判断是否送冲	
	for(int i=0;i<N;i++){
		if((top[i])>=2&&i!=noY){		
			if(_userWin(top[i]-2,i,M,N,board)){
				forb_child.insert(i);
				_cprintf("forb_child %d\n",i);
			}
		}
	}
	if((top[noY]-2)==noX&&top[noY]>=3){
		if(_userWin(top[noY]-3,noY,M,N,board)){
			forb_child.insert(noY);
			_cprintf("forb_child %d\n",noY);
		}
	}
	else{
		if(top[noY]>=2){
			if(_userWin(top[noY]-2,noY,M,N,board)){
				forb_child.insert(noY);
				_cprintf("forb_child %d\n",noY);
			}
		}
	}
	//
	for(int i=0;i<N;i++){
		if(top[i]>=1&&_machineWin(top[i]-1,i,M,N,board)){
			return new Point(top[i]-1, i);
		}
	}
	if(samelinedoublewin(top,board,M,N,x,y)){
		return new Point(x,y);
	}
	
	//判断是否有对手必胜的子
	for(int i=0;i<N;i++){
		if(top[i]>=1&&_userWin(top[i]-1,i,M,N,board)){
			return new Point(top[i]-1, i);
		}
	}
	v.clear();
	Node t;
	//print(board,M,N);
	v.push_back(t);
	v.back().index=0;
	int temp_pos=0;
	DWORD starttime = timeGetTime(); //计时
	DWORD currttime;
	bool counternopoint=false;
	for(int k=0;((currttime = timeGetTime()) - starttime) < uppertime;k++){
		int step=2;
		for(int times=0;v[temp_pos].haschild==true;times++){
			temp_pos=v[temp_pos].best_child(1);
			board[v[temp_pos].stone_posx][v[temp_pos].stone_posy]=step;
			temp_top[v[temp_pos].stone_posy]--;
			if((v[temp_pos].stone_posx>=1)&&(board[v[temp_pos].stone_posx-1][v[temp_pos].stone_posy]==-1)){
				temp_top[v[temp_pos].stone_posy]--;
				assert(temp_top[v[temp_pos].stone_posy]==v[temp_pos].stone_posx-1);
			}
			assert(temp_top[v[temp_pos].stone_posy]==v[temp_pos].stone_posx);
			step=3-step;
		}
		assert(v[temp_pos].haschild==false);
		if(v[temp_pos].mustwin==true){
				v[temp_pos].n+=1;
				v[temp_pos].result+=1;
				v[temp_pos].pass_up(1,1);
			}
		else if(v[temp_pos].musttie==true){
				v[temp_pos].n+=1;
				v[temp_pos].pass_up(1,0);
		}
		else if(v[temp_pos].haschild==false){
			assert(v[temp_pos].mustwin==false&&v[temp_pos].musttie==false);
			v[temp_pos].haschild=true;
			for(int i=0;i<N;i++){
				if(temp_top[i]>0){
					v.push_back(t);
					v[temp_pos].child.push_back(v.size()-1);
					v.back().index=v.size()-1;
					v.back().parent=temp_pos;
					v.back().stone_posx=temp_top[i]-1;
					v.back().stone_posy=i;
					if(step==2&&_machineWin(temp_top[i]-1,i,M,N,board)){
						v.back().n=100;
						v.back().result=100;
						v.back().mustwin=true;
					}
					else if(step==1&&_userWin(temp_top[i]-1,i,M,N,board)){
						v.back().n=100;
						v.back().result=100;
						v.back().mustwin=true;
					}
					else if(_isTie(i,N,temp_top)){
						v.back().n=100;
						v.back().result=0;
						v.back().musttie=true;
					}
					else{
						for(int j=0;j<100;j++){
							v.back().result=v.back().result+stimulate(board,temp_top,temp_board,temp_temp_top,M,N,noX,noY,step,temp_top[i]-1,i);
							v.back().n++;
						}
					}
					v.back().pass_up();
				}
			}
		}
		temp_pos=present_pos;//回到初始状态
		refresh_board(M,N,board,_board);
		board[noX][noY]=-1;
		refresh_top(N,temp_top,top);
	}
	int bc=v[present_pos].best_child(0);
	//v[present_pos].print_child();
	x=v[bc].stone_posx;
	y=v[bc].stone_posy;
	board[x][y]=2;
	//print(board,M,N);
	print_fbc();
	forb_child.clear();
	//_cprintf("the stone i choose:x=%d y=%d\n",x,y);
	clearArray(M, N, board);
	//_cprintf("the vector size():%d\n",v.size());
	//_cprintf("-----------END------------\n\n\n");
	return new Point(x, y);
}


/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/
