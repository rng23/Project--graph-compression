#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/functional/hash.hpp>

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

struct stats{
	int min, max;
	double mean;
}; 

//Get the file in a way that metis can accept
pair<string, int> get_metis_format(string &fin){
	string fout;

	int pos1, pos2;

	for(int i=0;i<(int)fin.size();i++){
		if(fin[i] == '/'){
			pos1 = i;
			break;
		}
	}

	for(int i=0;i<(int)fin.size();i++){
		if(fin[i] == '.'){
			pos2 = i;
			break;
		}
	}

	fout = fin.substr(0,pos1+1);
	fout += fin.substr(pos1+1, pos2-pos1-1);
	fout += "_metis.graph";
	
	vector<ii> edges;
	int x, y, quant_vertices = -1;

	ifstream file_in(fin);
	while(file_in.is_open()){
		while(file_in >> x >> y){
			quant_vertices = max(quant_vertices, x);
			quant_vertices = max(quant_vertices, y);
			edges.push_back({x, y});
		} 
		file_in.close();
	}

	vector<vector<int>> adj_list(quant_vertices + 1);

	for(int i=0;i<(int)edges.size();i++){
		adj_list[edges[i].first].push_back(edges[i].second);
		adj_list[edges[i].second].push_back(edges[i].first);
	}

	ofstream file_out(fout);
	while(file_out.is_open()){
		file_out << quant_vertices << " " << edges.size() << endl;
		int cont = 1;

		for(int i=1;i<=quant_vertices;i++){
			for(int j=0;j<(int)adj_list[i].size();j++){
				file_out << adj_list[i][j] << " ";
			}

			file_out << endl;
		}

		file_out.close();
	}

	return {fout, quant_vertices};
}

//Get the quantity of communities using the metis partition file
int get_communities_number(vector<int> &tag){
	set<int> tag_set;

	for(int i=0;i<(int)tag.size();i++)
		tag_set.insert(tag[i]);

	return tag_set.size();
}

//Get the tags for the vertices using the result file given by the metis program
vector<int> get_tag(string &file_name){
	int p;
	vector<int> tag;
	ifstream fin(file_name);

	while(fin.is_open()){
		while(fin >> p) tag.push_back(p);
		fin.close();
	}

	return tag;
}

//Get the quantity of vertices per community
vector<int> get_quant_vertices_per_community(vector<int> &tag, int quant_communities){
	vector<int> quant_vertices(quant_communities);

	for(int i=0;i<(int)tag.size();i++) quant_vertices[tag[i]]++;

	return quant_vertices;
}

//Get the edges using the Konect file (.edges)
vector<ii> get_edges(string &file_name){
	int x, y;
	vector<ii> edges;
	ifstream fin(file_name);

	while(fin.is_open()){
		while(fin >> x >> y) edges.push_back({x, y});
		fin.close();
	}

	return edges;
}

//Get an adjacency list corresponding to the edges list
//Suppose that the coordinates in the edge list are compressed
vector<vector<int>> get_adj_list(vector<ii> &edges){
	int quant_vertices = -1;

	for(int i=0;i<(int)edges.size();i++){
		quant_vertices = max(quant_vertices, edges[i].first);
		quant_vertices = max(quant_vertices, edges[i].second);
	}

	vector<vector<int>> adj_list(quant_vertices+1);

	for(int i=0;i<(int)edges.size();i++){
		adj_list[edges[i].first].push_back(edges[i].second);
		adj_list[edges[i].second].push_back(edges[i].first);
	}

	return adj_list;
}

//Compress coordinates of a graph in the Konect format (list of edges)
void compress_coordinates(vector<ii> &edges){
	int cont = 0;
	set<int> vertices;
	set<int>::iterator it;
	map<int, int> contraction;

	for(int i=0;i<(int)edges.size();i++){
		vertices.insert(edges[i].first);
		vertices.insert(edges[i].second);
	}

	for(it = vertices.begin();it != vertices.end(); it++){
		contraction[*it] = cont;
		cont++;
	}

	for(int i=0;i<(int)edges.size();i++){
		edges[i].first = contraction[edges[i].first];
		edges[i].second = contraction[edges[i].second];
	}
}

