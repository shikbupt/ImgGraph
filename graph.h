#ifndef GRAPH_H__
#define GRAPH_H__

#include <vector>
#include <algorithm>
#include <string>
#include <list>
using std::vector;
using std::string;
using std::list;
using std::sort;

union Status {bool is_cal, is_undot;};
struct Node {
	Status stat;
	float edge_value;
	string node_value;
};

struct Successors {
	int nodes_number;          /* number of nodes */
	//int slots_number;        /* number of slots in array */
	//bool is_sorted;		/* true if list is already sorted */
	list<Node> *nodes_list; /* actual list of nodes */
	string node_value;
};

struct GraphProperties{
	int nobes_number;              /* number of vertices */
	int edges_number;              /* number of edges */
	vector<Successors> *successors_list;
};

class Graph{
public:
	friend class Dot;
	typedef vector<Successors>::iterator SuccessorsIter;
	typedef vector<Successors>::const_iterator ConstSuccessorsIter;
	typedef list<Node>::iterator NodesIter;
	typedef list<Node>::const_iterator ConstNodesIter;
	Graph(const string &algorithm_name){
		graph_.nobes_number = 0;
		graph_.edges_number = 0;
		graph_.successors_list = new vector<Successors>;
		algorithm_name_ = algorithm_name;
	}
	Graph(const vector<string>& nodes_name, const string &algorithm_name) : 
		algorithm_name_(algorithm_name) {
		graph_.successors_list = new vector<Successors>;
		CreateGraph(nodes_name);
	}

	//向图中添加节点，无边，节点唯一
	void CreateGraph(const vector<string>& nodes_name);
	void AddNode(const string &node_name);
	void AddNode(const vector<string> &node_name_vect);

	//两点之间添加边, 返回1成功；返回0失败，可能是node不存在
	bool AddEdge(const string& node1, const string& node2, float edge_value = -1);

	//改变权重
	bool SetEdgeValue(const string &node1, const string &node2, float edge_value);

	//得到图的节点和边数
	int GetNodesNumber() {return graph_.nobes_number;}
	int GetEdgesNumber() {return graph_.edges_number;}

	//取方法名
	string GetAlgorithm() const {return algorithm_name_;}

	//一个节点的度数
	int GetOutDegree(const string &node);
	
	//判断是否有这点
	bool HasNode(const string &node1) const;

	//判断两点是否有边
	bool HasEdge(const string &node1, const string &node2);
	//void DoOnEdges(string node1, void (*f)(int node1, node *node2, void *data), void *data);
	
	//返回节点迭代器
	SuccessorsIter Find(const string &node) const ;

	//迭代器
	SuccessorsIter Nbegin() {return graph_.successors_list->begin();}
	SuccessorsIter Nend() {return graph_.successors_list->end();}

	ConstSuccessorsIter Nbegin() const {return graph_.successors_list->begin();}
	ConstSuccessorsIter Nend() const {return graph_.successors_list->end();}

	//将图存外设为矩阵结构,返回1成功，返回0失败
	bool SaveGraphbyMatrix(const string &file_name) const;

	//从外设取矩阵结构的图，返回1成功，返回0失败
	bool LoadGraphbyMatrix(const string &file_name);

	//将图存外设为简单结构,返回1成功，返回0失败
	bool SaveGraphbySimple(const string &file_name) const;

	//从外设取简单结构表示的图，返回1成功，返回0失败
	bool LoadGraphbySimple(const string &file_name);

	//查找相关出度节点
	float SearchOutdegreeNode(ConstNodesIter &begin, ConstNodesIter &end, const string &target) const;

	//计算边值
	void CalEdgeValue(const string &q_img);

	~Graph();
private:
	GraphProperties graph_;
	string algorithm_name_;
	Graph(const Graph&){};
	bool HasNode(const string &node, SuccessorsIter &iter) const;
	void SetNodeStat(const string &node1, const string &node2, bool stat);
};
#endif // GRAPH_H__
