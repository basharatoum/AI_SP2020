#include<iostream>
#include<stdio.h>
#include<string.h>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

struct node{
    string name; 
    int id;
    vector< pair < int , double > > next;
    bool visited ;
    double cost;
    double h;
    int parent;
    node(){
      cost = 0;
      visited = 0;
    }
};
bool operator<(struct node a,struct node b){return a.cost>b.cost?true:false;}

void traceRoute(vector<struct node> *listOfNodes,struct node finish){
    struct node cur=finish;
    int parent;
    double distance;
    vector<string> path;
    char c[10];
    while(cur.parent!=-1){
        parent = (*listOfNodes)[cur.id].parent;
        for (int i =0;i<cur.next.size();++i){
            if(cur.next[i].first==parent){
                distance = cur.next[i].second;
            }
        }
        sprintf(c,"%.1f km",distance);
        path.push_back((*listOfNodes)[parent].name+" to "+cur.name+", "+c);
        cur = (*listOfNodes)[parent];
    }
    for (int i = path.size()-1;i>=0;i--){
        cout<<path[i]<<endl;
    }
}

void findPathInformed(vector<struct node> *listOfNodes,struct node src,struct node finish){
    int nodesExpanded=0,nodesGenerated=1,maxNodesInMemory=1,currentNodes=1;
    double distance=-1;
    priority_queue<struct node> q;
    q.push(src);

    while (!q.empty())
    {
        src = q.top();
        q.pop();
        // mark this node visited
        if(currentNodes>maxNodesInMemory){
            maxNodesInMemory=currentNodes;
        }
        currentNodes-=1;
        if(src.parent!=-1){
            (*listOfNodes)[src.id].cost-=(*listOfNodes)[src.id].h;
            src.cost-= (*listOfNodes)[src.id].h; 
        }
        (*listOfNodes)[src.id].visited =1;
        nodesExpanded+=1;
        if(src.name.compare(finish.name)==0){
            distance = src.cost;
            cout<<"nodes expanded: "<<nodesExpanded<<endl;
            cout<<"nodes generated: "<<nodesGenerated<<endl;
            cout<<"max nodes in memory: "<<maxNodesInMemory<<endl;
            cout<<"distance: "<<distance<<endl;
            cout<<"route:"<<endl;
            traceRoute(listOfNodes,finish);
            return;
        }

        pair < int , double > temp;
        for (int i=0;i<src.next.size();++i){
            temp = src.next[i];
            if((*listOfNodes)[temp.first].visited==0){
                (*listOfNodes)[temp.first].cost=src.cost+temp.second+(*listOfNodes)[temp.first].h;
                (*listOfNodes)[temp.first].visited =1;
                (*listOfNodes)[temp.first].parent= src.id;
                q.push((*listOfNodes)[temp.first]);
                nodesGenerated+=1;
                currentNodes+=1;
            }
        }
        if(currentNodes>maxNodesInMemory){
            maxNodesInMemory=currentNodes;
        }

    }
    cout<<"nodes expanded: "<<nodesExpanded<<endl;
    cout<<"nodes generated: "<<nodesGenerated<<endl;
    cout<<"max nodes in memory: "<<maxNodesInMemory<<endl;
    cout<<"distance: infinity"<<endl;
    cout<<"route:"<<endl<<"none"<<endl;
    return;
}