//Get communities vertices
vector<vector<int>> get_communities_vertices(vector<int> &tag, int quant_community){
	vector<vector<int>> communities_vertices(quant_community);

	for(int i=0;i<(int)tag.size();i++)
		communities_vertices[tag[i]].push_back(i);

	return communities_vertices;
}

//Get communities graphs
vector<vector<ii>> get_communities_edges(vector<ii> &edges, vector<int> &tag, int quant_communities){
	vector<vector<ii>> communities_edges(quant_communities);

	for(int i=0;i<(int)edges.size();i++){
		if(tag[edges[i].first] == tag[edges[i].second]){
			communities_edges[tag[edges[i].first]].push_back({edges[i].first, edges[i].second});
		}
	}

	return communities_edges;
}

//Test equality of vertices, edges and degrees of each vertex between two graphs
//To avoid unnecessary calls to the isomorphism function
bool iso_conditions(vector<ii> &edges1, int quant1, vector<ii> &edges2, int quant2){
	if(quant1 != quant2 || edges1.size() != edges2.size()) return false;

	vector<int> deg1(quant1), deg2(quant2);

	for(int i=0;i<(int)edges1.size();i++){
		deg1[edges1[i].first]++;
		deg1[edges1[i].second]++;
	}

	for(int i=0;i<(int)edges2.size();i++){
		deg2[edges2[i].first]++;
		deg2[edges2[i].second]++;
	}

	for(int i=0;i<quant1;i++){
		if(deg1[i] != deg2[i]) return false;
	}

	return true;
}

//Get statistics for the communities
stats get_stats(vector<vector<ii>> &communities_edges){
	stats stats_communities;
	stats_communities.min = -1;
	stats_communities.max = -1;
	stats_communities.mean = 0.0;

	for(int i=0;i<(int)communities_edges.size();i++){
		stats_communities.mean += 1.0*communities_edges[i].size();

		if(stats_communities.min == -1 || communities_edges[i].size() < stats_communities.min)
			stats_communities.min = communities_edges[i].size();

		if(stats_communities.max == -1 || communities_edges[i].size() > stats_communities.max)
			stats_communities.max = communities_edges[i].size();
	}

	stats_communities.mean /= 1.0*communities_edges.size();
	return stats_communities;
}

//Get a file with the graphs of all communities with coordinates compressed
//The format is:
/*
Number_of_vertices number_of_edges
List of edges (one per line) with vertices beginning to be counted from zero
A space to separate different graphs
*/
void get_communities_file(vector<vector<ii>> &communities_edges, vector<int> &quant_vertices_per_community, string &konect_file){
	stats stats_communities = get_stats(communities_edges);

	string communities_file;
	int pos1, pos2;

	for(int i=0;i<(int)konect_file.size();i++){
		if(konect_file[i] == '/'){
			pos1 = i;
			break;
		}
	}

	for(int i=0;i<(int)konect_file.size();i++){
		if(konect_file[i] == '.'){
			pos2 = i;
			break;
		}
	}

	communities_file = konect_file.substr(0,pos1+1);
	communities_file += konect_file.substr(pos1+1, pos2-pos1-1);
	communities_file += "_communities.out";
	ofstream fout(communities_file);

	while(fout.is_open()){
		fout << "Min: " << stats_communities.min << endl;
		fout << "Max: " << stats_communities.max << endl;
		fout << "Mean: " << stats_communities.mean << endl;

		for(int i=0;i<(int)communities_edges.size();i++){
			fout << quant_vertices_per_community[i] << " " << communities_edges[i].size();
			
			for(int j=0;j<(int)communities_edges[i].size();j++){
				fout << " " << communities_edges[i][j].first << " " << communities_edges[i][j].second;
			}

			fout << endl;
		}

		fout.close();
	}
}

//Get the hash table that uses as key the number of vertices and edges and store the community index
std::unordered_map<ii,vector<int>,boost::hash<ii>> get_hash_table(vector<vector<ii>> &communities_edges, vector<int> &quant_vertices_per_community){
	std::unordered_map<ii,vector<int>,boost::hash<ii>> hash_table;

	for(int i=0;i<(int)communities_edges.size();i++){
		int n = quant_vertices_per_community[i];
		int m = communities_edges[i].size();
		hash_table[{n, m}].push_back(i);
	}

	return hash_table;
}

