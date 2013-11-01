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

	//��ͼ����ӽڵ㣬�ޱߣ��ڵ�Ψһ
	void CreateGraph(const vector<string>& nodes_name);
	void AddNode(const string &node_name);
	void AddNode(const vector<string> &node_name_vect);

	//����֮����ӱ�, ����1�ɹ�������0ʧ�ܣ�������node������
	bool AddEdge(const string& node1, const string& node2, float edge_value = -1);

	//�ı�Ȩ��
	bool SetEdgeValue(const string &node1, const string &node2, float edge_value);

	//�õ�ͼ�Ľڵ�ͱ���
	int GetNodesNumber() {return graph_.nobes_number;}
	int GetEdgesNumber() {return graph_.edges_number;}

	//ȡ������
	string GetAlgorithm() const {return algorithm_name_;}

	//һ���ڵ�Ķ���
	int GetOutDegree(const string &node);
	
	//�ж��Ƿ������
	bool HasNode(const string &node1) const;

	//�ж������Ƿ��б�
	bool HasEdge(const string &node1, const string &node2);
	//void DoOnEdges(string node1, void (*f)(int node1, node *node2, void *data), void *data);
	
	//���ؽڵ������
	SuccessorsIter Find(const string &node) const ;

	//������
	SuccessorsIter Nbegin() {return graph_.successors_list->begin();}
	SuccessorsIter Nend() {return graph_.successors_list->end();}

	ConstSuccessorsIter Nbegin() const {return graph_.successors_list->begin();}
	ConstSuccessorsIter Nend() const {return graph_.successors_list->end();}

	//��ͼ������Ϊ����ṹ,����1�ɹ�������0ʧ��
	bool SaveGraphbyMatrix(const string &file_name) const;

	//������ȡ����ṹ��ͼ������1�ɹ�������0ʧ��
	bool LoadGraphbyMatrix(const string &file_name);

	//��ͼ������Ϊ�򵥽ṹ,����1�ɹ�������0ʧ��
	bool SaveGraphbySimple(const string &file_name) const;

	//������ȡ�򵥽ṹ��ʾ��ͼ������1�ɹ�������0ʧ��
	bool LoadGraphbySimple(const string &file_name);

	//������س��Ƚڵ�
	float SearchOutdegreeNode(ConstNodesIter &begin, ConstNodesIter &end, const string &target) const;

	//�����ֵ
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
