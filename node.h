#ifndef NODE_H_
#define	NODE_H_

#include <vector>
using namespace std;
class Node{
public:
	Node();
	double priority(double c);
	void pass_up();
	void pass_up(int _n,int _result);
	int corr_child(const int lastY);
	int best_child(double c);
	void print_child();
	void initialize();
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