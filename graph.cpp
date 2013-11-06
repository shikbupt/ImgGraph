#include "graph.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::find_if;
using std::copy;
using std::back_inserter;
using std::pow;


//successors节点的排序函数,以节点名字典排序
bool CompareSuccessors(const Successors& first, const Successors& second) {
	return first.node_value < second.node_value ? 1 : 0;
}

//node容器的排序函数，以节点名字典排序
struct CompareNodes {
	bool operator()(const Node& first, const Node& second) {
		return first.node_value < second.node_value ? 1 : 0;
	} 
};

//successors节点的除去相同节点
bool CompareUniqueSuccessors(const Successors& first, const Successors& second) {
	return (first.node_value == second.node_value);
}

//nodes节点的除去相同节点
bool CompareUniqueNodes(const Node& first, const Node& second) {
	return (first.node_value == second.node_value);
}

//node相同点
struct IsEqualNodes {
	IsEqualNodes(const string &node_value) : node_value_(node_value){};
	bool operator()(const Node &node) {
		return (node.node_value == node_value_);
	}
private:
	const string node_value_;
};

//Successor相同点
struct IsEqualSuccessors {
	IsEqualSuccessors(const string &successor_value) : successor_value_(successor_value){};
	bool operator()(const Successors &successor) {
		return (successor.node_value == successor_value_);
	}
private:
	const string successor_value_;
};

float Graph::SearchOutdegreeNode(ConstNodesIter &begin, ConstNodesIter &end, 
	const string &target) const{
	Graph::ConstNodesIter node_iter = find_if(begin, end, IsEqualNodes(target));
	if (node_iter == end) return -1;
	return node_iter->edge_value;
}

void Graph::CreateGraph(const vector<string>& nodes_name) {
	graph_.nobes_number = nodes_name.size();
	graph_.edges_number = 0;
	graph_.successors_list->reserve(1.2*graph_.nobes_number);
	Successors successor_temp;
	for (vector<string>::const_iterator iter = nodes_name.begin(); 
		iter != nodes_name.end(); ++iter) {
			successor_temp.node_value = *iter;
			successor_temp.nodes_number = 0;
			successor_temp.nodes_list = new list<Node>;
			graph_.successors_list->push_back(successor_temp);
	}
	sort(graph_.successors_list->begin(), graph_.successors_list->end(), CompareSuccessors);
	vector<Successors>::iterator end_unique = unique(graph_.successors_list->begin(), 
													graph_.successors_list->end(), CompareUniqueSuccessors);
	graph_.successors_list->erase(end_unique, graph_.successors_list->end());
}

void Graph::AddNode(const string &node_name) {
	if (Find(node_name) == Nend()) {
		Successors successor_temp;
		successor_temp.node_value = node_name;
		successor_temp.nodes_number = 0;
		successor_temp.nodes_list = new list<Node>;
		graph_.successors_list->push_back(successor_temp);
		++graph_.nobes_number;
		sort(graph_.successors_list->begin(), graph_.successors_list->end(), CompareSuccessors);
	}
}

void Graph::AddNode(const vector<string> &node_name_vect) {
	for (vector<string>::const_iterator iter = node_name_vect.begin(); 
		iter != node_name_vect.end(); ++iter) {
			if (Find(*iter) == Nend()) {
				Successors successor_temp;
				successor_temp.node_value = *iter;
				successor_temp.nodes_number = 0;
				successor_temp.nodes_list = new list<Node>;
				graph_.successors_list->push_back(successor_temp);
				++graph_.nobes_number;
			}
	}
	sort(graph_.successors_list->begin(), graph_.successors_list->end(), CompareSuccessors);
}

bool Graph::AddEdge(const string& node1, const string& node2, float edge_value){
	SuccessorsIter node1_successors_iter, node2_successors_iter;
	if (HasNode(node1, node1_successors_iter) && HasNode(node2, node2_successors_iter) &&
		SearchOutdegreeNode(node1_successors_iter->nodes_list->begin(), 
			node1_successors_iter->nodes_list->end(), node2) == -1) {
		Node node_temp = {0, edge_value, node2};
		node1_successors_iter->nodes_list->push_back(node_temp);
		++node1_successors_iter->nodes_number;
		node1_successors_iter->nodes_list->sort(CompareNodes());

		node_temp.node_value = node1;
		node2_successors_iter->nodes_list->push_back(node_temp);
		++node2_successors_iter->nodes_number;
		node2_successors_iter->nodes_list->sort(CompareNodes());
		++graph_.edges_number;
		return 1;
	}
	return 0;
}