//Auxiliar function to print adjacency matrix
void print_adj_matrix(vector<ii> &edges, ofstream &fout){
	set<int> vertices;
	set<int>::iterator it;

	for(int i=0;i<(int)edges.size();i++){
		vertices.insert(edges[i].first);
		vertices.insert(edges[i].second);
	}

	int quant_vertices = vertices.size();
	vector<vector<int>> adj_mat(quant_vertices+1, vector<int>(quant_vertices+1));

	for(int i=0;i<(int)edges.size();i++){
		adj_mat[edges[i].first][edges[i].second] = 1;
	}

	for(int i=0;i<=quant_vertices;i++){
		for(int j=0;j<i;j++) fout << " ";
		for(int j=i;j<=quant_vertices;j++) fout << adj_mat[i][j];
		fout << endl;
	}
}

//Test to see if a graph is a star
bool test_star(vector<ii> &edges, int quant){
	vector<int> degree(quant);

	for(int i=0;i<(int)edges.size();i++){
		degree[edges[i].first]++;
		degree[edges[i].second]++;
	}

	int leafs = 0, center = 0, isolated = 0;

	for(int i=0;i<quant;i++){
		if(degree[i] == 1) leafs++;
		else if(degree[i] == 0) isolated++;
		else if(degree[i] == quant - 1) center++;
	}

	return (center == 1 && (leafs == quant-1 || (leafs == quant-2 && isolated == 1)));
}

//Test to see if a graph is a path
bool test_path(vector<ii> &edges, int quant){
	vector<int> degree(quant);

	for(int i=0;i<(int)edges.size();i++){
		degree[edges[i].first]++;
		degree[edges[i].second]++;
	}

	int leafs = 0, intermediate = 0;

	for(int i=0;i<quant;i++){
		if(degree[i] == 1) leafs++;
		else if(degree[i] == 2) intermediate++;
	}

	return (leafs == 2 && intermediate == quant-2);
}

//Test to see if a graph is a cycle
bool test_cycle(vector<ii> &edges, int quant){
	vector<int> degree(quant);

	for(int i=0;i<(int)edges.size();i++){
		degree[edges[i].first]++;
		degree[edges[i].second]++;
	}

	int intermediate = 0;

	for(int i=0;i<quant;i++){
		if(degree[i] == 2) intermediate++;
	}

	return (intermediate == quant);
}

//Test to see if a graph is a clique
bool test_clique(vector<ii> &edges, int quant){
	vector<int> degree(quant);

	for(int i=0;i<(int)edges.size();i++){
		degree[edges[i].first]++;
		degree[edges[i].second]++;
	}

	int intermediate = 0;

	for(int i=0;i<quant;i++){
		if(degree[i] == quant-1) intermediate++;
	}

	return (intermediate == quant);
}

