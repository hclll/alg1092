// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2021/02/26 Hsien-Chia Chen]
// **************************************************************************

#include "sort_tool.h"
#include<iostream>
#include<cmath>
#include<vector>

// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
    
    for(int j=1; j<=data.size()-1; j++){
        //cout<<"j"<<j<<endl;
        int key = data[j];
        //cout<< key<< endl;
        int i = j - 1;
        while(i>=0 && data[i]>key){
            data[i+1] = data[i];
            i -= 1;
        }   
        data[i+1] = key;

    }
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data){
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high) {
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed

	if(low < high){
		int q = Partition(data, low, high);
		QuickSortSubVector(data, low, q-1);
		QuickSortSubVector(data, q+1, high);
	}
}

int SortTool::Partition(vector<int>& data, int low, int high) {
    // Function : Partition the vector 
    // TODO : Please complete the function
    // Hint : Textbook page 171

    int x = data[high];
    int i = low - 1;
    for(int j=low; j<=high-1; j++){
    	if(data[j] <= x){
    		i += 1;
    		swap(data[i], data[j]);
    	} 
    }
    swap(data[i+1], data[high]);
    return i+1;
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed

    if(low < high){
        int q = floor((low + high) / 2);
        MergeSortSubVector(data, low, q);
        MergeSortSubVector(data, q+1, high);
        Merge(data, low, q, q+1, high); 
    }
}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    const int infin = 10000000;
    int n1 = middle1 - low +1;
    int n2 = high - middle2 +1;
    //vector<int> test = {0,1,2,3,4,5};
    
    vector<int> Left(data.begin()+low, data.begin()+middle1+1);
    vector<int> Right(data.begin()+middle2, data.begin()+high+1);
    //cout<<"Left"<<Left[Left.size()-1]<<endl;
    //cout<<"Right"<<Right[Right.size()-1]<<endl;
    Left.insert(Left.end(), infin);
    Right.insert(Right.end(), infin);
    //cout<<"Left"<<Left[Left.size()-1]<<endl;
    //cout<<"Right"<<Right[Right.size()-1]<<endl;
    int i = 0, j = 0;
    for(int k=0; k<n1+n2; k++){
        if(Left[i] <= Right[j]){
            data[low+k] = Left[i];
            i += 1;
        }
        else{
            data[low+k] = Right[j];
            j += 1;
        }
    }

}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    heapSize = BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
	int tmp = 0;
    for (int i = data.size() - 1; i >= 1; i--) {
		tmp = data[i];
		data[i] = data[0];
		data[0] = tmp;
        heapSize--;
        
        MaxHeapify(data, 0, heapSize);

    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root, int heapSize) {
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int l = (root+1)*2-1;
    int r = (root+1)*2;
    int largest;
    
    if((l <= heapSize-1) && (data[l] > data[root])){
        largest = l;
    }
    else{
        largest = root;
    }
    if((r <= heapSize-1) && (data[r] > data[largest])){
        largest = r;
    }
    if(largest != root){
        swap(data[largest], data[root]);
        MaxHeapify(data, largest, heapSize);
    }
}

//Build max heap
int SortTool::BuildMaxHeap(vector<int>& data) {
    int heapSize = data.size(); // initialize heap size
    //cout<<"b"<<heapSize<<endl;
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for(int i=floor(data.size()/2)-1; i>=0; i--){
        MaxHeapify(data, i, heapSize);
    }
    /*
    for (int i=0; i<=data.size()-1; i++){
        cout<<data[i]<<endl;
    }
    cout<<"//"<<endl;
    */
    return heapSize;
}