bool Graph::SetEdgeValue(const string &node1, const string &node2, float edge_value) {
	if (HasEdge(node1, node2)) {
		SuccessorsIter successor1_iter = Find(node1);
		SuccessorsIter successor2_iter = Find(node2);

		Node node_temp = {0, 0, node2};
		NodesIter node1_iter = lower_bound(successor1_iter->nodes_list->begin(),
			successor1_iter->nodes_list->end(), node_temp, CompareNodes());
		node1_iter->edge_value = edge_value;

		node_temp.node_value = node1;
		NodesIter node2_iter = lower_bound(successor2_iter->nodes_list->begin(),
			successor2_iter->nodes_list->end(), node_temp, CompareNodes());
		node2_iter->edge_value = edge_value;

		return 1;
	}
	return 0;
}

int Graph::GetOutDegree( const string &node ) {
	SuccessorsIter iter;
	if (HasNode(node, iter)) return iter->nodes_list->size();
	return -1;
}


bool Graph::HasEdge( const string &node1, const string &node2 ) {
	SuccessorsIter successors_iter;
	if (HasNode(node1, successors_iter) && HasNode(node2)) {
		//Node node_temp = {0, 0, node2};
		if (find_if(successors_iter->nodes_list->begin(),
			successors_iter->nodes_list->end(),IsEqualNodes(node2)) != 
			successors_iter->nodes_list->end()) return 1;
		return 0;
	}
	return 0;
}

bool Graph::HasNode(const string &node) const{
	SuccessorsIter iter = find_if(graph_.successors_list->begin(), graph_.successors_list->end(), 
		IsEqualSuccessors(node));
	if (iter != graph_.successors_list->end()) return 1;
	return 0;
}

bool Graph::HasNode(const string &node, SuccessorsIter &iter) const{
	iter = find_if(graph_.successors_list->begin(), graph_.successors_list->end(), 
		IsEqualSuccessors(node));
	if (iter != graph_.successors_list->end()) return 1;
	return 0;
}

Graph::SuccessorsIter Graph::Find(const string& node) const{
	SuccessorsIter iter;
	HasNode(node, iter);
	return iter;
}

bool Graph::SaveGraphbyMatrix(const string &file_name) const{
	if (graph_.nobes_number == 0) {
		return 0;
	} 
	else {
		ofstream file(file_name);
		for (ConstSuccessorsIter successors_iter = graph_.successors_list->begin();
			successors_iter != graph_.successors_list->end(); ++successors_iter) {
			file << successors_iter->node_value << " ";
		}
		file << '\n';
		for (int i = 1; i < graph_.nobes_number; ++i) {
			ConstNodesIter nodes_iter_begin = graph_.successors_list->at(i).nodes_list->begin();
			ConstNodesIter nodes_iter_end = graph_.successors_list->at(i).nodes_list->end();
			for (int j = 0; j < i; ++j) {
				file << SearchOutdegreeNode(nodes_iter_begin, nodes_iter_end, graph_.successors_list->at(j).node_value)
					<< " ";
			}
			file << '\n';
		}
		return 1;
	}
}

bool Graph::LoadGraphbyMatrix(const string &file_name) {
	ifstream file(file_name);
	string line_in_file;
	int line_number = 0;
	istringstream word_in_line;
	vector<string> successors;
	if (file.good()) {
		while (getline(file, line_in_file, '\n')) {			
			word_in_line.str(line_in_file);
			//第一行，添加各node
			if (line_number == 0) {
				string word;
				while(word_in_line >> word) {
					successors.push_back(word);
				}
				//CreateGraph(successors);
				AddNode(successors);
			}//其他行，处理nodes之间边关系
			else {
				int column_number = 0;
				float edge_value;
				while (word_in_line >> edge_value) {
					if (edge_value != -1) {
						AddEdge(successors.at(line_number), successors.at(column_number), edge_value);
					}
					++column_number;
				}
			}
			word_in_line.clear();
			++line_number;
		}
		return 1;
	}
	return 0;
}

bool Graph::SaveGraphbySimple(const string &file_name) const{
	if (graph_.nobes_number == 0) {
		return 0;
	} 
	else {
		ofstream file(file_name);
		for (ConstSuccessorsIter successors_iter = graph_.successors_list->begin();
			successors_iter != graph_.successors_list->end(); ++successors_iter) {
				file << successors_iter->node_value << " ";
		}
		file << '\n';
		for (ConstSuccessorsIter successors_iter = graph_.successors_list->begin();
			successors_iter != graph_.successors_list->end(); ++successors_iter) {
				file << successors_iter->node_value;
				for (ConstNodesIter nodes_iter = successors_iter->nodes_list->begin();
					nodes_iter != successors_iter->nodes_list->end(); ++nodes_iter) {
					file << " " << nodes_iter->node_value << " " << nodes_iter->edge_value;
				}
				file << '\n';
		}
		return 1;
	}
}