//Get the star representation for the catalog
vector<vector<int>> get_iso_graph(ofstream &fout, vector<vector<ii>> &communities_edges, vector<int> &quant_vertices_per_community, int quant_communities, vector<ii> &weak_iso){
	vector<vector<int>> iso_graph(quant_communities);
	vector<bool> marked(quant_communities);
	std::unordered_map<ii,vector<int>,boost::hash<ii>> hash_table = get_hash_table(communities_edges, quant_vertices_per_community);

	for(int i=0;i<(int)communities_edges.size();i++){
		if(i%10 == 0) cout << "Iteration " << i << endl;

		graph_type graph1(quant_vertices_per_community[i]);

		for(int j=0;j<(int)communities_edges[i].size();j++)
			add_edge(communities_edges[i][j].first, communities_edges[i][j].second, graph1);

		int n = quant_vertices_per_community[i];
		int m = communities_edges[i].size();

		//Efficient way to fill the catalog
		for(int j=0;j<(int)hash_table[{n, m}].size();j++){
			if(hash_table[{n, m}][j] == i || marked[hash_table[{n, m}][j]] || 
				!iso_conditions(communities_edges[i], quant_vertices_per_community[i],
				 communities_edges[hash_table[{n, m}][j]], quant_vertices_per_community[hash_table[{n, m}][j]])) 
				continue;

			if(m == 0 || m == 1 || m == (n*(n-1))/2 || (test_star(communities_edges[i], quant_vertices_per_community[i]) 
				&& test_star(communities_edges[hash_table[{n, m}][j]], quant_vertices_per_community[hash_table[{n, m}][j]]))){
				marked[hash_table[{n, m}][j]] = true;
				iso_graph[i].push_back(hash_table[{n, m}][j]);
				iso_graph[hash_table[{n, m}][j]].push_back(i);
				continue;
			}

			graph_type graph2(quant_vertices_per_community[hash_table[{n, m}][j]]);

			for(int z=0;z<(int)communities_edges[hash_table[{n, m}][j]].size();z++)
				add_edge(communities_edges[hash_table[{n, m}][j]][z].first, communities_edges[hash_table[{n, m}][j]][z].second, graph2);
			
			my_call_back<graph_type, graph_type> callback(graph1, graph2);
			auto start = std::chrono::high_resolution_clock::now();

			if(vf2_subgraph_iso(graph1, graph2, callback)){
				auto finish = std::chrono::high_resolution_clock::now();
			    std::chrono::duration<double> elapsed = finish - start;
			   
			 	if(elapsed.count() > 2.0){
			 		fout << "Time consumed: " << elapsed.count() << endl;
			 		fout << "Communities: " << i << " and " << hash_table[{n, m}][j] << endl;
			 		fout << "Number of vertices and edges: " << quant_vertices_per_community[i] << " " << communities_edges[i].size() << endl;
					print_adj_matrix(communities_edges[i], fout);
					fout << endl;
					fout << "Number of vertices and edges: " << quant_vertices_per_community[hash_table[{n, m}][j]] << " " << communities_edges[hash_table[{n, m}][j]].size() << endl;
					print_adj_matrix(communities_edges[hash_table[{n, m}][j]], fout);
					fout << endl << "------------------------------------" << endl;
			 		weak_iso.push_back({i, hash_table[{n, m}][j]});
			 	} 

				marked[hash_table[{n, m}][j]] = true;
				iso_graph[i].push_back(hash_table[{n, m}][j]);
				iso_graph[hash_table[{n, m}][j]].push_back(i);
			}
		}
	}

	return iso_graph;
}

//Get a connected component with depth first search
void get_cc(int v, vector<vector<int>> &iso_graph, vector<bool> &visit, vector<int> &cc){
	visit[v] = true;
	cc.push_back(v);

	for(int i=0;i<(int)iso_graph[v].size();i++){
		if(!visit[iso_graph[v][i]]) get_cc(iso_graph[v][i], iso_graph, visit, cc);
	}
}

//Get the catalog by doing depth first seach in the star representation
 vector<vector<int>> get_catalog(vector<vector<int>> &iso_graph){
 	vector<vector<int>> catalog;
 	vector<bool> visit((int)iso_graph.size());

	for(int i=0;i<(int)iso_graph.size();i++){
		if(!visit[i]){
			vector<int> cc;
			get_cc(i, iso_graph, visit, cc);
			catalog.push_back(cc);
		}
	}

	return catalog;
}

//Get the catalog file
void get_catalog_file(vector<vector<int>> &iso_graph, string &konect_file){
	vector<vector<int>> catalog = get_catalog(iso_graph);

	string catalog_file;
	int pos1, pos2;

	for(int i=0;i<(int)konect_file.size();i++){
		if(konect_file[i] == '/'){
			pos1 = i;
			break;
		}
	}

	for(int i=0;i<(int)konect_file.size();i++){
		if(konect_file[i] == '.'){
			pos2 = i;
			break;
		}
	}

	catalog_file = konect_file.substr(0,pos1+1);
	catalog_file += konect_file.substr(pos1+1, pos2-pos1-1);
	catalog_file += "_catalog.out";
	ofstream fout(catalog_file);

	fout << "Catalog with isomorphisms (by communities indices):" << endl;
	for(int i=0;i<(int)catalog.size();i++){
		fout << "Class " << i << ": ";
		for(int j=0;j<(int)catalog[i].size();j++){
			fout << catalog[i][j] << " ";
		}
		fout << endl;
	}
}

//Get the number of bits used before the compression
int get_bits_before_compression(vector<int> &quant_vertices_per_community){
	int  bits = (int)ceil((float)log2((float)quant_vertices_per_community.size()));

	for(int i=0;i<(int)quant_vertices_per_community.size();i++)
		bits += (quant_vertices_per_community[i]*(quant_vertices_per_community[i] - 1))/2;

	return bits;
}

