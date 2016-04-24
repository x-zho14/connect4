#include "node.h"
#include <fstream>
extern vector <Node> v;
extern int present_pos;
extern ofstream fout;
Node::Node():haschild(false),n(0),result(0),mustwin(false){

}
double Node::priority(double c){
	double s1=(double)result/(double)n;
	double s2=2*log((double)v[parent].n);
	double s3=sqrt(s2/(double)n);
	return (s1+c*s3);
}
void Node::pass_up(){
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
int Node::best_child(double c){
	double m=INT_MIN,pri;
	int bc;
	for(int i=0;i<child.size();i++){
		pri=v[child[i]].priority(c);
		if(pri>m){
			m=pri;
			bc=child[i];
		}
	}
	return bc;
}
void Node::print_child(){
	for(int i=0;i<child.size();i++){
		fout<<i<<" "<<child[i]<<" result: "<<v[child[i]].result<<" n: "<<v[child[i]].n<<"pri: "<<v[child[i]].priority(1)<<endl;
	}
}
int Node::corr_child(int lastY){
	for(int i=0;i<child.size();i++){
		if(v[child[i]].stone_posy==lastY){
			return child[i];
		}
	}
	return -1;

}