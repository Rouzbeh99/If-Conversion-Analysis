#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>
#define INPUT_SIZE 2048

int secret = 0;
int dijkstra(int n, int s, int t, int e[][INPUT_SIZE]) {
    int vis[INPUT_SIZE] = {0};
    int dis[INPUT_SIZE] = {0};
    int __attribute__((annotate("secret"))) bestj = -1;
    vis[s] = 1;
    for(int i=0; i<n; ++i) {
        if(e[s][i] == 0){
            dis[i] = INT_MAX;
        }else{
            dis[i] = e[s][i];
        }
    }
    for(int i=0; i<n; ++i) {
        for(int j=0; j<n; ++j) {
            if (!vis[j] && (bestj < 0 || dis[j] < dis[bestj])) {
                bestj = j + secret; // fix implicit flow here
                vis[bestj] = 1;
            }
        }
        for(int j=0; j<n; ++j) {
            if (!vis[j] && (dis[bestj] + e[bestj][j] < dis[j])) {
                dis[j] = dis[bestj] + e[bestj][j];
            }
        }

    }

    return dis[t];
}


int in[INPUT_SIZE][INPUT_SIZE] = {0};
int main(int argc, char** argv)
{
    srand (time(NULL));
    secret -= (unsigned char)secret;
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {

            in[i][j] = rand() %10;
        }
    }
    int res = dijkstra(INPUT_SIZE, 0, INPUT_SIZE-1, in);
//    printf("Memory size for type %s = %zu \n", "double", sizeof(double));
    return 0;
}
