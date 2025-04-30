/* Structs for global leaderboard storage */
struct ResponseBuffer {
    char *data;
    size_t size;
};

typedef struct TopScore {
	char name[64];
	int score;
} TopScore;

extern int placement; /* The hero's placement against the world */
extern int num_topscores; /* The number of the top 10 scores that are available */

TopScore* get_scores(const char *name, int score);
TopScore* parse(char *str);
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);