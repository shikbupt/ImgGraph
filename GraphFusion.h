#ifndef GRAPHFUSION_H__
#define GRAPHFUSION_H__

#include "graph.h"
#include "util.h"

class GraphFusion {
public:

	GraphFusion(const Graph *prior_graphs1, const Graph *prior_graphs2, RerankAlgorithm *rank_algo) : 
	  prior_graphs1_(prior_graphs1), prior_graphs2_(prior_graphs2), 
	  fusion_graph_("fusion"), rank_algorithm_(rank_algo){};

	  //�ں������㷨�ó���ͼ
	void Fuse(const string &query_img, 
		const vector<string> &leaf_imgs1, const vector<string> &leaf_imgs2);

	//���ݵó���ͼ�����query���Ƶ�ͼƬ
	void Rerank(const string &query_img);
	
	void GetQueryResponse(vector<string> &result_imgs) {
		result_imgs.assign(response_imgs_.begin(), response_imgs_.end());}

	Graph& GetFusionImage() {return fusion_graph_;}

private:
	
	//��queryimageΪ���ĸ��ݲ�ͬ���㷨����ͼ�ṹ
	void BuildQueryGraph(const string &query_img, 
		const vector<string> &leaf_imgs, Graph &query_graph);

	//��BuildQueryGraphs����������ͼ�ں�
	void FuseQueryGraphs(const Graph &query_graph1, const Graph &query_graph2);

	const Graph *prior_graphs1_, *prior_graphs2_;
	RerankAlgorithm *rank_algorithm_;
	Graph fusion_graph_;
	vector<string> response_imgs_;
};
#endif // GRAPHFUSION_H__