//Get the number of bits used after the compression
int get_bits_after_compression(vector<vector<int>> &iso_graph, vector<vector<ii>> &communities_edges, vector<int> &quant_vertices_per_community){
	vector<vector<int>> catalog = get_catalog(iso_graph);
	int bits = (int)ceil((float)log2((float)catalog.size()));
	bool entry = false;

	for(int i=0;i<(int)catalog.size();i++){
		if(catalog[i].size() > 0 && quant_vertices_per_community[catalog[i][0]] > 0){
			int representant_size = quant_vertices_per_community[catalog[i][0]];
			int inc;

			
			if(test_star(communities_edges[catalog[i][0]], quant_vertices_per_community[catalog[i][0]]) ||
		 	test_cycle(communities_edges[catalog[i][0]], quant_vertices_per_community[catalog[i][0]]) ||
		  	test_path(communities_edges[catalog[i][0]], quant_vertices_per_community[catalog[i][0]]) ||
		   	test_clique(communities_edges[catalog[i][0]], quant_vertices_per_community[catalog[i][0]]))
				inc = min((int)ceil((float)log2((float)representant_size)) + 2, (representant_size*(representant_size - 1))/2);
			else inc = (representant_size*(representant_size - 1))/2;  

			bits += inc;
		}
	}

	return bits;
}

void print_weak_iso_pairs (vector<ii> &weak_iso, vector<vector<ii>> &communities_edges,  vector<int> &quant_vertices_per_community, ofstream &fout){
	for(int i=0;i<(int)weak_iso.size();i++){
		fout << quant_vertices_per_community[weak_iso[i].first] << " " << communities_edges[weak_iso[i].first].size() << endl;
		print_adj_matrix(communities_edges[weak_iso[i].first], fout);
		fout << endl;
		fout << quant_vertices_per_community[weak_iso[i].second] << " " << communities_edges[weak_iso[i].second].size() << endl;
		print_adj_matrix(communities_edges[weak_iso[i].second], fout);
		fout << endl << "------------------------------------" << endl;
	}
}

string get_weak_iso_file(string &fin){
	int pos1, pos2;

	for(int i=0;i<(int)fin.size();i++){
		if(fin[i] == '/'){
			pos1 = i;
			break;
		}
	}

	for(int i=0;i<(int)fin.size();i++){
		if(fin[i] == '.'){
			pos2 = i;
			break;
		}
	}

	string fout = fin.substr(0,pos1+1);
	fout += fin.substr(pos1+1, pos2-pos1-1);
	fout += "_weak_iso.out";
	return fout;
}

int main(int argc, char* argv[]){

	if(argc < 3){
		cout << "Pass the file with the tags and the file with the konect format as parameters" << endl;
		return 0;
	}

	string partition_file = argv[1];
	string konect_file = argv[2];

	vector<int> tag = get_tag(partition_file);
	int quant_vertices = tag.size();
	int quant_communities = get_communities_number(tag);
	vector<int> quant_vertices_per_community = get_quant_vertices_per_community(tag, quant_communities);
	vector<ii> edges = get_edges(konect_file);
	compress_coordinates(edges);
	vector<vector<int>> communities_vertices = get_communities_vertices(tag, quant_communities);
	vector<vector<ii>> communities_edges = get_communities_edges(edges, tag, quant_communities);

	for(int i=0;i<quant_communities;i++) compress_coordinates(communities_edges[i]);

	vector<ii> weak_iso;
	ofstream fout(get_weak_iso_file(konect_file));
	vector<vector<int>> iso_graph = get_iso_graph(fout, communities_edges, quant_vertices_per_community, quant_communities, weak_iso);

	int bits_before_compression = get_bits_before_compression(quant_vertices_per_community);
	cout << "Number of bits before compression: " << bits_before_compression << endl;
	int bits_after_compression = get_bits_after_compression(iso_graph, communities_edges, quant_vertices_per_community);
	cout << "Number of bits after compression: " << bits_after_compression << endl;
	cout << "Compression rate in terms of bits: " << (1.0*(bits_before_compression - bits_after_compression))/bits_before_compression << endl;
	
	get_catalog_file(iso_graph, konect_file);
	return 0;
}
