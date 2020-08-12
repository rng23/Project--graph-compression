/************************************************************************************************
Author: joaopfg

To compile: g++ get_community.cpp -o get_community

When running, give partition file and Konect file
Example: 
./get_community caida/caida_metis.graph.part.2647 caida/caida.edges  

This program gives the compressed graphes of each community (taking out isolated vertex) in the
object 'communities_edges'.

Tested with:

Caida 26K

Livemocha 104k

Amazon mds 334k

Orkut 3M

Dolphin 62 nodes

Hyves 1M

Com-DBLP 317K

Wordnet 146k

Proteins 1.8k
************************************************************************************************/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> ii;

struct stats{
	int min, max;
	double mean;
}; 

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

//Get the quantity of communities using the metis partition file
int get_communities_number(vector<int> &tag){
	set<int> tag_set;

	for(int i=0;i<(int)tag.size();i++)
		tag_set.insert(tag[i]);

	return tag_set.size();
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

//Get communities file
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

int main(int argc, char* argv[]){

	if(argc < 3){
		cout << "Pass the file with the tags and the file with the konect format as parameters" << endl;
		return 0;
	}

	string partition_file = argv[1];
	string konect_file = argv[2];
	ifstream fin(konect_file);
	vector<int> tag = get_tag(partition_file);
	int quant_vertices = tag.size();
	int quant_communities = get_communities_number(tag);
	vector<int> quant_vertices_per_community = get_quant_vertices_per_community(tag, quant_communities);
	vector<ii> edges = get_edges(konect_file);
	compress_coordinates(edges);
	vector<vector<ii>> communities_edges = get_communities_edges(edges, tag, quant_communities);
	for(int i=0;i<quant_communities;i++) compress_coordinates(communities_edges[i]);
	get_communities_file(communities_edges, quant_vertices_per_community, konect_file);

	return 0;
}
