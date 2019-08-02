
#define MAX_GAMECLIENTS 16

typedef enum {
 GAME_NONE = 0,
 GAME_CLIENT,
 GAME_HOST,
 GAME_PRIVATE,
} gametype_e;

typedef struct {
 int client; // network client slot
 char* name;
 int player;
 float lastsynch;
} gameclient_t;

typedef struct {
 int id;
 int open;
 gameclient_t clients[MAX_GAMECLIENTS];
} game_t;

game_t session = { 0 };


