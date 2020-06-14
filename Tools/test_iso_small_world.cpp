/************************************************************************************************
Author: joaopfg

For me, path-to-boost-library is /home/dell/Documentos/boost_1_71_0

To compile: g++ -I path-to-boost-library test_iso_small_world.cpp -o test_iso_small_world

When running, give the paramters n, k and beta as arguments:
./test_iso_small_world n k beta

n: number of nodes
k: mean degree (assumed to be an evem integer)
beta: 0 <= beta <= 1 and n >> k >> ln(n) >> 1

The model constructs an undirected graph with n nodes (nk)/2 edges.

Algorithm:
1) If the nodes are labeled 0..(n-1), there is an edge (i,j) if and only if 0 < |i-j| mod (n-1-(k/2)) <= k/2

2) For every node i=0,...,(n-1) take every edge connecting i to its (k/2) rightmost neighbors, that is every
edge (i,j mod n) with i < j <= i+(k/2), and rewire it with probability beta. Rewiring is done by replacing 
(i,j mod n) with (i,k) where k is chosen uniformly at random from all possible nodes while avoiding self-loops 
(k ~= i) and link duplication (there is no edge (i,k') with k = k' at this point in the algorithm).
************************************************************************************************/

#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include "get_graphviz_format.cpp"

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


//Take uniformly an integer in the interval [lower_bound, upper_bound]
int random_int(int lower_bound, int upper_bound){
   	random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<int> dis(lower_bound, upper_bound);
    return dis(gen);
}

//Construct a regular ring lattice
vector<set<int>> get_initial_graph(int n, int k){
	vector<set<int>> initial_graph(n);

	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(j != i && (abs(i - j))%(n-1-(k/2)) <= k/2)
				initial_graph[i].insert(j);
		}
	}

	return initial_graph;
}

//Simulate a bernoulli variable to see if it will rewire
bool lets_rewire(double beta){
	random_device rd;
    mt19937 gen(rd()); 
    bernoulli_distribution distribution(beta);
    return distribution(gen);
}

void rewire(vector<set<int>> &graph, int i, int n){
	vector<int> candidates;

	for(int j=0;j<n;j++){
		if(j != i && graph[i].count(j) == 0)
			candidates.push_back(j);
	}

	if(candidates.size() != 0){
		int ind = random_int(0, (int)candidates.size() - 1);
		graph[i].insert(candidates[ind]);
		graph[candidates[ind]].insert(i);
	}
}

vector<vector<int>> get_final_graph(vector<set<int>> &initial_graph, int n, int k, double beta){
	for(int i=0;i<n;i++){
		for(int j=i+1;j <=i+(k/2);j++){
			if(initial_graph[i].count(j%n) && lets_rewire(beta)){
				rewire(initial_graph, i, n);
				initial_graph[i].erase(j%n);
				initial_graph[j%n].erase(i);
			}
		}
	}

	vector<vector<int>> final_graph(n);

	for(int i=0;i<n;i++){
		set<int>::iterator it;

		for(it = initial_graph[i].begin();it!=initial_graph[i].end();it++)
			final_graph[i].push_back(*it);
	}

	return final_graph;
}

vector<vector<int>> generate_small_world(int n, int k, double beta){
	vector<set<int>> initial_graph = get_initial_graph(n, k);
	vector<vector<int>> final_graph = get_final_graph(initial_graph, n, k, beta);
	return final_graph;
}

vector<ii> get_konect_format(vector<vector<int>> &adj_list){
	vector<ii> edges;

	for(int i=0;i<(int)adj_list.size();i++){
		for(int j=0;j<(int)adj_list[i].size();j++){
			if(adj_list[i][j] > i) edges.push_back({i, adj_list[i][j]});
		}
	}

	return edges;
}

void generate_small_world_file(int n, int k, double beta){
	vector<vector<int>> small_world = generate_small_world(n, k ,beta);
	vector<ii> edges = get_konect_format(small_world);
	string fin = "small_world/small_world.edges";
	ofstream file_in(fin);

	while(file_in.is_open()){
		for(int i=0;i<(int)edges.size();i++)
			file_in << edges[i].first << " " << edges[i].second << endl;

		file_in.close();
	}

	get_graphviz_format(fin);
}

double test(int n, int k, double beta){
	vector<vector<int>> small_world1 = generate_small_world(n, k ,beta);
	vector<ii> edges1 = get_konect_format(small_world1);
	vector<vector<int>> small_world2 = generate_small_world(n, k ,beta);
	vector<ii> edges2 = get_konect_format(small_world2);

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
	if(argc < 4){
		cout << "Please, provide n, k and beta!" << endl;
		return 0;
	}

	int n = stoi(argv[1]), k = stoi(argv[2]);
	double beta = stod(argv[3]);

	cout << "Time taken to test isomorphism: " << test(n, k, beta) << endl;

	generate_small_world_file(n, k, beta);
  	return 0;
}