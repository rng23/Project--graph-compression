#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> ii;

void get_BV_format(string &external_file){
	ifstream fin(external_file);
	vector<ii> edges;
	int x, y;
	bool entry = false;

	while(fin.is_open()){
		while(fin >> x >> y){
			if(entry)
				edges.push_back({x+1,y+1});
			else
				entry = true;
		}

		fin.close();
	}

	sort(edges.begin(), edges.end());

	int pos1, pos2;
	string file_name;

	for(int i=0;i<(int)external_file.size();i++){
		if(external_file[i] == '/'){
			pos1 = i;
			break;
		}
	}

	for(int i=0;i<(int)external_file.size();i++){
		if(external_file[i] == '.'){
			pos2 = i;
			break;
		}
	}

	file_name = external_file.substr(0,pos1+1);
	file_name += external_file.substr(pos1+1, pos2-pos1-1);
	file_name += ".edges";

	ofstream fout(file_name);

	while(fout.is_open()){
		for(int i=0;i<(int)edges.size();i++){
			fout << edges[i].first << " " << edges[i].second << endl;
		}

		fout.close();
	}
}

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Please, provide the file name" << endl;
	}

	string external_file = argv[1];
	get_BV_format(external_file);
	return 0;
}