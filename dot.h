#ifndef DOT_H__
#define DOT_H__


#include <sstream>
#include <string>
#include <fstream>
using std::fstream;
using std::string;
using std::stringstream;

class Graph;
class Dot {
public:
	Dot(Graph* g) : graph_(g){SetStat();}	
	void CreatDotFile(const string& file_path);
private:
	void Edges2Dot();
	void Nodes2Dot();
	void SetStat();
	stringstream string_buffer_;
	Graph *graph_;
};
#endif // DOT_H__
