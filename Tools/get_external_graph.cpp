#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> ii;

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

pair<vector<ii>, string> get_external_graph_file(int quant_vertices, vector<ii> &edges, vector<int> &tag, string &konect_file){
	vector<ii> external_graph;

	for(int i=0;i<(int)edges.size();i++){
		if(tag[edges[i].first] != tag[edges[i].second])
			external_graph.push_back({edges[i].first, edges[i].second});
	}

	string file_name;
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

	file_name = konect_file.substr(0,pos1+1);
	file_name += konect_file.substr(pos1+1, pos2-pos1-1);
	file_name += "_external_graph.out";

	ofstream fout(file_name);

	while(fout.is_open()){
		//fout << quant_vertices << " " << external_graph.size();

		for(int i=0;i<(int)external_graph.size();i++){
			fout << " " << external_graph[i].first << " " << external_graph[i].second;
		}

		fout.close();
	}

	return {external_graph, file_name};
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
	vector<ii> edges = get_edges(konect_file);
	compress_coordinates(edges);
	vector<ii> external_graph = get_external_graph_file(quant_vertices, edges, tag, konect_file).first;

	return 0;
}