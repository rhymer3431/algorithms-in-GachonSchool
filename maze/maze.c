#include <stdio.h>
#include <stdlib.h>

#define INF 9999
#define WALL 0
#define ROAD 1
#define START 2
#define END 3
typedef struct MAP{
    int **arr;
    int domain;
}Map;

typedef struct NODE{
    int index;
    int distance;
    int visited;
    int before;
}Node;
typedef struct POINT{
    int x;
    int y;
}Point;

typedef struct QUEUE{
    Node *arr;
    int *idxTable;
    int size;
    int max_size; 
}Queue;

int startIdx, endIdx;
Map *createMap(){
    FILE *fptr = fopen("../map.dat","r");
    if(fptr==NULL)exit(1);

    int domain = 0;
    fscanf(fptr,"%d",&domain);
    fgetc(fptr);
    printf("domain %d\n",domain);
    Map *newMap = (Map*)calloc(1,sizeof(Map));

    // initialize map;
    newMap->arr = (int**)calloc(domain,sizeof(int*));
    for (int i=0; i<domain; i++){
        newMap->arr[i] = (int*)calloc(domain,sizeof(int));
    }
    for (int i=0; i<domain; i++){
        for (int j=0; j<domain; j++){
            fscanf(fptr,"%d",&newMap->arr[i][j]);
        }
        
    }

    newMap->domain = domain;
    fclose(fptr);
    return newMap;
}
void freeMap(Map *m){
    for (int i=0; i<m->domain; i++){
        free(m->arr[i]);
    }
    free(m->arr);
    free(m);
}
Queue *createQueue(int domain){
    int size = domain*domain;
    Queue *newQueue = (Queue*)calloc(1,sizeof(Queue));

    newQueue->arr = (Node*)calloc(size,sizeof(Node));
    newQueue->idxTable = (int*)calloc(size,sizeof(int));
    newQueue->size = 0;
    newQueue->max_size = size;

    return newQueue;
}
void freeQueue(Queue *q){
    free(q->arr);
    free(q->idxTable);
    free(q);
}
Node createNode(int idx, int status){
    Node newNode;
    newNode.index = idx;
    newNode.visited = 0;
    newNode.before = -1;

    if(status == WALL){
        newNode.distance = -1;
    }
    else if(status == ROAD|| status == END){
        newNode.distance = INF;
    }

    if(status == START){
        printf("%d %d is start\n",idx%10, idx/10);
        startIdx = idx;
        newNode.distance = 0;
    }
    else if(status == END){
        endIdx = idx;
    }
    return newNode;
}
void swap(int a, int b, Node *arr, int *table){
    // change value in array
    Node temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
    // update table
    int idxA = arr[a].index;
    int idxB = arr[b].index;
    int tempIdx = table[idxA];
    table[idxA] = table[idxB];
    table[idxB] = tempIdx;
}

int getChild(int root){
    return (root*2)+1;
}
int getParent(int child){
    return (child-1)/2;
}


void relaxation(int target, Queue *q){
    int c = target;
    while(c>0){
        int root = getParent(c);
        if(root>=0 && (q->arr[c].distance < q->arr[root].distance||q->arr[root].distance == -1)){
            swap(root,c,q->arr,q->idxTable);
        }
        c = root;
    }
}
void enqueue(Node node, Queue *q){
    q->arr[q->size++] = node;
    q->idxTable[node.index] = q->size-1;
    int c=q->size - 1;
    relaxation(c,q);
}
Node *extract_min(Queue *q){
    swap(0,q->size-1,q->arr,q->idxTable);
    Node *ret = &(q->arr[q->size-1]);
    q->size -= 1;
    int root = 0;
    while(getChild(root) < q->size){
        int c = getChild(root);
        if(c+1 < q->size && q->arr[c].distance > q->arr[c+1].distance){
            c+=1;
        }
        if(q->arr[c].distance < q->arr[root].distance || q->arr[root].distance == -1){
            swap(root,c,q->arr,q->idxTable);
        }
        root = c;
    }
    
    return ret;
}

int convertP2I(int x, int y, int domain){
    return x + y*domain;
}
Point convertI2P(int idx, int domain){
    Point p;
    p.x = idx%domain;
    p.y = idx/domain;
    return p;
}

