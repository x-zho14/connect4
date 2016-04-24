#include <iostream>
#include <vector>
#include <ctime>
#include <ctime>
#include <fstream>
#include "windows.h"
#include "Point.h"
#include "Strategy.h"
#include "node.h"
#include <conio.h>
#include <atlstr.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
vector <Node> v;
int present_pos;
ofstream fout("a.txt");
#define pInt(str,x) _cprintf("%s=%d\n"str,x)
const unsigned long uppertime=4500;
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
	int **temp_board=new int*[M];
	for(int i = 0; i < M; i++){
		temp_board[i] = new int[N];
		for(int j = 0; j < N; j++){
			temp_board[i][j] = _board[i * N + j];
		}
	}
	//fout<<"M="<<M<<endl<<"N="<<N<<endl<<"lastX="<<lastX<<endl<<"lastY="<<lastY<<endl<<"noX="<<noX<<endl<<"noY"<<noY<<endl;
	int *temp_top=new int[N],*temp_temp_top=new int[N];
	refresh_top(N,temp_top,top);
	refresh_top(N,temp_temp_top,top);
	int stone_num=0;
	for(int i = 0; i < M; i++){
		for(int j = 0; j < N; j++){
			if(board[i][j])
				stone_num++;
		}
	}
	Node t;
	if((stone_num/2)==0){//�����Ƿ���һ���ֵ�����
		v.push_back(t);
		v.back().index=0;
		present_pos=0;
	}/*Ȼ��������ǰ�ľ��棬��������Ѿ�����һ�����ˣ�Ȼ����ָո�����һ������ô�Ҿ���Ҫ���ն����µķ�ʽ��������һ����
	 ��Ӧ�ľ��ǰ�present_posת��������ĳ�����ӣ�������ӵ��ص�������stone_posy�ͺʹ�����lastyһ�¡�
	 */
	else{
		fout<<"original_pos"<<present_pos<<endl;
		print(board,M,N);
		present_pos=v[present_pos].corr_child(lastY);
		fout<<"go one way to "<<present_pos<<endl;
		fout<<"stone_posx"<<v[present_pos].stone_posx<<" stone_posy"<<v[present_pos].stone_posy<<endl;

	}/*
	 ��ת�������λ��֮�����ǾͿ�ʼģ���Լ��������ˡ�



	 */
	//�ж��Ƿ��б�ʤ�ĺ���
	for(int i=0;i<v[present_pos].child.size();i++){
		if(v[v[present_pos].child[i]].mustwin==true)
			return new Point(v[v[present_pos].child[i]].stone_posx, v[v[present_pos].child[i]].stone_posy);
	}
	for(int i=0;i<N;i++){
		if(top[i]){
			board[top[i]-1][i]=1;
			if(userWin(top[i]-1,i,M,N,board)){
				return new Point(top[i]-1, i);
			}
			board[top[i]-1][i]=0;
		}
	}	
	
	
	int temp_pos=present_pos;
	DWORD starttime = timeGetTime(); //��ʱ
	DWORD currttime;
	
	for(int k=0;((currttime = timeGetTime()) - starttime) < uppertime;k++){
		int step=2;
		//fout<<"time used:"<<((currttime = timeGetTime()) - starttime)<<endl;
		for(int times=0;v[temp_pos].haschild==true;times++){//����к���ѡ������ڵ�����õĺ���,Ȼ�������һ�������
		//����������������������ڵ�ʱ�к��ӵģ���Ӧ�ô�ͷ��ʼ���������������һ������Ӧ����ô��
			//fout<<"search for bc:temp_pos"<<temp_pos<<endl;
			//v[temp_pos].print_child();
			if(((currttime = timeGetTime()) - starttime)>uppertime)
				_cprintf("overtime1");
			temp_pos=v[temp_pos].best_child(1);
			if(((currttime = timeGetTime()) - starttime)>uppertime)
				_cprintf("overtime2");
			//fout<<"go deep"<<times<<" time to"<<temp_pos<<endl;
			board[v[temp_pos].stone_posx][v[temp_pos].stone_posy]=step;
			//print(board,M,N);

			temp_top[v[temp_pos].stone_posy]--;
			step=3-step;//ת��������ɫ
			//fout<<"the stone type"<<step<<endl;
		}
		if(v[temp_pos].haschild==false){//�ҵ���һ��Ҷ�ӽڵ㣬Ȼ��ʼ��չ���ĺ��ӣ���չ�˺���֮��Ͱ���Ϣ�ش���������Ȼ�����¿�ʼ�ڶ���ѡȡ��õĺ��ӣ�
			v[temp_pos].haschild=true;
			for(int i=0;i<N;i++){
				if(((currttime = timeGetTime()) - starttime)>uppertime)
				_cprintf("overtime");
				if(temp_top[i]>0){
					if(((currttime = timeGetTime()) - starttime)>uppertime)
						_cprintf("overtime");
					v.push_back(t);
					v[temp_pos].child.push_back(v.size()-1);
					if(i==noY&&(temp_top[i]-1)==noX){//�������Ҫ���ӵ�λ���ǲ��ɷŵ�
						temp_top[i]--;
					}//temp_top ��boardҪ���Ʋ���Ϊ��ǰtemp_pos�Ľڵ��ͼ
					//temp_temp_top��temp_board�����Ժ��ӽ���ģ��
					if(temp_top[i]==0){//temp_top��ʱ�Ѿ������˲��ܷ��ӵ�
						continue;
					}
					temp_top[i]--;
					board[temp_top[i]][i]=step;//Ҫ�������ҷ����ӻ�����������
					v.back().index=v.size()-1;
					v.back().parent=temp_pos;
					v.back().stone_posx=temp_top[i];
					v.back().stone_posy=i;
					if(step==2&&machineWin(temp_top[i],i,M,N,board)){
						//fout<<"child"<<i<<"must machinewin";
						v.back().n=100;
						v.back().result=100;
						v.back().mustwin=true;
						//print(board,M,N);
					}
					else if(step==1&&userWin(temp_top[i],i,M,N,board)){
						//fout<<"child"<<i<<"must userwin";
						v.back().n=100;
						v.back().result=100;
						//print(board,M,N);
					}
					else if(isTie(N,temp_temp_top)){
						//fout<<"must tie";
						v.back().n=100;
						v.back().result=0;
						//print(board,M,N);
					}
					else{
						for(int j=0;j<100;j++){
							v.back().result=v.back().result+stimulate(board,temp_top,temp_board,temp_temp_top,M,N,noX,noY,3-step);
							//fout<<"child"<<i<<"with "<<j<<"stimulations result is "<<v.back().result<<endl;
							v.back().n++;
						}
					}
					//fout<<"temp_pos"<<temp_pos<<endl;
					//v[temp_pos].print_child();
					v.back().pass_up();
					board[temp_top[i]][i]=0;
					temp_top[i]++;//�ָ������׵�״̬
				}
			}
			temp_pos=present_pos;//�ص���ʼ״̬
			refresh_board(M,N,board,_board);
			refresh_top(N,temp_top,top);
		}
	}
	fout<<"search for bc:temp_pos"<<present_pos<<endl;
	int bc=v[present_pos].best_child(0);
	v[present_pos].print_child();
	x=v[bc].stone_posx;
	y=v[bc].stone_posy;//��Ȼѡ�����ⲽ�壬�ҵ�prensent_pos��λ��ҲҪ��֮�仯
	present_pos=v[present_pos].corr_child(y);
	fout<<"go the stone i choose"<<endl;
	board[x][y]=2;
	print(board,M,N);

	fout<<"the stone i choose:x="<<x<<"y="<<y<<endl;
	clearArray(M, N, board);
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