void findPathUninformed(vector<struct node> *listOfNodes,struct node src,struct node finish){
    int nodesExpanded=0,nodesGenerated=0,maxNodesInMemory=1,currentNodes=1;
    double distance=-1;
    priority_queue<struct node> q;
    q.push(src);
    while (!q.empty())
    {
        src = q.top();
        q.pop();
        // mark this node visited
        if(currentNodes>maxNodesInMemory){
            maxNodesInMemory=currentNodes;
        }
        currentNodes-=1;

        (*listOfNodes)[src.id].visited =1;
        nodesExpanded+=1;
        if(src.name.compare(finish.name)==0){
            distance = src.cost;
            cout<<"nodes expanded: "<<nodesExpanded<<endl;
            cout<<"nodes generated: "<<nodesGenerated<<endl;
            cout<<"max nodes in memory: "<<maxNodesInMemory<<endl;
            cout<<"distance: "<<distance<<endl;
            cout<<"route:"<<endl;
            traceRoute(listOfNodes,finish);
            return;
        }

        pair<int,double> temp;
        for (int i=0;i<src.next.size();++i){
            temp = src.next[i];
            if((*listOfNodes)[temp.first].visited==0){
                (*listOfNodes)[temp.first].cost=src.cost+temp.second;
                (*listOfNodes)[temp.first].visited =1;
                (*listOfNodes)[temp.first].parent= src.id;
                q.push((*listOfNodes)[temp.first]);
                nodesGenerated+=1;
                currentNodes+=1;
            }
        }
        if(currentNodes>maxNodesInMemory){
            maxNodesInMemory=currentNodes;
        }

    }
    cout<<"nodes expanded: "<<nodesExpanded<<endl;
    cout<<"nodes generated: "<<nodesGenerated<<endl;
    cout<<"max nodes in memory: "<<maxNodesInMemory<<endl;
    cout<<"distance: infinity"<<endl;
    cout<<"route:"<<endl<<"none"<<endl;
    return;
}

int main(int argc,char* argv[]){
    vector<struct node> listOfNodes;
    bool city1_exists =0,city2_exists = 0;
    int i;
    if (argc>=4){
        // uninformed search - uses uniform cost (modified with a visited array)

        // assumes largest city name consists of 50 characters
        string city1,city2,tempEdge;
        int idCity1 =0,idCity2=0;
        double edgeDistance;
        ifstream input;
        input.open(argv[1]);
        input>>city1>>city2>>tempEdge;
        
        // Create the graph
        while (city1.compare("END")!=0 && city2.compare("OF")!=0 &&tempEdge.compare("INPUT")!=0)
        {
	    edgeDistance = atof(tempEdge.c_str());
            city1_exists = 0;
            city2_exists = 0;
            for(i=0;i<listOfNodes.size();++i){
                if(listOfNodes[i].name.compare(city1)==0){
                    city1_exists = 1;
                    idCity1=i;
                }else if(listOfNodes[i].name.compare(city2)==0){
                    city2_exists = 1;
                    idCity2 = i;
                }
            }

            if(city1_exists==0){
                struct node *tempCity1 = new struct node();
                tempCity1->name = city1;
                tempCity1->id = listOfNodes.size();
                idCity1 = tempCity1->id;
                listOfNodes.push_back(*tempCity1);
            }

            if(city2_exists==0){
                struct node *tempCity2 = new struct node();
                tempCity2->name = city2;
                tempCity2->id = listOfNodes.size();
                idCity2 = tempCity2->id;
                listOfNodes.push_back(*tempCity2);
            }

            listOfNodes[idCity1].next.push_back(make_pair(idCity2,edgeDistance));
            listOfNodes[idCity2].next.push_back(make_pair(idCity1,edgeDistance));
            input>>city1>>city2>>tempEdge;
        }
        input.close();
        for(i=0;i<listOfNodes.size();++i){
            if(listOfNodes[i].name.compare(argv[2])==0){
                city1_exists = 1;
                idCity1=i;
            }else if(listOfNodes[i].name.compare(argv[3])==0){
                city2_exists = 1;
                idCity2 = i;
            }
        }
        if (city2_exists==0||city1_exists==0){
            cout<<"INPUT ERROR, one or both of the cities do not exist"<<endl;
            return 0;
        }
        listOfNodes[idCity1].parent=-1;
        if(argc ==5){
            input.open(argv[4]);
            string h;
            input>>city1>>h;
            while (city1.compare("END")!=0&&h.compare("OF")!=0)
            {
                for(i=0;i<listOfNodes.size();++i){
                    if(listOfNodes[i].name.compare(city1)==0){
		        listOfNodes[i].h = atof(h.c_str());
                        break;
                    }
                }
                input>>city1>>h;
            }
            
            findPathInformed(&listOfNodes,listOfNodes[idCity1],listOfNodes[idCity2]);
        }else{
            findPathUninformed(&listOfNodes,listOfNodes[idCity1],listOfNodes[idCity2]);
        }
    }

 return 0;
}


