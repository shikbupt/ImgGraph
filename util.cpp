#include "util.h"
#include <algorithm>
#include <fstream>
#include <sstream>
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::sort;


void MatrixTransition::CheckSymmetryCandidate() {
	if (!dist_matrix_.empty()) {
		int matrix_rows = dist_matrix_.size();
		for (int i = 0; i < matrix_rows-1; ++i) {
			for (int j = i+1; j < matrix_rows; ++j) {
				if (dist_matrix_[i][j] == -1 ||
					dist_matrix_[j][i] == -1 ) {
					dist_matrix_[j][i] = -1;
				}
			}
		}
	}
}

void RankTransition::SelectCandidate(vector<float> &dist_line) {
	vector<float> dist_temp(dist_line);
	sort(dist_temp.begin(), dist_temp.end());
	float top_k = dist_temp[top_k_];
	for (vector<float>::iterator iter = dist_line.begin(); iter != dist_line.end(); ++iter) {
		if (*iter <= top_k) {
			*iter = 1;
		} 
		else {
			*iter = -1;
		}
	}
	push_back(dist_line);
}

void ThresholdTransition::SelectCandidate(vector<float> &dist_line) {
	for (vector<float>::iterator iter = dist_line.begin(); iter != dist_line.end(); ++iter) {
		if (*iter < threshold_) {
			*iter = 1;
		}
		else{
			*iter = -1;
		}
	}
	push_back(dist_line);
}

//fortest
void print(MatrixTransition &transition) {
	MatrixTransition::Dist_Matrix_Iter iter = transition.GetMatrixBegin();
	ofstream test_file("d:\\projectresult\\test-interproc2.txt");
	for (; iter != transition.GetMatrixEnd(); ++iter) {
		for (vector<float>::iterator iter2 = iter->begin(); iter2 != iter->end(); ++iter2) {
			test_file << *iter2 << " ";
		}
		test_file << '\n';
	}
}


void ConvertDistMatrix(const string &dist_matrix_file, const string &convert_matrix_file, 
	const vector<string> &images_name, MatrixTransition &transition) {
	ifstream in_file(dist_matrix_file);
	string line_in_file;
	istringstream word_in_line;
	vector<float> dist_line;
	while (getline(in_file, line_in_file, '\n')) {
		word_in_line.str(line_in_file);
		float word;
		while (word_in_line >> word) {
			dist_line.push_back(word);
		}
		word_in_line.clear();
		transition.SelectCandidate(dist_line);
		dist_line.clear();
	}
	
	transition.CheckSymmetryCandidate();
	//test
	//print(transition);
	ofstream out_file(convert_matrix_file);
	for (vector<string>::const_iterator iter = images_name.begin(); iter != images_name.end(); ++iter) {
		out_file << *iter << " ";
	}
	out_file << '\n';
	MatrixTransition::Dist_Matrix_Iter iter = transition.GetMatrixBegin()+1;
	for (int i = 1; iter != transition.GetMatrixEnd(); ++iter, ++i) {
		int j = 0;
		for (vector<float>::iterator iter2 = iter->begin(); j < i; ++iter2, ++j) {
			out_file << *iter2 << " ";
		}
		out_file << '\n';
	}
}

bool CompareCandidate(const Node &first, const Node &second) {
	return first.edge_value < second.edge_value;
}

struct IsInRanked {
	IsInRanked(string node_name) : node_name_(node_name) {}
	bool operator()(const Node &node) {
		return node_name_ == node.node_value;
	}
private:
	string node_name_;
};

void MaxWeightDensity::Rank(const string &query_img, const Graph &fusion_graph, vector<string> &ranked) {	
	ranked.push_back(query_img);
	Graph::ConstSuccessorsIter iter = fusion_graph.Find(query_img);
	list<Node> candidate(iter->nodes_list->begin(), iter->nodes_list->end());
	while (!candidate.empty() && (ranked.size()-1) < top_k_) {
		candidate.sort(CompareCandidate);
		ranked.push_back(candidate.back().node_value);
		candidate.pop_back();
		iter = fusion_graph.Find(ranked.back());
		candidate.insert(candidate.end(), iter->nodes_list->begin(), iter->nodes_list->end());
		for (vector<string>::iterator str_iter = ranked.begin(); str_iter != ranked.end(); ++str_iter) {
			candidate.remove_if(IsInRanked(*str_iter));
		}
	}
	ranked.erase(ranked.begin());
}
