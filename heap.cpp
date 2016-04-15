#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

struct entry {
	int e;
	int depth;
	bool isRightChild;
	bool hasRightChild;
};

vector<int> readFile(char fname[]) {
    
	std::list<int> dl;
	
	fstream input;
	input.open(fname);
	
	int a;
	while (input >> a) {
		dl.push_back(a);
	}
	
	vector<int> dv{ dl.begin(), dl.end() };
	
    return dv;
}

int parentIndex(int i) {
    return (i - 1) / 2;
}

int leftChildIndex(int i) {
    return 2 * i + 1;
}

int rightChildIndex(int i) {
    return 2 * i + 2;
}

void upHeap(vector<int> &data, int n) {
    if (n == 0) {
        return;
	}
    int p = parentIndex(n);
    if (data[p] >= data[n]) {
        return;
    } else {
		swap(data[p], data[n]);
        upHeap(data, p);
	}
}

void buildHeap(vector<int> &data) {
    int n = data.size();
	for (int i = 0; i < n; ++i) {
        upHeap(data, i);
	}
}

void downHeap(vector<int> &data, int n, int stop) {
    int left = leftChildIndex(n);
    int right = rightChildIndex(n);
	
    if (left >= stop) {
        // case 1: already a leave
        return;
    } else if (left < stop && right >= stop) {
        // case 2: only have a left child
        if (data[n] < data[left]) {
			swap(data[n], data[left]);
            downHeap(data, left, stop);
        } else return;
    } else {
        // case 3: has both left and right children
        if (data[n] >= data[left] && data[n] >= data[right]) {
            return;
        } else {
            int largestChild = left;
            if (data [left] < data [right]) largestChild = right;
			
			swap(data [n], data[largestChild]);
            downHeap(data, largestChild, stop);
		}
	}
}

void sortHeap(vector<int> &data) {
    int n = data.size();
	for (int i = n - 1; i > 0; --i) {
		swap(data [0], data [i]);
        downHeap(data, 0, i);
	}
}

template<typename T> vector<T> concVect(vector<T> a, vector<T> b) {
	vector<T> ab;
	ab.reserve(a.size() + b.size());
	ab.insert(ab.end(), a.begin(), a.end());
	ab.insert(ab.end(), b.begin(), b.end());
	return ab;
}

vector<entry> inorder(vector<int> &data, int n = 0, int depth = 0) {
    int nn = data.size();
    if (n >= nn) {
		vector<entry> out;
        return out;
	}
    int left = leftChildIndex(n);
    int right = rightChildIndex(n);
    bool isRightChild = (n % 2) == 0; // only exception is for the root
    bool hasRightChild = (n * 2 + 2) < nn;
	
	entry e = { data[n], depth, isRightChild, hasRightChild};
	vector<entry> a(1, e);
	vector<entry> b = concVect(inorder(data, left, depth+1), a);
	return concVect(b, inorder(data, right, depth+1));
}

string updateBars (string bars, int depth, bool isRightChild, bool hasRightChild) {
    int location1 = 5 * depth - 4; // location for level depth - 1
    int location2 = location1 + 5; // location for level depth
    if (bars.length() < location2) {
        bars = bars + string(location2 - bars.length(), ' ');
	}
    if (depth == 0) {
        // this is the root, no bar to remove
        
    } else if (isRightChild) {
        // remove the bar at level depth - 1
        bars.insert(location1, " ");
    } else {
        // add a bar at level depth - 1
        bars.insert(location1, "|");
	}
    if (!hasRightChild) {
        // remove the bar at level depth, since no one else will remove it.
        bars.insert(location2, " ");
	}
    return bars;
}


string drawTree(vector<int> data) {
    string bars = "";
    list<string> results;
    vector<entry> items = inorder(data);
	for (int i = 0; i < items.size(); ++i) {
		int value = items[i].e;
		int depth = items[i].depth;
		bool isRightChild = items[i].isRightChild;
		bool hasRightChild = items[i].hasRightChild;
		
        results.push_back(bars);
        string valueStr = to_string(value);
        if (!isRightChild) {
            bars = updateBars(bars, depth, isRightChild, hasRightChild);
		}
        if (depth == 0) results.push_back(valueStr);
        else {
            int barPrefix = (depth-1) * 5;
			string temp = bars.substr(0, barPrefix);
			temp += " +---" + valueStr;
            results.push_back(temp);
		}
        if (isRightChild) bars = updateBars (bars, depth, isRightChild, hasRightChild);
        results.push_back(bars);
	}
    results.reverse();
	string temp = "";
	for (list<string>::const_iterator i = results.begin(); i != results.end(); ++i) {
		temp += *i + "\n";
	}
    return temp;
}

string sortData(vector<int> data) {
    buildHeap(data);
    string result = drawTree(data);
    sortHeap(data);
    return result;
}

bool verifySorted(vector<int> data) {
    int n = data.size();
    if (n < 2) {
        //# std::list of length less than two is sorted by definition
        return true;
    } else {
		for (int i = 1; i < n; ++i) {
            if (data[i-1] > data[i]) return false;
		}
        return true;
	}

}

template<typename T> void printV(vector<T> v) {
	
	cout << "{";
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i != v.size() - 1) cout << ", ";
		else cout << "}" << endl;
	}
	
}

void processData(vector<int> data) {

	string heap = sortData(data);
	if (!verifySorted(data)) {
		cout << "***** Warning: Data not sorted. *****" << endl;
	}
	cout << heap << endl;
	printV(data);
}

int main(int n, char **args[]) {
	
	vector<int> v = {3,1,4,1,5,9,2,6,5,3,6};
	
    processData(v);
    processData(readFile ("data.txt"));
}

    