void initQueue(Queue *q, Map *m){
    int domain = m->domain;

    for (int y=0; y<domain; y++){
        for (int x=0; x<domain; x++){
            int status = m->arr[y][x];
            enqueue(createNode(convertP2I(x,y,domain),status),q);
        }
    }
}
void setKey(int idx, int key, Queue *q){
    int idx_in_queue = q->idxTable[idx];

    if(idx_in_queue >= q->size){
        // cannot access queue
        return;
    }
    q->arr[idx_in_queue].distance = key;
    relaxation(idx_in_queue,q);
}
void printMap(Map *m){
    int domain = m->domain;
    printf("\nprint map\n");
    for(int y=0; y<domain; y++){
        for (int x=0; x<domain; x++){
            switch(m->arr[y][x]){
                case WALL:printf("◼ ");break;
                case ROAD:printf("□ ");break;
                case START:printf("\033[1;31m■ \033[0m");break;
                case END:printf("\033[1;32m■ \033[0m");break;
                case 7:printf("\033[1;34m■ \033[0m");break;
            }
        }
        printf("\n");
    }
}
int checkCondition(Point src, Point dest, Map *m, Queue *q){

    int domain = m->domain;
    // x,y값이 0이상 도메인 값 미만
    if(!(src.x > -1 && src.y > -1 && src.x < domain && src.y < domain))return 0;
    if(!(dest.x > -1 && dest.y > -1 && dest.x < domain && dest.y < domain)){
        return 0;}
    

    
    int cond_lessKey = 0;

    int srcIdx = convertP2I(src.x,src.y,domain);
    int srcIdxInQ = q->idxTable[srcIdx];
    int destIdx = convertP2I(dest.x,dest.y,domain);
    int destIdxInQ = q->idxTable[destIdx];

    Node *srcNode = &q->arr[srcIdxInQ];
    Node *destNode = &q->arr[destIdxInQ];

    Point destPt = convertI2P(destIdx, domain);
    // is satisfied domain (Essential)
    if(destNode->visited){
        return 0;
    }
    // is there wall
    if(m->arr[destPt.y][destPt.x] == WALL){
        return 0;
    }

    // check if new path is more faster
    int newDistance = srcNode->distance + 1;

    if(newDistance < destNode->distance){
        cond_lessKey = 1;
    }

    if(cond_lessKey){
        destNode->before = srcIdx;
        return newDistance;
    }
    else return 0;

}

void visit(Node *target, Map *m, Queue *q){
    
    int domain = m->domain;
    Point src = convertI2P(target->index, domain);
    if(m->arr[src.y][src.x] == WALL){
        target->visited = 1;
        return;
    }

    // 동서남북 중 갈 수 있는 곳 확인
    int newDist = 0;
    // 동
    Point eastFromSrc;
    eastFromSrc.x = src.x+1;
    eastFromSrc.y = src.y;
    if((newDist = checkCondition(src,eastFromSrc,m,q))){
        
        int idxOfEast = convertP2I(eastFromSrc.x,eastFromSrc.y,domain);
        setKey(idxOfEast,newDist,q);
    }
    

    // 서
    Point westFromSrc;
    westFromSrc.x = src.x-1;
    westFromSrc.y = src.y;
    if((newDist = checkCondition(src,westFromSrc,m,q))){
        int idxOfWest = convertP2I(westFromSrc.x,westFromSrc.y,domain);
        setKey(idxOfWest,newDist,q);
    }


    // 남
    Point southFromSrc;
    southFromSrc.x = src.x;
    southFromSrc.y = src.y+1;
    if((newDist = checkCondition(src,southFromSrc,m,q))){

        int idxOfSouth = convertP2I(southFromSrc.x,southFromSrc.y,domain);
        setKey(idxOfSouth,newDist,q);
    }


    // 북
    Point northFromSrc;
    northFromSrc.x = src.x;
    northFromSrc.y = src.y-1;
    if((newDist = checkCondition(src,northFromSrc,m,q))){

        int idxOfNorth = convertP2I(northFromSrc.x,northFromSrc.y,domain);
        setKey(idxOfNorth,newDist,q);
    }

    target->visited = 1;
}
void printDistance(Queue *q, Map *m){
    for(int i=0; i<m->domain; i++){
        for (int j=0; j<m->domain; j++){
            Node target = q->arr[q->idxTable[convertP2I(j,i,m->domain)]];
            printf("%d ",target.distance);
        }
        printf("\n");
    }
}
void dijkstra(Map *m, Queue *q){
    while(q->size != 0){ // 큐가 빌 때까지
        Node *target = extract_min(q);
        visit(target,m,q);
    }

}
void backtracking(Map *m, Queue *q){
    Node start = q->arr[q->idxTable[startIdx]];
    Node end = q->arr[q->idxTable[endIdx]];
    Node cur = end;

    while(cur.index != start.index){
        Point temp = convertI2P(cur.index,m->domain);
        printf("visit %d %d\n",temp.x,temp.y);
        Point curPt = convertI2P(cur.index, m->domain);
        m->arr[curPt.y][curPt.x] = 7;
        printf("%d\n",q->arr[q->idxTable[cur.before]].index);
        cur = q->arr[q->idxTable[cur.before]];
        if(cur.index == q->arr[q->idxTable[cur.before]].index)break;

    }
    Point endPt = convertI2P(endIdx,m->domain);
    m->arr[endPt.y][endPt.x] = END;
}
void writeResult(Map *m){
    int domain = m->domain;
    FILE *fptr = fopen("../result.dat","w");
    fprintf(fptr,"%d\n",m->domain);
    for (int y=0; y<domain; y++){
        for (int x=0; x<domain; x++){
            fprintf(fptr,"%d",m->arr[y][x]);
            if(x<domain-1)fprintf(fptr," ");
            else if(y<domain-1)fprintf(fptr,"\n");
        }
    }
    fclose(fptr);
}
int main(void){
    Map *m = createMap();
    Queue *q = createQueue(m->domain);

    initQueue(q,m);
    dijkstra(m,q);
    backtracking(m,q);
    printDistance(q,m);
    writeResult(m);
    freeMap(m);
    freeQueue(q);
    
}



/*
    모든 노드 큐에 넣음 /ok
    시작 노드 거리 0으로 설정 /ok

    루트 노드 추출, 인접한 점들 방문
    기존 거리보다 현재 점 거리+1이 더 작으면 테이블 업데이트
*/