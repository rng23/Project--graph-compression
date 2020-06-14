/************************************************************************************************
Author: joaopfg

For me, path-to-boost-library is /home/dell/Documentos/boost_1_71_0

To compile: g++ -I path-to-boost-library test_iso.cpp -o test_iso

Running it like that:
./test_iso graph.edges graph_iso.edges
************************************************************************************************/

#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>

using namespace std;
using namespace boost;

typedef pair<int,int> ii;
typedef adjacency_list<setS, vecS, bidirectionalS> graph_type;

template <typename Graph1, typename Graph2>

//New callback to give just the existence of isomorphism between two graphs
class my_call_back {

public:
    // constructor
    my_call_back(const Graph1& graph1, const Graph2& graph2) : graph1_(graph1), graph2_(graph2) {}

    template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const {

    return true;
	}

private:
    const Graph1& graph1_;
    const Graph2& graph2_;   
};  

vector<ii> get_edges(string &file_name){
	ifstream file(file_name);
	vector<ii> edges;
	int x, y;

	while(file.is_open()){
		while(file >> x >> y){
			x--;
			y--;
			edges.push_back({x, y});
		}
		file.close();
	}

	return edges;
}

double test(int n, string &graph, string &graph_iso){
	vector<ii> edges1 = get_edges(graph);
	vector<ii> edges2 = get_edges(graph_iso);
	graph_type graph1(n);
	graph_type graph2(n);

	for(int i=0;i<(int)edges1.size();i++){
		add_edge(edges1[i].first, edges1[i].second, graph1);
		add_edge(edges1[i].first, edges1[i].second, graph1);
	}

	for(int i=0;i<(int)edges2.size();i++){
		add_edge(edges2[i].first, edges2[i].second, graph2);
		add_edge(edges2[i].first, edges2[i].second, graph2);
	}

	my_call_back<graph_type, graph_type> callback(graph1, graph2);
	auto start = std::chrono::high_resolution_clock::now();
	if(vf2_subgraph_iso(graph1, graph2, callback)){}
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	return elapsed.count();
}

int main(int argc, char* argv[])
{
	string file_name = "ResultsPA.out";
	ofstream fout(file_name);

	while(fout.is_open()){
		for(int i=7;i<=100;i++){
			string graph = "smallWorld_" + to_string(i) + ".edges";
			string graph_iso = "smallWorld_" + to_string(i) + "_iso.edges";
			fout << test(i, graph, graph_iso) << endl;
		}

		//string graph = "smallWorld_" + to_string(500) + ".edges";
		//string graph_iso = "smallWorld_" + to_string(500) + "_iso.edges";
		//fout << test(500, graph, graph_iso) << endl;
		fout.close();
	}


  	return 0;
}
