#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#define NMAX 1000005
using namespace std;
int lazy[100000] = {0};

//Time complexity is O(logN)
int querySegmentTree(int *tree,int s,int e,int qs,int qe,int index) {
	//No Overlap
	if(qs>e || qe<s){
		return INT_MAX;
	}

	//Complete Overlap
	if(s>=qs && e<=qe){
		return tree[index];
	}

	//Partial Overlap
	int middle = (s+e)/2;
	int leftAns = querySegmentTree(tree,s,middle,qs,qe,2*index);
	int rightAns = querySegmentTree(tree,middle+1,e,qs,qe,2*index+1);
	return min(leftAns,rightAns);
}

//Time complexity is O(logN)
void updateNodeSegmentTree(int *tree,int s,int e,int i,int inc,int index) {

	//No Overlap
	if(i > e || i < s){
		return;
	}

	//Leaf Node
	if(s==e){
		tree[index] += inc;
		return;
	}

	//If i is lying in range of s and e
	int middle = (s + e)/2;
	updateNodeSegmentTree(tree,s,middle,i,inc,2*index);
	updateNodeSegmentTree(tree,middle+1,e,i,inc,2*index+1);
	tree[index] = min(tree[2*index] , tree[2*index+1]);
	return;
}

//Time complexity is O(N) in worst case
void updateRangeSegmentTree(int *tree,int s,int e,int l,int r,int inc,int index) {

	//No Overlap
	if(l > e || r < s){
		return;
	}

	//Leaf node
	if(s==e){
		tree[index] += inc;
		return;
	}

	//Partial overlap
	int middle = (s+e)/2;
	updateRangeSegmentTree(tree,s,middle,l,r,inc,2*index);
	updateRangeSegmentTree(tree,middle+1,e,l,r,inc,2*index+1);
	tree[index] = min(tree[2*index] , tree[2*index+1]);
	return;
}

//Optimize Code for updation of range --> Lazy Propogation
// This will work in O(log(N))
void updateRangeLazy(int *tree,int s,int e,int l,int r,int inc,int index) {
	//First task is to make Pending updates
	if(lazy[index] != 0) {
		tree[index] += lazy[index];
		//if current node is not a leaf node 
		if(s != e){
			lazy[2*index] += lazy[index];
			lazy[2*index+1] += lazy[index];
		}
		lazy[index] = 0;
	}

	//No Overlap
	if(l>e || r<s){
		return;
	}

	//Complete Overlap
	if(s>=l && e<=r) {
		tree[index] += inc;
		if(s != e){
			lazy[2*index] += inc;
			lazy[2*index+1] += inc;
		}
		return;
	}

	//Partial overlap
	int middle = (s+e)/2;
	updateRangeLazy(tree,s,middle,l,r,inc,2*index);
    updateRangeLazy(tree,middle+1,e,l,r,inc,2*index+1);
    tree[index] = min(tree[2*index],tree[2*index+1]);
    return;
}

int queryLazy(int *tree,int s,int e,int qs,int qe,int index) {
	if(lazy[index] != 0) {
		tree[index] += lazy[index];
		
		if(s!=e){
			lazy[2*index] += lazy[index];
			lazy[2*index+1] += lazy[index];
		}

		lazy[index] = 0;
	}

	//No Overlap
	if(qs>e || qe<s){
		return INT_MAX;
	} 
	
	//Complete Overlap
	if(s>=qs && e<=qe) {
		return tree[index];
	}

	//Partial Overlap
	int middle = (s+e)/2;
	int left = queryLazy(tree,s,middle,qs,qe,2*index);
	int right = queryLazy(tree,middle+1,e,qs,qe,2*index+1);

	return min(left,right);
}

//Time complexity is O(N)
void buildSegmentTreeHelper(int *a,int s,int e,int *tree,int index) {
	//Base Case
	if(s == e) {
		tree[index] = a[s];
		return ;
	}

	//Recursive Case
	int middle = (s + e) / 2;
	
	buildSegmentTreeHelper(a,s,middle,tree,2*index);

	buildSegmentTreeHelper(a,middle + 1,e,tree,2*index+1);
	tree[index] = min(tree[2*index] , tree[2*index + 1]);
	return;
}

int* buildSegmentTree(int *a,int n) {
	int* segmentTree = new int[4*n+1];

	// Helper function to build segment tree 
	buildSegmentTreeHelper(a,0,n-1,segmentTree,1);

	return segmentTree;
}

int main(int argc, char const *argv[])
{
	int n;
	cout << "Enter the number of elements in array\n";
	cin >> n;
	int a[n];
	cout << "Enter array elements\n";
	for(int i = 0; i < n; i++){
		cin >> a[i];
	}
	int q,t,qs,qe,i,inc,l,r;
	char ch;
	cout << "Enter number of queries\n";
	cin >> q;
	int index = 1;

	//tree is a dynamic array which will contain the elements of segment tree
	int *tree = buildSegmentTree(a,n);

	while(q--){
		cin >> ch;

		if(ch == 'Q'){
			cin >> qs >> qe;
			cout << "Answer of range is " << queryLazy(tree,0,n-1,qs,qe,1);
			cout << '\n';
		}
		else{
			cin >> l >> r >> inc;
			updateRangeLazy(tree,0,n-1,l,r,inc,1);
		}
	}
	
	return 0;
}