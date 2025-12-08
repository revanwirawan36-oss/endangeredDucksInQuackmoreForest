#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_ENTRIES 10 


struct ScoreEntry {
    char nama[21]; 
    int skor;
    int level; //0 easy 1 hard
};

void showLeaderboard();

void addScore(const char *nama, int skor, int level);


#endif 
