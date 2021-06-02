#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>
#include <stack>
#include <chrono>
using namespace std;

typedef struct Vertice
{
    double xpos;
    double ypos;
}Vertice;

double getDist(Vertice* a, Vertice* b){
    int a_xpos=a->xpos; int a_ypos=a->ypos;
    int b_xpos=b->xpos; int b_ypos=b->ypos;

    return sqrt((a_xpos-b_xpos)*(a_xpos-b_xpos) + (a_ypos-b_ypos)*(a_ypos-b_ypos));
}

queue<int*> dynamicLoop(queue <int*>entireList,int* subList,int K,int index,int* stopover){
    if(index==K){
        int flag=1;
        for(int i=0;i<K;i++){
            for(int j=0;j<K;j++){
                if(subList[i] == subList[j] && i!=j){
                    flag=0;
                    break;
                }
            }
        }
        if(flag){
            entireList.push(subList);
        }
        return entireList;
    }
    
    for(int i=0;i<K;i++){
        int* subListSub=new int[K]();

        for(int j=0;j<index;j++){
            subListSub[j]=subList[j];
        }
        subListSub[index++]=stopover[i];
        entireList=dynamicLoop(entireList,subListSub,K,index,stopover);
        index-=1;
    }
    return entireList;
}

int main(int argc, char** argv){
    if( argc != 3 )
	{
		fprintf( stderr , "USAGE:  EXECUTABLE   INPUT_FILE_NAME   OUTPUT_FILE_NAME\n");
		return 1;
	}

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    fstream in(argv[1]);
    int N,M,K;
    in >> N; in >> M; in >> K;

    Vertice* verticeList=new Vertice[N+1]();
    for(int i=1;i<N+1;i++){
        double xpos; double ypos;
        in >> xpos; in >> ypos;
        verticeList[i]={xpos,ypos};
    }

    double** cost=new double*[N+1];
    for(int i=0;i<N+1;i++){
        cost[i]=new double[N+1];
        fill_n(cost[i],N+1,INFINITY);
        cost[i][i]=0;
    }

    for(int i=0;i<M;i++){
        int a; int b;
        in >> a; in >> b;
        Vertice* left=verticeList+a; Vertice* right=verticeList+b;
        double dist=getDist(left,right);
        cost[a][b]=dist;
        cost[b][a]=dist;
    }

    int* stopover=new int[K];
    for(int i=0;i<K;i++){
        int tmp; in>> tmp;
        stopover[i]=tmp;
    }


    int** trace=new int*[N+1];
    for(int i=0;i<N+1;i++){
        trace[i]=new int[N+1];
        for(int j=0;j<N+1;j++){
            trace[i][j]=j;
        }
    }
    

    for(int k=1;k<N+1;k++){
        for(int i=1;i<N+1;i++){
            for(int j=1;j<N+1;j++){
                if(cost[i][k] + cost[k][j] < cost[i][j]){
                    cost[i][j]=cost[i][k] + cost[k][j];
                    trace[i][j]=trace[i][k];
                }
            }
        }
    }



    int* stopOVerOrder=new int[3];
    

    queue<int*> entireList;
    int* sublist=new int[K];
    entireList=dynamicLoop(entireList,sublist,K,0,stopover);
    double min=0;
    int* target=new int[K];

    int* ftmp= new int[3];
    ftmp=entireList.front();
    entireList.pop();
    int tmptmp=1;
    for(int idx=0;idx<K;idx++){  
        min+=cost[tmptmp][ftmp[idx]];
        tmptmp=ftmp[idx];
    }
    min+=cost[tmptmp][N];

    while(!entireList.empty()){
        double minTemp=0;
        int* tmp= new int[K];
        tmp=entireList.front();
        entireList.pop();
        int tmpInt=1;
        for(int idx=0;idx<K;idx++){  
            minTemp+=cost[tmpInt][tmp[idx]];
            tmpInt=tmp[idx];
        }
        minTemp+=cost[tmpInt][N];

        if(minTemp < min){
            target=tmp;
            min=minTemp;
        }
    }
   /* for(int i=0;i<K;i++){
        printf("%d ",target[i]);
    }
    printf("%f\n",min);*/

    stack<int> s;
    queue<int> qq;
    s.push(10);
    for(int i=0;i<K;i++){
        s.push(target[K-i-1]);
    }
    s.push(1);
    int lastVer=s.top();
    s.pop();
    qq.push(lastVer);
    while(!s.empty()){
        int ver=s.top();
        s.pop();
        
        while(trace[lastVer][ver] != ver){
            lastVer=trace[lastVer][ver];
            qq.push(lastVer);
        }
        qq.push(ver);
        lastVer=ver;
    }   

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	std::chrono::milliseconds elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	double res_time = elapsed_time.count();


    FILE* out=fopen(argv[2],"w");
    fprintf(out,"%d\n",qq.size());
    printf("\n%d\n",qq.size());
    while(!qq.empty()){
        fprintf(out,"%d ",qq.front());
        printf("%d ",qq.front());
        qq.pop();
    }
    fprintf(out,"\n%d",(int)res_time); 
    fclose(out);
}
