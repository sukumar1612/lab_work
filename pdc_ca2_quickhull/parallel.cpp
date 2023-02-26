#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

#define IntegerPair pair<int, int>
#define PADDING 127
#define SEQUENTIAL_LIM 1000000

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
    if(n/depth <= SEQUENTIAL_LIM){
        quickHull(a, n, a[ind], p1, -findIfAboveOrBelow(a[ind], p1, p2), convex_a, depth+1);
	    quickHull(a, n, a[ind], p2, -findIfAboveOrBelow(a[ind], p2, p1), convex_a, depth+1);
        return;
    }
    #pragma omp parallel sections
    {
        #pragma omp section
        quickHull(a, n, a[ind], p1, -findIfAboveOrBelow(a[ind], p1, p2), convex_a, depth+1);
        #pragma omp section
	    quickHull(a, n, a[ind], p2, -findIfAboveOrBelow(a[ind], p2, p1), convex_a, depth+1);
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
	for (int i=1; i<n; i++)
	{
		if (a[i].first < a[min_x].first)
			min_x = i;
		if (a[i].first > a[max_x].first)
			max_x = i;
	}

    convex_a[min_x][PADDING] = true;
    convex_a[max_x][PADDING] = true;

	quickHull(a, n, a[min_x], a[max_x], 1, convex_a, 1);
	quickHull(a, n, a[min_x], a[max_x], -1, convex_a, 1);
}

int main()
{
    int lb = -20000;
    int ub = 20000;
    int n = 10000000;

	vector<IntegerPair> a(n);
    for(int i=0;i<n;i++){
        a[i].first = (rand() % (ub - lb + 1)) + lb;
        a[i].second = (rand() % (ub - lb + 1)) + lb;
    } 
    vector<vector<bool>> convex_a (n);
    for(int i=0;i<n;i++)
    {
        vector<bool> b(PADDING+1, false);
        convex_a[i] = b;
    }
	driverCode(a, n, convex_a);
	return 0;
}