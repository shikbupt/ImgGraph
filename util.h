#ifndef UTIL_H__
#define UTIL_H__

#include "graph.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class MatrixTransition {
public:
	virtual void SelectCandidate(vector<float> &dist_line) = 0;
	void CheckSymmetryCandidate();
	void push_back(vector<float> &line){dist_matrix_.push_back(line);}
	typedef vector<vector<float> >::iterator Dist_Matrix_Iter;
	Dist_Matrix_Iter GetMatrixBegin() {return dist_matrix_.begin();}
	Dist_Matrix_Iter GetMatrixEnd() {return dist_matrix_.end();}
private:
	vector<vector<float> >dist_matrix_;
};

//��������ת��
class RankTransition : public MatrixTransition {
public:
	RankTransition(int top_k) : top_k_(top_k) {}
	virtual void SelectCandidate(vector<float> &dist_line);
private:
	int top_k_;
};

//������ֵת��
class ThresholdTransition : public MatrixTransition {
public: 
	ThresholdTransition(float threshold) : threshold_(threshold) {}
	virtual void SelectCandidate(vector<float> &dist_line);
private:
	float threshold_;
};

//���������װ��Ϊ��׼ͼ�ṹ����
void ConvertDistMatrix(const string &dist_matrix_file, const string &convert_matrix_file, 
		const vector<string> &images_name, MatrixTransition &transition);

//�ں�ͼƬ�ڵ���ŷ���
class RerankAlgorithm {
public:
	virtual void Rank(const string &query_img, const Graph &fusion_graph, vector<string> &ranked) = 0;
};

//���Ȩ���ܶȷ�
class MaxWeightDensity : public RerankAlgorithm {
public:
	MaxWeightDensity(int k) : top_k_(k) {}
	virtual void Rank(const string &query_img, const Graph &fusion_graph, vector<string> &ranked);
private:
	int top_k_;
};
#endif // UTIL_H__
