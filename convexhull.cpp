#include "iostream"
#include "vector"
#include "ctime"
#include "omp.h"
#include "math.h"
#include <algorithm>
// HELP taken FROM: https://www.geeksforgeeks.org/quickhull-algorithm-convex-hull/ for algorithm

using namespace std;


int whichSide(pair<int,int> p1,pair<int,int> p2, pair<int,int> c){
    int dist = (c.second - p1.second) * (p2.first - p1.first) -(p2.second - p1.second) * (c.first - p1.first);
    if (dist>0){
        return 1;
    } else if (dist<0){
        return -1;
    } else {
        return 0;
    }
}

int distance(pair<int,int> p1,pair<int,int> p2,pair<int,int> c){
    int dist = (c.second - p1.second) * (p2.first - p1.first) -(p2.second - p1.second) * (c.first - p1.first);
    return abs(dist);
}


vector<pair<int,int>> quickHull(vector<pair<int,int>> v,pair<int,int> min, pair<int,int> max,int side, int range,int num_threads){
    vector<pair<int,int>> s;
    int l = v.size();
    int maxdistance=0;
    
    int index=-1;
    for (int i=0;i<l;i++){
        int d=distance(min,max,v[i]);
        if (whichSide(min,max,v[i])==side && d > maxdistance){
            maxdistance=d;
            index=i;
        }
    }
    
    if (index==-1){
        s.push_back(min);
        s.push_back(max);
        return s;
    } else {
        vector<pair<int,int>> v1;
        vector<pair<int,int>> v2;

        if (range<num_threads){   
            #pragma omp parallel num_threads(2)
            {
                int id=omp_get_thread_num();
                if (id==0){
                    v1= quickHull(v,v[index],min,-whichSide(v[index],min, max),2*range,num_threads);   
                } 
                if (id==1){
                    v2= quickHull(v,v[index],min,-whichSide(v[index],max, min),2*range,num_threads);
                }
            }
        } else {
            v1=quickHull(v,v[index],min,-whichSide(v[index],min, max),range,num_threads);
            v2=quickHull(v,v[index],max,-whichSide(v[index],max, min),range,num_threads);
        }
        
        s.insert(s.end(),v1.begin(),v1.end());
        s.insert(s.end(),v2.begin(),v2.end());
        return s;
    }
}



vector<pair<int,int>> calcConvexHull ( vector<vector<int>> image, int num_threads){
    omp_set_nested(1);
    vector<pair<int,int>> points;
    int l=image.size();
    for (int i=0;i<l;i++){
        for (int j=0;j<int(image[0].size());j++){
            if (image[i][j]==1){
                points.push_back(make_pair(i,j));
            }
        }
    }
    int m = points.size();
    if (m<=2){
        vector<pair<int,int>> p;
        return p;
    } else {
    pair<int,int> min = points[0];
    pair<int,int> max = points[0];
    for (int i=0;i<m;i++){
        if (points[i].first<min.first){
            min=points[i];
        }
        if (points[i].first>max.first){
            max=points[i];
        }
    }


    vector<pair<int,int>> v1;
    vector<pair<int,int>> v2;
    vector<pair<int,int>> p;
    if (num_threads>=2){
        #pragma omp parallel num_threads(2)
        {
            int id=omp_get_thread_num();
            if (id==0){
                // cout<<"mon";
                v1=quickHull(points,min,max,1,2,num_threads);
            } 
             if (id==1){
                // cout<<"eon";
                v2=quickHull(points,min,max,-1,2,num_threads);
             }   
        }
       
    } else {
        v1=quickHull(points,min,max,1,2,num_threads);
        v2=quickHull(points,min,max,-1,2,num_threads);
    }

   
    p.insert(p.end(),v1.begin(),v1.end());
    p.insert(p.end(),v2.begin(),v2.end());
    sort( p.begin(), p.end() );
    p.erase( unique( p.begin(), p.end() ), p.end() );

    return p;
    }

}



