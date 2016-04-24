#ifndef NODE_H_
#define	NODE_H_

#include <vector>
using namespace std;
class Node{
public:
	Node();
	double priority(double c);
	void pass_up();
	int corr_child(const int lastY);
	int best_child(double c);
	void Node::print_child();
	vector <int> child;
	int parent;
	int n;
	int result;
	int index;
	bool haschild;
	bool mustwin;
	int stone_posx;
	int stone_posy;
};

#endif