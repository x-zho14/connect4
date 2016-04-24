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
	if((stone_num/2)==0){//区分是否是一开局的情形
		v.push_back(t);
		v.back().index=0;
		present_pos=0;
	}/*然后来到当前的局面，如果现在已经下了一两步了，然后对手刚刚下了一步，那么我就需要按照对手下的方式，我来走一步，
	 对应的就是把present_pos转移至它的某个孩子，这个孩子的特点是他的stone_posy就和传来的lasty一致。
	 */
	else{
		fout<<"original_pos"<<present_pos<<endl;
		print(board,M,N);
		present_pos=v[present_pos].corr_child(lastY);
		fout<<"go one way to "<<present_pos<<endl;
		fout<<"stone_posx"<<v[present_pos].stone_posx<<" stone_posy"<<v[present_pos].stone_posy<<endl;

	}/*
	 在转到了这个位置之后，我们就开始模拟自己的下棋了。



	 */
	//判断是否有必胜的孩子
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
	DWORD starttime = timeGetTime(); //计时
	DWORD currttime;
	
	for(int k=0;((currttime = timeGetTime()) - starttime) < uppertime;k++){
		int step=2;
		//fout<<"time used:"<<((currttime = timeGetTime()) - starttime)<<endl;
		for(int times=0;v[temp_pos].haschild==true;times++){//如果有孩子选择这个节点中最好的孩子,然后进入下一层迭代；
		//我现在遇到的问题是这个节点时有孩子的，我应该从头开始看，如果对手下了一步，我应该怎么走
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
			step=3-step;//转换棋子颜色
			//fout<<"the stone type"<<step<<endl;
		}
		if(v[temp_pos].haschild==false){//找到了一个叶子节点，然后开始扩展他的孩子，扩展了孩子之后就把信息回传到顶部；然后重新开始在顶部选取最好的孩子；
			v[temp_pos].haschild=true;
			for(int i=0;i<N;i++){
				if(((currttime = timeGetTime()) - starttime)>uppertime)
				_cprintf("overtime");
				if(temp_top[i]>0){
					if(((currttime = timeGetTime()) - starttime)>uppertime)
						_cprintf("overtime");
					v.push_back(t);
					v[temp_pos].child.push_back(v.size()-1);
					if(i==noY&&(temp_top[i]-1)==noX){//如果即将要放子的位置是不可放点
						temp_top[i]--;
					}//temp_top 和board要控制不变为当前temp_pos的节点的图
					//temp_temp_top和temp_board用来对孩子进行模拟
					if(temp_top[i]==0){//temp_top此时已经到达了不能放子点
						continue;
					}
					temp_top[i]--;
					board[temp_top[i]][i]=step;//要区分是我方的子还是它方的子
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
					temp_top[i]++;//恢复到父亲的状态
				}
			}
			temp_pos=present_pos;//回到初始状态
			refresh_board(M,N,board,_board);
			refresh_top(N,temp_top,top);
		}
	}
	fout<<"search for bc:temp_pos"<<present_pos<<endl;
	int bc=v[present_pos].best_child(0);
	v[present_pos].print_child();
	x=v[bc].stone_posx;
	y=v[bc].stone_posy;//既然选择了这步棋，我的prensent_pos的位置也要随之变化
	present_pos=v[present_pos].corr_child(y);
	fout<<"go the stone i choose"<<endl;
	board[x][y]=2;
	print(board,M,N);

	fout<<"the stone i choose:x="<<x<<"y="<<y<<endl;
	clearArray(M, N, board);
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
