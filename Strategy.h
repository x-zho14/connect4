/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Point.h"
#include "Judge.h"
#include "node.h"
#include <vector>
#include <conio.h>
#include <atlstr.h>
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

void clearArray(int M, int N, int** board);
extern int present_pos;
extern ofstream fout;
extern set<int> forb_child;
void refresh_board(const int M, const int N, int **board,const int* _board){
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
}
void refresh_board(const int M, const int N, int ** board,int** _board){
	for(int i = 0; i < M; i++){
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i][j];
		}
	}
}

void refresh_top(const int N, int *temp_top,const int *top){
	for(int j = 0; j < N; j++){
		temp_top[j]=top[j];
	}
}

int random(int u,int v){
	return (rand() % (v-u)) + u;
}
void print(int **board,int M,int N){
	for(int i = 0; i < M; i++){
		for(int j = 0; j < N; j++){
			if(board[i][j]){
				fout<<(char)('A'-1+board[i][j])<<" ";
				_cprintf("%c ",'A'-1+board[i][j]);

			}
			else{
				fout<<board[i][j]<<" ";
				_cprintf("%d ",board[i][j]);
			}
		}
		fout<<endl;
		_cprintf("\n");
	}

}
int stimulate(int **board,int *temp_top,int **temp_board,int *temp_temp_top,int M,int N,int noX,int noY,int step,int x,int y){
	refresh_board(M,N,temp_board,board);
	refresh_top(N,temp_temp_top,temp_top);
	temp_board[x][y]=step;
	temp_temp_top[y]--;
	if((x>=1)&&(temp_board[x-1][y]==-1)){
		temp_temp_top[y]--;
	}
	step=3-step;
	int owner;
	if(step==1)
		owner=1;
	else
		owner=-1;
	while(true){
		int pos=random(0,N);
		if(temp_temp_top[pos]==0){
			continue;
		}
		temp_temp_top[pos]--;
		temp_board[temp_temp_top[pos]][pos]=step;
		if((temp_temp_top[pos]>=1)&&(temp_board[temp_temp_top[pos]-1][pos]==-1)){
			temp_temp_top[pos]--;
		}
		if(step==2&&machineWin(temp_temp_top[pos],pos,M,N,temp_board)){
			return owner;
		}
		if(step==1&&userWin(temp_temp_top[pos],pos,M,N,temp_board)){
			return (-owner);
		}
		if(isTie(N,temp_temp_top)){
			return 0;
		}
		step=3-step;
	}
}

void print_fbc(){
	for(auto rit=forb_child.begin();rit!=forb_child.end();rit++)
		_cprintf("%d ",*rit);
	_cprintf("\n");

}

bool _machineWin(const int x, const int y, const int M, const int N, int** board){
	board[x][y]=2;
	bool win=machineWin(x,y,M,N,board);
	board[x][y]=0;
	return win;
}

bool _userWin(const int x, const int y, const int M, const int N, int** board){
	board[x][y]=1;
	bool win=userWin(x,y,M,N,board);
	board[x][y]=0;
	return win;
}

bool _isTie(const int y,const int N, int* top){
	top[y]--;
	bool win=isTie(N,top);
	top[y]++;
	return win;
}


bool samelinedoublewin(const int *top,int **board,int M,int N,int &x,int &y){
	for(int i=0;i<N;i++){
		if(top[i]>=3){
			board[top[i]-1][i]=2;
			if(_machineWin(top[i]-2,i,M,N,board)&&_machineWin(top[i]-3,i,M,N,board)){
				_cprintf("one line two win condition%d\n",i);
				if(forb_child.count(i)==0){		//如果不是禁止下的点
					_cprintf("count:d\n",forb_child.count(i));
					x=top[i]-1;
					y=i;
					return true;
				}
			}
			board[top[i]-1][i]=0;
		}
	}
	return false;
}

bool difflinedoublewin(const int *top,int **board,int M,int N,int &x,int &y){
	for(int i=0;i<N;i++){
		if(top[i]>=1){
		for(int j=0;j<N;j++){
		if(j!=i&&top[j]>=2){
			board[top[i]-1][i]=2;
			if(_machineWin(top[j]-1,j,M,N,board)&&_machineWin(top[j]-2,j,M,N,board)){
				_cprintf("diff line two win condition%d\n",i);
				if(forb_child.count(i)==0){		//如果不是禁止下的点
					x=top[i]-1;
					y=i;
					return true;
				}
			}
			board[top[i]-1][i]=0;
		}
		}
		}
	}
	return false;
}


bool difflinedoublelose(const int *top,int **board,int M,int N,int &x,int &y){
	for(int i=0;i<N;i++){
		if(top[i]>=1){
		for(int j=0;j<N;j++){
		if(j!=i&&top[j]>=2){
			board[top[i]-1][i]=1;
			if(_userWin(top[j]-1,j,M,N,board)&&_userWin(top[j]-2,j,M,N,board)){
				_cprintf("diff line two lose condition%d\n",i);
				if(forb_child.count(i)==0){		//如果不是禁止下的点
					x=top[i]-1;
					y=i;
					return true;
				}
			}
			board[top[i]-1][i]=0;
		}
		}
		}
	}
	return false;
}


bool twolinewin(){



}

bool twolinelose(){



}
/*
	添加你自己的辅助函数
*/


#endif