#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include <map>
#include <algorithm>

using namespace std;

vector<int> chords;
int total_num;

int maxPlanarSubset(int i, int j, vector<int>& chords, vector<vector<int> >& M, vector<vector<int> >& case_path);
void findChord(int i, int j, vector<vector<int> >& case_path, vector<pair<int,int> >& resulting_chord_set);

void help_message() {
    cout << "usage: ./bin/mps inputs/<input file name> outputs/<output file name>" << endl;
    
}


int main(int argc, char* argv[]){

    if(argc != 3) {
       help_message();
       return 0;
    }

    /////////read file/////////
    fstream fin(argv[1]);
    int x, y;
    fin >> total_num;
    chords.resize(total_num);
    for (int i = 0; i<total_num/2; i++) {
        fin >> x >> y;
        chords[x] = y;
        chords[y] = x;
    }

    fin.close();
    

    vector<vector<int> > M(total_num,vector<int> (total_num,-1));
    vector<vector<int> > case_path(total_num,vector<int> (total_num,0));
    vector<pair<int,int> > resulting_chord_set;
    

    int max_num = maxPlanarSubset(0, total_num-1, chords, M, case_path);
    cout<<"finish_mps"<<endl;
    findChord(0, total_num-1, case_path, resulting_chord_set);
    sort(resulting_chord_set.begin(), resulting_chord_set.end());


    //////////write file/////////
    fstream fout;
    fout.open(argv[2], ios::out);
    fout << max_num << endl;
    for(int i=0; i<resulting_chord_set.size(); i++){
    	fout << resulting_chord_set[i].first << " " << resulting_chord_set[i].second << endl;
    }
    fout.close();

    return 0;    

}

int maxPlanarSubset(int i, int j, vector<int>& chords, vector<vector<int> >& M, vector<vector<int> >& case_path){
    
    if(i>j){
        return 0;
    }

    if(M[i][j]!=-1){
    	return M[i][j];
    }

    if(i==j){
    	M[i][j] = 0;
    	return M[i][j];
    }

    int k = chords[j];
    
    //case1
	if(k<i || k>j){
		M[i][j] = maxPlanarSubset(i, j-1, chords, M, case_path);
		case_path[i][j] = 1;
		
	}
	//case2
	else if(k==i){
		M[i][j] = maxPlanarSubset(i+1, j-1, chords, M, case_path) + 1;
		case_path[i][j] = 2;
		
	}
	//case3,4
	else{
		int a = maxPlanarSubset(i, k-1, chords, M, case_path)+1+maxPlanarSubset(k+1, j-1, chords, M, case_path);
		int b = maxPlanarSubset(i, j-1, chords, M, case_path);
		if(a>b){
			M[i][j] = a;
			case_path[i][j] = 3;
			
		}
		else{
			M[i][j] = b;
			case_path[i][j] = 4;
			
		}

	}
	return M[i][j];
}


void findChord(int i, int j, vector<vector<int> >& case_path, vector<pair<int,int> >& resulting_chord_set){ //////basecase
	if(i<j){
		if(case_path[i][j]==1 || case_path[i][j]==4){
			findChord(i,j-1, case_path, resulting_chord_set);
		}
		else if(case_path[i][j]==2){
			resulting_chord_set.push_back(make_pair(i,j));
			findChord(i+1,j-1, case_path, resulting_chord_set);
		}
		else if(case_path[i][j]==3){
            int k = chords[j];
			resulting_chord_set.push_back(make_pair(k,j));
			findChord(i,k-1, case_path, resulting_chord_set);
			findChord(k+1,j-1, case_path, resulting_chord_set);
		}
	}
	
}