bool Graph::LoadGraphbySimple(const string &file_name) {
	ifstream file(file_name);
	string line_in_file;
	int line_number = 0;
	istringstream word_in_line;
	vector<string> successors;
	if (file.good()) {
		while (getline(file, line_in_file, '\n')) {			
			word_in_line.str(line_in_file);
			//第一行，添加各node
			if (line_number == 0) {
				string word;
				while(word_in_line >> word) {
					successors.push_back(word);
				}
				AddNode(successors);
			}//其他行，处理nodes之间边关系
			else {
				string successor, node;
				float node_value;
				word_in_line >> successor;
				while(word_in_line >> node >> node_value) {
					AddEdge(successor, node, node_value);
				}
			}
			word_in_line.clear();
			++line_number;
		}
		return 1;
	}
	return 0;
}

void Graph::SetNodeStat( const string &node1, const string &node2, bool stat )
{
	SuccessorsIter successor1_iter = Find(node1);
	SuccessorsIter successor2_iter = Find(node2);

	Node node_temp = {0, 0, node2};
	NodesIter node1_iter = lower_bound(successor1_iter->nodes_list->begin(),
		successor1_iter->nodes_list->end(), node_temp, CompareNodes());
	node1_iter->stat.is_cal = stat;

	node_temp.node_value = node1;
	NodesIter node2_iter = lower_bound(successor2_iter->nodes_list->begin(),
		successor2_iter->nodes_list->end(), node_temp, CompareNodes());
	node2_iter->stat.is_cal = stat;
}

void Graph::CalEdgeValue( const string &q_img )
{
	//计算decay coefficient
	vector<Node> one_hop, two_hop;
	SuccessorsIter hop_iter = Find(q_img);
	one_hop.assign(hop_iter->nodes_list->begin(), hop_iter->nodes_list->end());
	for (vector<Node>::iterator iter = one_hop.begin(); iter != one_hop.end(); ++iter) {
		hop_iter = Find(iter->node_value);
		for (NodesIter node_iter = hop_iter->nodes_list->begin(); 
			node_iter != hop_iter->nodes_list->end(); ++node_iter) {
			if (node_iter->node_value != q_img && 
				find_if(one_hop.begin(), one_hop.end(), IsEqualNodes(node_iter->node_value)) == one_hop.end()) {
				two_hop.push_back(*node_iter);
			}
		}
	}
	sort(two_hop.begin(), two_hop.end(), CompareNodes());
	vector<Node>::iterator two_hop_unique_iter = unique(two_hop.begin(), two_hop.end(), CompareUniqueNodes);
	two_hop.erase(two_hop_unique_iter, two_hop.end());
	//计算jaccard coefficient
	vector<Node> q_outdegree, d_outdegree;
	for (SuccessorsIter siter = Nbegin(); siter != Nend(); ++siter) {
		q_outdegree.assign(siter->nodes_list->begin(), siter->nodes_list->end());
		for (NodesIter niter = siter->nodes_list->begin(); 
			niter != siter->nodes_list->end(); ++niter) {
				if (niter->stat.is_cal) continue;
				SuccessorsIter iter_temp = Find(niter->node_value);
				d_outdegree.assign(iter_temp->nodes_list->begin(), iter_temp->nodes_list->end());
				int i = d_outdegree.size();
				int j = q_outdegree.size();
				copy(q_outdegree.begin(), q_outdegree.end(), back_inserter(d_outdegree));
				sort(d_outdegree.begin(), d_outdegree.end(), CompareNodes());
				vector<Node>::iterator iter = unique(d_outdegree.begin(), 
					d_outdegree.end(), CompareUniqueNodes);
				d_outdegree.erase(iter, d_outdegree.end());
				int k = d_outdegree.size();
				float jaccard_coefficient = (float)(i+j-k+2)/(k);
				int path1 = 0;
				int path2 = 0;
				float a0 = 0.8;
				if (find_if(one_hop.begin(), one_hop.end(), IsEqualNodes(siter->node_value)) != one_hop.end()) {
					path1 = 1;
				} 
				else if (find_if(two_hop.begin(), two_hop.end(), IsEqualNodes(siter->node_value)) != two_hop.end()) {
					path1 = 2;
				}
				if (find_if(one_hop.begin(), one_hop.end(), IsEqualNodes(niter->node_value)) != one_hop.end()) {
					path2 = 1;
				} 
				else if (find_if(two_hop.begin(), two_hop.end(), IsEqualNodes(niter->node_value)) != two_hop.end()) {
					path2 = 2;
				}
				
				float edge_value = jaccard_coefficient * pow(a0, path1 > path2 ? path1 : path2);
				SetEdgeValue(siter->node_value, niter->node_value, edge_value);
				SetNodeStat(siter->node_value, niter->node_value, true);
		}
	}
}

Graph::~Graph() {
	for (SuccessorsIter iter = graph_.successors_list->begin(); 
		iter != graph_.successors_list->end(); ++iter) {
		delete iter->nodes_list;
	}
	delete graph_.successors_list;
}