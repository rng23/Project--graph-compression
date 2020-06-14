/************************************************************************************************
Author: joaopfg

To compile: g++ -o get_metis_format get_metis_format.cpp

Input: Give the Konect file (.edges).
Example: ./get_metis_format test/test.edges

Output: File with the graph in the metis format (adjacency list)
Example: test/test_metis.graph
************************************************************************************************/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> ii;

int main(int argc, char *argv[]){
	string fin, fout;

	if(argc < 2){
		cout << "Please, provide the path to the file you want to convert!" << endl;
		return 0;
	}

	fin = argv[1];
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
	cout << "fout name: " << fout << endl;
	
	vector<ii> edges;
	set<int> vertices;
	set<int>::iterator it;
	map<int, int> contraction;
	int x, y, cont = 1;

	ifstream file_in(fin);
	while(file_in.is_open()){
		while(file_in >> x >> y){
			vertices.insert(x);
			vertices.insert(y);
			edges.push_back({x, y});
			edges.push_back({y, x});
		} 
		file_in.close();
	}

	for(it = vertices.begin(); it != vertices.end(); it++){
		contraction[*it] = cont;
		cont++; 
	}

	map<int,int>::iterator it_map;

	sort(edges.begin(), edges.end());

	for(int i=0;i<(int)edges.size();i++){
		edges[i].first = contraction[edges[i].first];
		edges[i].second = contraction[edges[i].second];
	}

	ofstream file_out(fout);
	while(file_out.is_open()){
		file_out << contraction.size() << " " << edges.size()/2 << endl;
		cont = 1;

		for(int i=0;i<(int)edges.size();i++){
			if(edges[i].first == cont) file_out << edges[i].second << " ";
			else{
				file_out << endl;
				file_out << edges[i].second << " ";
				cont++;
			}
		}
		file_out.close();
	}
	return 0;
}