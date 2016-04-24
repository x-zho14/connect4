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
extern vector <Node> v;
extern ofstream fout;
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
			if(board[i][j])
				fout<<(char)('A'-1+board[i][j])<<" ";
			else
				fout<<board[i][j]<<" ";
		}
		fout<<endl;
	}

}
int stimulate(int **board,int *temp_top,int **temp_board,int *temp_temp_top,int M,int N,int noX,int noY,int step){
	refresh_board(M,N,temp_board,board);
	refresh_top(N,temp_temp_top,temp_top);
	
	int owner;
	if(step==1)
		owner=1;
	else
		owner=-1;
	while(true){
		int pos=random(0,N);
		if(pos==noY&&(temp_temp_top[pos]-1)==noX){
			temp_temp_top[pos]--;
		}
		if(temp_temp_top[pos]==0){
			continue;
		}
		temp_temp_top[pos]--;
		temp_board[temp_temp_top[pos]][pos]=step;
		
		if(step==2&&machineWin(temp_temp_top[pos],pos,M,N,temp_board)){
			//if(owner==-1){
				//fout<<"owner lose"<<owner<<endl;
			//此时说明是在对手的节点进行模拟如果machine赢了，应该说明对手输了，此时返回负值;
			//}
			return owner;
		}
		if(step==1&&userWin(temp_temp_top[pos],pos,M,N,temp_board)){
			//fout<<"temp_temp_top[pos]"<<temp_temp_top[pos]<<"pos"<<pos<<"lose"<<endl;
			return (-owner);
		}
		if(isTie(N,temp_temp_top)){
			//fout<<"tie"<<endl;
			return 0;
		}
		step=3-step;
	}
}


/*
	添加你自己的辅助函数
*/


#endif