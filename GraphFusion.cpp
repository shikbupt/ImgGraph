#include "GraphFusion.h"
#include "dot.h"
#include <iterator>
using std::back_inserter;


//Successor相同点
struct IsEqualSuccessors {
	IsEqualSuccessors(const string &successor_value) : successor_value_(successor_value){};
	bool operator()(const Successors &successor) {
		return (successor.node_value == successor_value_);
	}
private:
	const string successor_value_;
};

//successors节点的排序函数,以节点名字典排序
bool CompareSuccessors(const Successors& first, const Successors& second);

//successors节点的除去相同节点
bool CompareUniqueSuccessors(const Successors& first, const Successors& second);

//node容器的排序函数，以节点名字典排序
struct CompareNodes {
	bool operator()(const Node& first, const Node& second) {
		return first.node_value < second.node_value ? 1 : 0;
	} 
};

void GraphFusion::BuildQueryGraph(const string &query_img, 
	const vector<string> &leaf_imgs, Graph &query_graph) {
	const Graph *prior_graph = 
		query_graph.GetAlgorithm() == prior_graphs1_->GetAlgorithm() ? prior_graphs1_ : prior_graphs2_;
	query_graph.CreateGraph(leaf_imgs);
	vector<string> grandson_nodes;
	//query节点和孙节点没加入
	for (Graph::SuccessorsIter iter = query_graph.Nbegin(); iter != query_graph.Nend(); ++iter) {
		Graph::SuccessorsIter prior_iter = prior_graph->Find(iter->node_value);
		iter->nodes_list->assign(prior_iter->nodes_list->begin(), prior_iter->nodes_list->end());
		for (Graph::NodesIter node_iter = prior_iter->nodes_list->begin(); 
			node_iter != prior_iter->nodes_list->end(); ++node_iter) {
				if (find(leaf_imgs.begin(), leaf_imgs.end(), node_iter->node_value) == leaf_imgs.end() &&
					node_iter->node_value != query_img) {
					grandson_nodes.push_back(node_iter->node_value);
				}
		}
	}	
	sort(grandson_nodes.begin(), grandson_nodes.end());
	vector<string>::iterator unique_iter = unique(grandson_nodes.begin(), 
		grandson_nodes.end());
	grandson_nodes.erase(unique_iter, grandson_nodes.end());
	//处理孙节点
	for (vector<string>::iterator iter = grandson_nodes.begin(); iter != grandson_nodes.end(); ++iter) {
		query_graph.AddNode(*iter);
	}
	for (vector<string>::iterator iter = grandson_nodes.begin(); iter != grandson_nodes.end(); ++iter) {
		Graph::SuccessorsIter prior_iter = prior_graph->Find(*iter);
		Graph::SuccessorsIter query_graph_iter = query_graph.Find(*iter);
		for (Graph::NodesIter nodes_iter = prior_iter->nodes_list->begin(); 
			nodes_iter != prior_iter->nodes_list->end(); ++nodes_iter) {
				if (find_if(query_graph.Nbegin(), query_graph.Nend(), 
					IsEqualSuccessors(nodes_iter->node_value)) != query_graph.Nend()) {
						query_graph_iter->nodes_list->push_back(*nodes_iter);						
				}
		}
	}
	//加入query节点
	query_graph.AddNode(query_img);
	Graph::SuccessorsIter query_graph_iter = query_graph.Find(query_img);
	for (vector<string>::const_iterator iter = leaf_imgs.begin(); iter != leaf_imgs.end(); ++iter) {
		Node node_temp = {0, 1, *iter};
		query_graph_iter->nodes_list->push_back(node_temp);
	}
	//test
	Dot fusion_dot1(&query_graph);
	fusion_dot1.CreatDotFile("D:\\projectresult\\fusion3_dot.txt");
	query_graph.CalEdgeValue(query_img);
}

void GraphFusion::FuseQueryGraphs(const Graph &query_graph1, const Graph &query_graph2) {
	vector<string> successors;
	for (Graph::ConstSuccessorsIter iter = query_graph1.Nbegin(); iter != query_graph1.Nend(); ++iter) {
		successors.push_back(iter->node_value);
	}
	for (Graph::ConstSuccessorsIter iter = query_graph2.Nbegin(); iter != query_graph2.Nend(); ++iter) {
		successors.push_back(iter->node_value);
	}
	sort(successors.begin(), successors.end());
	vector<string>::iterator end_unique = unique(successors.begin(),successors.end());
	successors.erase(end_unique, successors.end());
	fusion_graph_.CreateGraph(successors);
	vector<Node> query_node1;
	vector<Node> query_node2;
	for (Graph::SuccessorsIter iter = fusion_graph_.Nbegin(); iter != fusion_graph_.Nend(); ++iter) {
		Graph::SuccessorsIter query_iter1 = query_graph1.Find(iter->node_value);
		Graph::SuccessorsIter query_iter2 = query_graph2.Find(iter->node_value);
		//容和时，节点只存在一幅图中
		if (query_iter1 == query_graph1.Nend() || query_iter2 == query_graph2.Nend()) {
			Graph::SuccessorsIter query_temp = (query_iter1 == query_graph1.Nend() ? query_iter2 : query_iter1);
			iter->nodes_list->assign(query_temp->nodes_list->begin(), query_temp->nodes_list->end());
			continue;
		}
		//节点存在两幅图中
		query_node1.assign(query_iter1->nodes_list->begin(), query_iter1->nodes_list->end());
		query_node2.assign(query_iter2->nodes_list->begin(), query_iter2->nodes_list->end());
		sort(query_node1.begin(), query_node1.end(), CompareNodes());
		sort(query_node2.begin(), query_node2.end(), CompareNodes());
		vector<Node> mix_query_node;
		merge(query_node1.begin(), query_node1.end(), query_node2.begin(), query_node2.end(), 
			back_inserter(mix_query_node), CompareNodes());
		for (vector<Node>::iterator node_iter = mix_query_node.begin(); 
			node_iter != mix_query_node.end(); ++node_iter) {
				if ((node_iter+1) != mix_query_node.end() && 
					node_iter->node_value == (node_iter+1)->node_value) {
					Node temp = {node_iter->stat, node_iter->edge_value + (node_iter+1)->edge_value,
						node_iter->node_value};
					++node_iter;
					iter->nodes_list->push_back(temp);
				}
				else{
					iter->nodes_list->push_back(*node_iter);
				}
		}
	}
}

void GraphFusion::Fuse(const string &query_img, 
	const vector<string> &leaf_imgs1, const vector<string> &leaf_imgs2) {
	Graph query_graph1(prior_graphs1_->GetAlgorithm());
	Graph query_graph2(prior_graphs2_->GetAlgorithm());
	BuildQueryGraph(query_img, leaf_imgs1, query_graph1);
	Dot fusion_dot1(&query_graph1);
	fusion_dot1.CreatDotFile("D:\\projectresult\\fusion1_dot.txt");
	BuildQueryGraph(query_img, leaf_imgs2, query_graph2);
	Dot fusion_dot2(&query_graph2);
	fusion_dot2.CreatDotFile("D:\\projectresult\\fusion2_dot.txt");
	FuseQueryGraphs(query_graph1, query_graph2);
}

void GraphFusion::Rerank(const string &query_img) {
	rank_algorithm_->Rank(query_img, fusion_graph_, response_imgs_);
}