#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include <map>
#include <algorithm>

using namespace std;
map<int,int> chord_set;
vector<pair<int,int> > resulting_chord_set;
int total_num;
// int M[12][12];
// int case_path[12][12];


void maxPlanarSubset(map<int,int>& chord_set, int& total_num, vector<vector<int> >& M, vector<vector<int> >& case_path);
void findChord(int i, int j, vector<vector<int> >& case_path);

void help_message() {
    cout << "usage: ./bin/mps inputs/<input file name> outputs/<output file name>" << endl;
    
}


int main(int argc, char* argv[]){

    if(argc != 3) {
       help_message();
       return 0;
    }

    /////////read file/////////
    //fstream fin("inputs/12.in");
    fstream fin(argv[1]);
    string line;
    total_num = -1;
    while(getline(fin,line)){
        std::istringstream is(line);
        int n;
        if(total_num == -1){
            if(is>>n){
                total_num = n;
            }
        }
        int a, b;
        if(is>>a>>b)  
        {  
            chord_set.insert(pair<int,int> (a,b)); 
        }
        
    }
    /*
    for(int i = 0; i < chord_set.size(); i++)
    {
        cout << chord_set[i].first << ", " << chord_set[i].second << endl;
    }
    cout<<total_num<<endl;
    */
    //cout << chord_set[chord_set.size()-1].first << ", " << chord_set[chord_set.size()-1].second << endl;
    //cout<<total_num<<endl;

    fin.close();
    
    // cout<<find(chord_set.begin(), chord_set.end(), p)->first<<endl;

    vector<vector<int> > M(total_num,vector<int> (total_num));
    vector<vector<int> > case_path(total_num,vector<int> (total_num));

    maxPlanarSubset(chord_set, total_num, M, case_path);
    cout<<"finish_mps"<<endl;
    findChord(0, total_num-1, case_path);
    sort(resulting_chord_set.begin(), resulting_chord_set.end());

    /*
    cout<<"ans"<< M[0][total_num-1]<<endl;
    for(int i = 0; i < resulting_chord_set.size(); i++)
    {
        cout << resulting_chord_set[i].first << ", " << resulting_chord_set[i].second << endl;
    }
    */

    //////////write file/////////
    fstream fout;
    //fout.open("outputs/12.out", ios::out);
    fout.open(argv[2], ios::out);
    fout << M[0][total_num-1] << endl;
    for(int i=0; i<resulting_chord_set.size(); i++){
    	fout << resulting_chord_set[i].first << " " << resulting_chord_set[i].second << endl;
    }
    fout.close();

    return 0;    

}


int findByKey(map<int,int>& chord_set, int key){
	int k;
	auto iter = chord_set.find(key);
	if(iter!=chord_set.end()){
		k = iter->second;
	}
	else{
		k = -1;
	}
	return k;
}

int findByValue(map<int,int>& chord_set, int value){
	int k;
	auto iter = chord_set.begin();
	while(iter!=chord_set.end()){
		if(iter->second==value){
			k = iter->first;
			return k;
		}
		iter++;
	}
	k=-1;
	return k;
}

int find_k(map<int,int>& chord_set, int j){
	int k = -1;
	for(map<int,int>::iterator iter=chord_set.begin(); iter!=chord_set.end(); iter++) {  
    	if(iter->first==j){
    		k = iter->second;
    		return k;
    	}
    	else if(iter->second==j){
    		k = iter->first;
    		return k;
    	}
    }
    return k;  
}

void maxPlanarSubset(map<int,int>& chord_set, int& total_num, vector<vector<int> >& M, vector<vector<int> >& case_path){
	
    for(int i=0; i<=total_num-1; i++){
        M[i][i] = 0;
    }
    int j, k;
    for(int l=1; l<=total_num-1; l++){
        for(int i=0; i<=total_num-l-1; i++){
            j = i + l;
            k = find_k(chord_set, j);

            pair<int,int> p;
            if(j>k){p = make_pair(k,j);}
            else{p = make_pair(j,k);}

        
        	//case1
        	if(k<i || k>j || k==-1){
        		M[i][j] = M[i][j-1];
        		case_path[i][j] = 1;
        	}
        	//case2
        	else if(k==i){
        		M[i][j] = M[i+1][j-1] + 1;
        		case_path[i][j] = 2;
        	}
        	//case3,4
        	else{
        		//M[i][j] = max(M[i][k-1]+1+M[k+1][j-1], M[i][j-1]);
        		
        		if(M[i][k-1]+1+M[k+1][j-1] >= M[i][j-1]){
        			M[i][j] = M[i][k-1]+1+M[k+1][j-1];
        			case_path[i][j] = 3;
        		}
        		else{
        			M[i][j] = M[i][j-1];
        			case_path[i][j] = 4;
        		}

        	}
            
            
        }
    }
}

void findChord(int i, int j, vector<vector<int> >& case_path){ //////basecase
	if(i<j){
		if(case_path[i][j]==1 || case_path[i][j]==4){
			findChord(i,j-1, case_path);
			//cout<<"a"<<i<<","<<j<<endl;
		}
		else if(case_path[i][j]==2){
			resulting_chord_set.push_back(make_pair(i,j));
			findChord(i+1,j-1, case_path);
			//cout<<"b"<<i<<","<<j<<endl;
		}
		else if(case_path[i][j]==3){
			int k = find_k(chord_set, j);
			resulting_chord_set.push_back(make_pair(k,j));
			findChord(i,k-1, case_path);
			findChord(k+1,j-1, case_path);
			//cout<<"c"<<i<<","<<j<<endl;
		}
	}
	
}

