#include "dot.h"
#include "graph.h"

void Dot::Edges2Dot() {
	for (Graph::ConstSuccessorsIter successors_iter = graph_->Nbegin(); 
		successors_iter != graph_->Nend(); ++successors_iter) {
			for (Graph::ConstNodesIter nodes_iter = successors_iter->nodes_list->begin();
				nodes_iter != successors_iter->nodes_list->end(); ++nodes_iter
				) {
					if (!nodes_iter->stat.is_undot) {
						string_buffer_ << successors_iter->node_value << "--" <<nodes_iter->node_value
							<< "[" << "label=\"" << nodes_iter->edge_value<< "\"];" <<'\n';
						graph_->SetNodeStat(successors_iter->node_value, nodes_iter->node_value, false);
					}
					
			}
	}
}

void Dot::Nodes2Dot() {
	for (Graph::ConstSuccessorsIter successors_iter = graph_->Nbegin(); 
		successors_iter != graph_->Nend(); ++successors_iter) {
			string_buffer_ << successors_iter->node_value << ";" << '\n';
	}
}

void Dot::CreatDotFile(const string& file_path) {
	Nodes2Dot();
	Edges2Dot();
	fstream out_file(file_path, std::ios::out);
	out_file << "graph main {\n" << string_buffer_.str() << '}';
}