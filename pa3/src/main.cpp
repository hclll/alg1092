#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

char type;
int vertices_num;
int edges_num;
vector<vector<int> > G;
vector<pair<int,int> > edges;
vector<pair<int,pair<int,int> > > edges_with_w;
int inf = 10000000;
int neg_inf = -10000000;

void help_message() {
    cout << "usage: ./bin/cb inputs/<input file name> outputs/<output file name>" << endl;
    
}

void read_file(char* file_name){
    fstream fin (file_name);
    
    fin >> type;
    fin >> vertices_num;
    fin >> edges_num;

    G.resize(vertices_num, vector<int> (vertices_num,neg_inf));
    
    int x, y, w;
    for(int i=1; i<=edges_num; i++){
        fin >> x >> y >> w;
        G[x][y] = w;
        edges.push_back(make_pair(x,y));
        edges_with_w.push_back(make_pair(w,make_pair(x,y)));
        
        if(type == 'u' && x != y){
        	G[y][x] = w;
        }
    } 
    fin.close();
}

void write_file(char* file_name, vector<pair<int,int> > removed_edges, vector<int> removed_weights, int total_removed_weight){
	fstream fout;
	fout.open(file_name, ios::out);

	fout << total_removed_weight << endl;
	for(int i=0; i<removed_weights.size(); i++){
		fout << removed_edges[i].first << " " << removed_edges[i].second << " " << removed_weights[i] << endl;
	}
	fout.close();
}

int max_key(vector<bool> visited, vector<int> key){
	int m = neg_inf; 
	int max_ind;
	for(int i=0; i<vertices_num; i++){
		if(visited[i]==false && key[i]>m){
			m = key[i];
			max_ind = i;
		}
	}
	return max_ind;
}

vector< vector<int> > Prim_maxSP(){

	//initialization
	vector<bool> visited(vertices_num, false);
	vector<int> key(vertices_num, neg_inf);
	vector<int> parent(vertices_num, -1);
	key[0] = 0;
	parent[0] = -1;

	vector< vector<int> > SPedges(vertices_num,vector<int> (vertices_num, 0));
	for(int i=0; i<vertices_num; i++){
		int u = max_key(visited, key);
		visited[u] = true;
		key[u] = neg_inf;
		int p = parent[u];
		if(p!=-1){
			SPedges[u][p] = 1;
			SPedges[p][u] = 1;
		}

		for(int v=0; v<vertices_num; v++){
			if(visited[v] == false && G[u][v]>key[v]){
				key[v] = G[u][v];
				parent[v] = u;
			}
		}
	}
	return SPedges;
}

void find_removed_edges(vector<pair<int, int> >& removed_edges, vector<int>& removed_weights, int& total_removed_weight, vector< vector<int> > SPedges){
	total_removed_weight = 0;
	for(int i=0; i<edges_num; i++){
		if(SPedges[edges[i].first][edges[i].second]==0){
			removed_edges.push_back(make_pair(edges[i].first,edges[i].second));
			removed_weights.push_back(G[edges[i].first][edges[i].second]);
			total_removed_weight += G[edges[i].first][edges[i].second];
		}
	
	}

}

void min_egde_in_cycle(int s, int t, vector<int> parent, pair<int,int>& min_edge){
	int now = t;
	int min_weight = inf;
	int tem;
	int count = 0;
	
	while(now!=s){
		
		if(G[parent[now]][now]<min_weight){
			tem = now;
			min_weight = G[parent[now]][now];
		}
		now = parent[now];

		count += 1;
		if(count>vertices_num+1){
			cout<<"sth wrong"<<endl;
			break;
		}
	}
	min_edge = make_pair(parent[tem],tem);
}

void DFS_Visit(vector< vector<int> > T, int u, vector<string>& color, vector<int>& parent, bool& is_cycle, pair<int,int>& min_edge){
	
	int s, t;
	color[u] = "g";

	for(int i=0; i<T[u].size(); i++){
		int v = T[u][i];
		if(color[v]=="w"){
			parent[v] = u;
			DFS_Visit(T, v, color, parent, is_cycle, min_edge);
		}
		else if(color[v]=="g"){
			is_cycle = true;
			t = u;
			s = v;
			min_egde_in_cycle(s, t, parent, min_edge);
			break;
		}
			
		
	}
	color[u] = "b";
}

bool DFS(vector< vector<int> > T, pair<int,int>& min_edge){
	vector<string> color(vertices_num, "w");
	vector<int> parent(vertices_num, -1);
	bool is_cycle = false;
	for(int i=0; i<vertices_num; i++){

		if(is_cycle==true){
			return is_cycle;
		}

		if(color[i]=="w"){
			DFS_Visit(T, i, color, parent, is_cycle, min_edge);
		}
	}
	
	
	return is_cycle;

}

void maxSP2(vector<pair<int,int> >& removed_edges, vector<int>& removed_weights, int& total_removed_weight){
	vector< vector<int> > T; //adj list
	vector<int> tem;
	for(int i=0; i<vertices_num; i++){
		T.push_back(tem);
	}
	pair<int,int> min_edge;

	sort(edges_with_w.begin(), edges_with_w.end());
	for(int i=0; i<edges_num; i++){
		T[edges_with_w[i].second.first].push_back(edges_with_w[i].second.second);
		
		if(DFS(T, min_edge)){
			T[min_edge.first].erase(std::remove(T[min_edge.first].begin(), T[min_edge.first].end(), min_edge.second), T[min_edge.first].end());
			removed_edges.push_back(make_pair(min_edge.first, min_edge.second));
			removed_weights.push_back(G[min_edge.first][min_edge.second]);
			total_removed_weight += G[min_edge.first][min_edge.second];
		}
	}
	
}


int main(int argc, char* argv[]){

    if(argc != 3) {
       help_message();
       return 0;
    }

    
    read_file(argv[1]);
    cout<<type<<endl;

    
    vector< vector<int> > SPedges;
    vector<pair<int,int> > removed_edges;
	vector<int> removed_weights;
	int total_removed_weight;

    if(type=='u'){
    	SPedges = Prim_maxSP();
    	find_removed_edges(removed_edges, removed_weights, total_removed_weight, SPedges);
	}
    else{
    	maxSP2(removed_edges, removed_weights, total_removed_weight);
    }

    

	

	write_file(argv[2], removed_edges, removed_weights, total_removed_weight);
	 

    return 0;    

}

