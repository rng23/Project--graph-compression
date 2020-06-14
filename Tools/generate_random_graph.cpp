/************************************************************************************************
Author: joaopfg

To compile: g++ -o generate_random_graph generate_random_graph.cpp
To run: ./generate_random_graph size_communities quant_communities

This program generates a random graph in the Konect format taking as parameter the size of the communities and
the quantity of communities.

At each iteration it generates a community by choosing at random between star, cycle, path and clique.

Then it connects a random vertex of the current community to a random vertex 
of the previous community (choosen at random the points of connection). 
************************************************************************************************/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> ii;

int random_int(int lower_bound, int upper_bound){
   	random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<int> dis(lower_bound, upper_bound);
    return dis(gen);
}

//type = 1: star
//type = 2: cycle
//type = 3: path
//type = 4: clique
vector<ii> generate_random_graph(int size_communities, int quant_communities){
	vector<ii> graph;

	for(int i=1;i<=quant_communities;i++){
		int type = random_int(1, 4);
		if(type == 1){
			for(int j=1;j<size_communities;j++)
				graph.push_back({(i-1)*size_communities, (i-1)*size_communities + j});
		}
		else if(type == 2){
			for(int j=0;j<size_communities;j++)
				graph.push_back({min((i-1)*size_communities + j, (i-1)*size_communities + (j+1)%size_communities), max((i-1)*size_communities + j, (i-1)*size_communities + (j+1)%size_communities)});	
		}
		else if(type == 3){
			for(int j=0;j<size_communities-1;j++)
				graph.push_back({min((i-1)*size_communities + j, (i-1)*size_communities + (j+1)%size_communities), max((i-1)*size_communities + j, (i-1)*size_communities + (j+1)%size_communities)});
		}
		else{
			for(int j=0;j<size_communities;j++){
				for(int z=j+1;z<size_communities;z++){
					graph.push_back({(i-1)*size_communities + j, (i-1)*size_communities + z});
				}
			}
		}

		if(i > 1){
			int connexion = random_int(1,i-1);
			int ind1 = random_int(0,size_communities-1);
			int ind2 = random_int(0,size_communities-1);
			graph.push_back({(connexion-1)*size_communities + ind1, (i-1)*size_communities + ind2});
		}
	}

	sort(graph.begin(), graph.end());

	for(int i=0;i<(int)graph.size();i++){
		graph[i].first++;
		graph[i].second++;
	}

	return graph;
}

//Auxiliar function to print the adjacency matrix
void print_adj_matrix(vector<ii> &edges){
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

	for(int i=1;i<=quant_vertices;i++){
		for(int j=0;j<i;j++) cout << " ";
		for(int j=i;j<=quant_vertices;j++) cout << adj_mat[i][j];
		cout << endl;
	}
}

//Print the random graph in the Knoect format to a file
void print_random_graph(int size_communities, int quant_communities, string &file_name){
	vector<ii> edges = generate_random_graph(size_communities, quant_communities);
	ofstream edges_file(file_name);

	while(edges_file.is_open()){
		for(int i=0;i<(int)edges.size();i++)
			edges_file << edges[i].first << " " << edges[i].second << endl;

		edges_file.close();
	}

	//print_adj_matrix(edges);
}

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Please, provide the path to the konect file and the path to the metis partition!" << endl;
		return 0;
	}

	int size_communities = stoi(argv[1]);
	int quant_communities = stoi(argv[2]);

	string file_name = "test/test.edges";
	print_random_graph(size_communities, quant_communities, file_name);
}