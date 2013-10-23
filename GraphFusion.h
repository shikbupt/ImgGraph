#ifndef GRAPHFUSION_H__
#define GRAPHFUSION_H__

#include "graph.h"
#include "util.h"

class GraphFusion {
public:

	GraphFusion(const Graph *prior_graphs1, const Graph *prior_graphs2, RerankAlgorithm *rank_algo) : 
	  prior_graphs1_(prior_graphs1), prior_graphs2_(prior_graphs2), 
	  fusion_graph_("fusion"), rank_algorithm_(rank_algo){};

	  //融合两种算法得出的图
	void Fuse(const string &query_img, 
		const vector<string> &leaf_imgs1, const vector<string> &leaf_imgs2);

	//根据得出的图计算和query相似的图片
	void Rerank(const string &query_img);
	
	void GetQueryResponse(vector<string> &result_imgs) {
		result_imgs.assign(response_imgs_.begin(), response_imgs_.end());}

	Graph& GetFusionImage() {return fusion_graph_;}

private:
	
	//以queryimage为中心根据不同的算法建立图结构
	void BuildQueryGraph(const string &query_img, 
		const vector<string> &leaf_imgs, Graph &query_graph);

	//把BuildQueryGraphs建立的两个图融合
	void FuseQueryGraphs(const Graph &query_graph1, const Graph &query_graph2);

	const Graph *prior_graphs1_, *prior_graphs2_;
	RerankAlgorithm *rank_algorithm_;
	Graph fusion_graph_;
	vector<string> response_imgs_;
};
#endif // GRAPHFUSION_H__
