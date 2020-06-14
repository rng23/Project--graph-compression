/************************************************************************************************
Author: joaopfg

To compile: g++ -o get_graphviz_format get_graphviz_format.cpp

Input: Give the Konect file as argument
Example: ./get_graphviz_format test/test.edges

Output: File with the graph in graphviz format
Example: test/test_graphviz.gv

To generate the file with the draw, run:
dot -Tps test/test_graphviz.gv -o test/test_graphviz.ps
************************************************************************************************/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> ii;

void get_graphviz_format(string &fin){
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
	fout += "_graphviz.gv";
	cout << "fout name: " << fout << endl;
	
	int x, y;
	vector<ii> edges;
	set<int> vertices;
	set<int>::iterator it;

	ifstream file_in(fin);
	while(file_in.is_open()){
		while(file_in >> x >> y){
			vertices.insert(x);
			vertices.insert(y);
			edges.push_back({x, y});
		} 

		file_in.close();
	}

	ofstream file_out(fout);
	while(file_out.is_open()){
		file_out << "digraph G {" << endl;

		for(it = vertices.begin();it != vertices.end(); it++){
			file_out << *it << " [shape=circle]" << endl;
		}

		file_out << endl;

		for(int i=0;i<(int)edges.size();i++){
			file_out << edges[i].first << " -> " << edges[i].second << " [penwidth=5, arrowhead=none]" << endl;
		}

		file_out << "}" << endl;

		file_out.close();
	}
}

/*
int main(int argc, char *argv[]){
	string fin, fout;

	if(argc < 2){
		cout << "Please, provide the path to the file you want to convert!" << endl;
		return 0;
	}

	fin = argv[1];
	get_graphviz_format(fin);

	
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
	fout += "_graphviz.gv";
	cout << "fout name: " << fout << endl;
	
	int x, y;
	vector<ii> edges;
	set<int> vertices;
	set<int>::iterator it;

	ifstream file_in(fin);
	while(file_in.is_open()){
		while(file_in >> x >> y){
			vertices.insert(x);
			vertices.insert(y);
			edges.push_back({x, y});
		} 

		file_in.close();
	}

	ofstream file_out(fout);
	while(file_out.is_open()){
		file_out << "digraph G {" << endl;

		for(it = vertices.begin();it != vertices.end(); it++){
			file_out << *it << " [shape=circle]" << endl;
		}

		file_out << endl;

		for(int i=0;i<(int)edges.size();i++){
			file_out << edges[i].first << " -> " << edges[i].second << " [penwidth=5, arrowhead=none]" << endl;
		}

		file_out << "}" << endl;

		file_out.close();
	}  
	return 0;
}

*/
