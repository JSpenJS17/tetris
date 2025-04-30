/* Structs for global leaderboard storage */
struct ResponseBuffer {
    char *data;
    size_t size;
};

typedef struct TopScore {
	char name[64];
	int score;
	char reason[64];
	int floor;
} TopScore;

int placement = 0; /* The hero's placement against the world */
int num_topscores = 0; /* The number of the top 10 scores that are available */

TopScore* parse(char *str);
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);