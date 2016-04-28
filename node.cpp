#include "node.h"
#include <fstream>
#include <set>
extern int present_pos;
extern ofstream fout;
extern set<int> forb_child;
#include <conio.h>
#include <atlstr.h>
Node::Node():haschild(false),n(0),result(0),mustwin(false),musttie(false),childnum(0){

}


double Node::priority(double c,vector <Node> &v){
	double s1=(double)result/(double)n;
	double s2=2*log((double)v[parent].n);
	double s3=sqrt(s2/(double)n);
	return (s1+c*s3);
}
void Node::pass_up(vector <Node> &v){
	int temp_pos=index;
	int step=-1;
	while(temp_pos!=present_pos){
		temp_pos=v[temp_pos].parent;
		v[temp_pos].n+=n;
		v[temp_pos].result+=step*result;
		step=-step;
		//fout<<"temp_pos"<<temp_pos<<" temp_pos.n"<<v[temp_pos].n<<" temp_pos.result"<<v[temp_pos].result<<endl;
	}
}
void Node::pass_up(int _n,int _result,vector <Node> &v){
	int temp_pos=index;
	int step=-1;
	while(temp_pos!=present_pos){
		temp_pos=v[temp_pos].parent;
		v[temp_pos].n+=_n;
		v[temp_pos].result+=step*_result;
		step=-step;
		//fout<<"temp_pos"<<temp_pos<<" temp_pos.n"<<v[temp_pos].n<<" temp_pos.result"<<v[temp_pos].result<<endl;
	}
}
int Node::best_child(double c,vector <Node> &v){
	double m=INT_MIN,pri;
	int bc=child[0];
	for(int i=0;i<childnum;i++){
		if(c==0){
			if(forb_child.count(v[child[i]].stone_posy)==0){	//保证不是禁选的落子，否则对手可以在这一步上继续走一步，然后就赢了
				pri=v[child[i]].priority(c,v);
				if(pri>m){
					m=pri;
					bc=child[i];
				}
			}
		}
		else{
			pri=v[child[i]].priority(c,v);
			if(pri>m){
				m=pri;
				bc=child[i];
			}
		}
	}
	return bc;
}
void Node::print_child(vector <Node> &v){
	for(int i=0;i<childnum;i++){
		fout<<i<<" "<<child[i]<<" result: "<<v[child[i]].result<<" n: "<<v[child[i]].n<<"pri: "<<v[child[i]].priority(1,v)<<endl;
		_cprintf("%d %d result: %d n: %d pri: %f stone_posx:%d stone_posy:%d\n",i,child[i],v[child[i]].result,v[child[i]].n,v[child[i]].priority(1,v),v[child[i]].stone_posx,v[child[i]].stone_posy);
	}
}
int Node::corr_child(int lastY,vector <Node> &v){
	for(int i=0;i<childnum;i++){
		if(v[child[i]].stone_posy==lastY){
			return child[i];
		}
	}
	return -1;
}
