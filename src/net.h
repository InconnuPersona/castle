#pragma once

#define MAX_QUEUES 16
#define MAX_PACKETLENGTH 2048
#define MAX_POOLLENGTH 128
#define MAX_CLIENTS 16
#define MAX_MESSAGES 128

#define DEFAULTHOSTID 1
#define INVALIDQUEUE -1 // messages with invalid queue targets are sent to the host's queue
#define BROADCAST -1

#define LOGREPORT(...) \
 { printf(__VA_ARGS__); printf("\n"); }

#define SDLNULLPRONE(Integer) \
 if (!Integer) { \
  LOGREPORT("SDL error - %s", SDL_GetError()); \
  exit(1); \
 }

#define SDLNEGATIVEPRONE(Integer) \
 if (Integer < 0) { \
  LOGREPORT("SDL error - %s", SDL_GetError()); \
  exit(1); \
 }

typedef enum {
 HOST_UNMADE = 0,
 HOST_CLIENT,
 HOST_LOCAL
} host_e;

typedef enum {
 NETMSG_ACKNOWLEDGE, // acknowledge packet delivery or existence
 NETMSG_REQUESTHOST,
 NETMSG_REVEALHOST,
 NETMSG_MIRRORQUEUES, // send queue message
 NETMSG_MESSAGE,
} netmessage_e;

typedef enum {
 MSG_ADDCLIENT,
 MSG_REMOVECLIENT,
 
 MSG_GAMESEND,
 MSG_PLAYERSEND,
 
 MSG_CHAT,
 MSG_SYNCHRONIZE,
 
 MSG_UPDATETILE,
 MSG_UPDATESTATE,
 MSG_UPDATEWORLD,
} message_e;

typedef unsigned char byte_t;

typedef struct {
 byte_t type;
 byte_t queue, length;
 
 union {
  byte_t bytes[4 * 4];
  short shorts[4 * 2];
  int integers[4];
 } data;
} message_t;

typedef struct {
 char* label;
 int queued; // boolean
 int count;
 message_t messages[MAX_MESSAGES];
} queue_t;

typedef struct {
 netmessage_e type;
 
 int length;
 int poollength;
 int pool[MAX_POOLLENGTH];
 int messagecount;
 message_t messages[MAX_MESSAGES];
} packet_t;

void joinhost(const char* address, int port, int local);
void openhost(int port);

void closehost();

int givequeue(const char* string);
int findqueue(const char* string);

void readyqueue(int queue);
void sharequeue(int queue, message_t* message);

int countclients();
void readyclient(int);
void appendclientmessage(message_t* message, int client);
int assignclient();

void appendmessage(message_t* message, int queue);

void handlequeues();
void updatequeues();

int hoststate();

int gethostID();
int getclientID(int client);
