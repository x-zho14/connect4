#ifndef NODE_H_
#define	NODE_H_

#include <vector>
using namespace std;
class Node{
public:
	Node();
	double priority(double c,vector <Node> &v);
	void pass_up(vector <Node> &v);
	void pass_up(int _n,int _result,vector <Node> &v);
	int corr_child(const int lastY,vector <Node> &v);
	int best_child(double c,vector <Node> &v);
	void Node::print_child(vector <Node> &v);
	int child[12];
	int childnum;
	int parent;
	int n;
	int result;
	int index;
	int stone_posx;
	int stone_posy;
	bool haschild;
	bool mustwin;
	bool musttie;
};

#endif