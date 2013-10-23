// GraphTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "graph.h"
#include "dot.h"
#include "util.h"
#include "GraphFusion.h"
#include <sstream>

#include <vector>
#include <string>
//#include <vld.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	///*vector<string> node_names;
	//node_names.push_back("A");
	//node_names.push_back("B");
	//node_names.push_back("C");
	//node_names.push_back("D");
	//node_names.push_back("a");
	//node_names.push_back("b");
	//node_names.push_back("c");*/
	///*node_names.push_back("d");
	//node_names.push_back("e");
	//node_names.push_back("E");

	//Graph graph(node_names);
	//graph.AddEdge("a", "A", 1);
	//graph.AddEdge("b", "B", 1);
	//graph.AddEdge("c", "C", 1);
	//graph.AddEdge("d", "D", 1);
	//graph.AddEdge("a", "D", 1);
	//graph.AddEdge("a", "B", 1);
	//graph.AddEdge("a", "e", 1);
	//graph.AddEdge("d", "B", 1);

	//graph.SetEdgeValue("A", "a", 2);
	//graph.SetEdgeValue("B", "a", 3);
	//graph.SetEdgeValue("B", "D", 3);

	//graph.SaveGraph("d:\\projectresult\\graph.txt");
	//Dot dot(&graph);
	//dot.CreatDotFile("d:\\projectresult\\dot.txt");*/
	///*Graph graph("test");
	//graph.LoadGraph("d:\\projectresult\\graph.txt");*/

	///*Dot dot(&graph);
	//dot.CreatDotFile("d:\\projectresult\\dot2.txt");*/

	///*RankTransition rank_transition(4);
	//ThresholdTransition threshold_transition(500);
	//ConvertDistMatrix("d:\\projectresult\\test.txt", "d:\\projectresult\\testresult.txt", node_names, threshold_transition);*/
	///*graph.CalEdgeValue("a");
	//Dot dot(&graph);
	//dot.CreatDotFile("d:\\projectresult\\dot3.txt");*/

	//vector<string> nodes1;
	//nodes1.push_back("A");
	//nodes1.push_back("B");
	//nodes1.push_back("C");
	//nodes1.push_back("D");
	//nodes1.push_back("E");
	//nodes1.push_back("F");

	//Graph graph1("sift");
	//graph1.CreateGraph(nodes1);
	//graph1.AddEdge("C", "D", 1);
	//graph1.AddEdge("C", "E", 1);
	//graph1.AddEdge("C", "F", 1);

	//Dot dot1(&graph1);
	//dot1.CreatDotFile("d:\\projectresult\\graph1.txt");

	//vector<string> nodes2;
	//nodes2.push_back("A");
	//nodes2.push_back("G");
	//nodes2.push_back("D");
	//nodes2.push_back("H");
	//nodes2.push_back("I");

	//Graph graph2("LBP");
	//graph2.CreateGraph(nodes2);
	//graph2.AddEdge("G", "D", 1);
	//graph2.AddEdge("G", "H", 1);
	//graph2.AddEdge("G", "I", 1);

	//Dot dot2(&graph2);
	//dot2.CreatDotFile("d:\\projectresult\\graph2.txt");

	//vector<string> leaf_img1;
	//leaf_img1.push_back("A");
	//leaf_img1.push_back("B");
	//leaf_img1.push_back("D");
	//leaf_img1.push_back("C");

	//vector<string> leaf_img2;
	//leaf_img2.push_back("A");
	//leaf_img2.push_back("D");
	//leaf_img2.push_back("G");

	//MaxWeightDensity max_weight(4);
	//GraphFusion graph_fusion(&graph1, &graph2, &max_weight);
	//graph_fusion.Fuse("q", leaf_img1, leaf_img2);

	//graph_fusion.Rerank("q");

	//vector<string> result;
	//graph_fusion.GetQueryResponse(result);

	//Dot dot3(&graph_fusion.GetFusionImage());
	//dot3.CreatDotFile("d:\\projectresult\\fusion.txt");

	//graph_fusion.GetFusionImage().SaveGraph("d:\\projectresult\\fusion_save.txt");
	
	//处理距离矩阵，得到标准图输入距离
	//ifstream distances_file("D:\\project\\NatureFund\\data\\distance.txt");
	//string dist_matrix_file, convert_matrix_file;
	//vector<string> images_name;
	//string line_distances_file;
	//string temp;	
	//while(getline(distances_file, dist_matrix_file, '\n')) {
	//	convert_matrix_file = dist_matrix_file.substr(dist_matrix_file.find('0'), 3);
	//	ifstream dist_matrix_file_input(dist_matrix_file.c_str());
	//	int row = 0;
	//	while (getline(dist_matrix_file_input, temp)) {
	//		++row;
	//	}		
	//	for (int i = 1; i <=row; ++i) {
	//		ostringstream ss;
	//		//ss << convert_matrix_file << '*' << i;
	//		ss << convert_matrix_file << i;
	//		images_name.push_back(ss.str());
	//	}
	//	convert_matrix_file = "D:\\project\\NatureFund\\data\\";
	//	convert_matrix_file += dist_matrix_file.substr(dist_matrix_file.find('0'));
	//	RankTransition rank_transition(5);
	//	ConvertDistMatrix(dist_matrix_file, convert_matrix_file, images_name, rank_transition);
	//	images_name.clear();
	//}

	Graph graph("sift");
	graph.LoadGraph("D:\\project\\NatureFund\\data\\079.txt");
	graph.LoadGraph("D:\\project\\NatureFund\\data\\002.txt");
	Dot graph_dot(&graph);
	graph_dot.CreatDotFile("D:\\project\\NatureFund\\data\\079dot.dot");
	return 0;
}

