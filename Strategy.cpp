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
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
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
	//�ж��Ƿ��ͳ�	
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
	
	//�ж��Ƿ��ж��ֱ�ʤ����
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
	DWORD starttime = timeGetTime(); //��ʱ
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
		temp_pos=present_pos;//�ص���ʼ״̬
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
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/
