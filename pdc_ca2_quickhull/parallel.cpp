#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

#define IntegerPair pair<int, int>
#define PADDING 127
#define SEQUENTIAL_LIM 100000000


int findIfAboveOrBelow(IntegerPair p1, IntegerPair p2, IntegerPair p)
{
	int val = (p.second - p1.second) * (p2.first - p1.first) -
			(p2.second - p1.second) * (p.first - p1.first);

	if (val > 0)
		return 1;
	if (val < 0)
		return -1;
	return 0;
}

int OrthogonalLineDist(IntegerPair p1, IntegerPair p2, IntegerPair p)
{
	return abs ((p.second - p1.second) * (p2.first - p1.first) -
			(p2.second - p1.second) * (p.first - p1.first));
}

void quickHull(vector<IntegerPair> &a, int n, IntegerPair p1, IntegerPair p2, int side, vector<vector<bool>> &convex_a, int depth)
{
	int ind = -1;
	int max_dist = 0;

	for (int i=0; i<n; i++)
	{
		int temp = OrthogonalLineDist(p1, p2, a[i]);
		if (findIfAboveOrBelow(p1, p2, a[i]) == side && temp > max_dist)
		{
			ind = i;
			max_dist = temp;
		}
	}
	if (ind == -1)
	{
		return;
	}
    convex_a[ind][PADDING] = true;
    if(n/pow(2, depth) <= SEQUENTIAL_LIM){
        quickHull(a, n, a[ind], p1, -findIfAboveOrBelow(a[ind], p1, p2), convex_a, depth+1);
	    quickHull(a, n, a[ind], p2, -findIfAboveOrBelow(a[ind], p2, p1), convex_a, depth+1);
    }
    else{
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				quickHull(a, n, a[ind], p1, -findIfAboveOrBelow(a[ind], p1, p2), convex_a, depth+1);
			}
			#pragma omp section
			{
				quickHull(a, n, a[ind], p2, -findIfAboveOrBelow(a[ind], p2, p1), convex_a, depth+1);
			}
		}
	}
}

void driverCode(vector<IntegerPair> &a, int n, vector<vector<bool>> &convex_a)
{
	if (n < 3)
	{
		cout << "number of points < 3 therefore Convex hull not possible\n";
		return;
	}

	int min_x = 0, max_x = 0;
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			for (int i=1; i<n; i++)
			{
				if (a[i].first < a[min_x].first)
					min_x = i;
			}
		}
		#pragma omp section
		{
			for (int i=1; i<n; i++)
			{
				if (a[i].first > a[max_x].first)
					max_x = i;
			}
		}
	}

    convex_a[min_x][PADDING] = true;
    convex_a[max_x][PADDING] = true;

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			quickHull(a, n, a[min_x], a[max_x], 1, convex_a, 1);
		}
		#pragma omp section
		{
			quickHull(a, n, a[min_x], a[max_x], -1, convex_a, 1);
		}
	}
}

int main()
{
	omp_set_num_threads(2);
	int lb = -23000;
    int ub = 23000;
    int n = 100000000;

	ifstream infile1("num1.txt", ios::in | ios::binary);
	ifstream infile2("num2.txt", ios::in | ios::binary);
    vector<int> a1(n);
	vector<int> a2(n);
    copy(istream_iterator<int>(infile1),
         istream_iterator<int>(),
         inserter(a1, a1.end()));
	copy(istream_iterator<int>(infile2),
         istream_iterator<int>(),
         inserter(a2, a2.end()));

	infile1.close();
	infile2.close();

	vector<IntegerPair> a(n);
    for(int i=0;i<n;i++){
        a[i].first = a1[i];
        a[i].second = a2[i];
    }
	a1.clear();
	a2.clear();
	vector<int>().swap(a1);
	vector<int>().swap(a2);

    vector<vector<bool>> convex_a (n);
    for(int i=0;i<n;i++)
    {
        vector<bool> b(PADDING+1, false);
        convex_a[i] = b;
    }
	driverCode(a, n, convex_a);
	return 0;
}