#include "net.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL_net.h>

#define BROADCASTCLIENT 0

#define CLASS_CLIENT \
 int usable; \
 int id; \
 IPaddress address;

#define FALLTHROUGH(String) \
 if (!strcmp(String, "unknownhost") || !strcmp(string, "fallqueue")) { \
  return INVALIDQUEUE; \
 }

typedef struct {
 CLASS_CLIENT;
 
 queue_t queue;
} client_t;

typedef struct {
 CLASS_CLIENT;
 
 host_e type;
 
 UDPsocket socket;
 
 queue_t queues[MAX_QUEUES];
} host_t;

host_t host = { 0 };
client_t clients[MAX_CLIENTS + 1] = { 0 };

UDPpacket* transmission = { 0 };

extern void routepacket(packet_t* packet);

void copymessage(message_t*, message_t*);
char* reprintstring(const char* string);

#if 1 // NETWORK AND HOST

void openhost(int port) {
 if (host.usable) {
  LOGREPORT("Host already opened.");
  return;
 }
 
 host.socket = SDLNet_UDP_Open(port);
 
 SDLNULLPRONE(host.socket);
 
 SDLNEGATIVEPRONE(SDLNet_ResolveHost(&host.address, 0, port));
 
 transmission = SDLNet_AllocPacket(MAX_PACKETLENGTH);
 
 SDLNULLPRONE(transmission);
 
 transmission->address.host = host.address.host;
 transmission->address.port = host.address.port;
 
 SDLNet_UDP_Unbind(host.socket, 0);
 
 host.type = HOST_LOCAL;
 
 host.id = DEFAULTHOSTID;
 host.usable = 1;
 
 LOGREPORT("Opening [localhost] 127.0.0.1:%i.", port);
 
 return;
}

void joinhost(const char* address, int port, int local) {
 host.socket = SDLNet_UDP_Open(local);
 
 SDLNULLPRONE(host.socket);
 
 SDLNEGATIVEPRONE(SDLNet_ResolveHost(&host.address, address, port));
 
 transmission = SDLNet_AllocPacket(MAX_PACKETLENGTH);
 
 SDLNULLPRONE(transmission);
 
 transmission->address.host = host.address.host;
 transmission->address.port = host.address.port;
 
 SDLNEGATIVEPRONE(SDLNet_UDP_Bind(host.socket, 0, &host.address));
 
 host.type = HOST_CLIENT;
 
 host.id = 0;
 host.usable = 1;
 
 LOGREPORT("Connecting to %s:%i.", address, port);
 
 return;
}

void readyclient(int client) {
 clients[client].queue.queued = 1;
}

void appendclientmessage(message_t* message, int client) {
 queue_t* queue;
 
 if (client < -1) {
  LOGREPORT("Invalid client");
  return;
 }
 
 queue = &clients[client + 1].queue;
 
 if (queue->count > -1 && queue->count < MAX_MESSAGES) {
  copymessage(&queue->messages[queue->count], message);
  
  queue->count++;
 }
 else {
  LOGREPORT("Queue overflowed");
 }
}

int gethostID() {
 return host.id;
}

int countclients() {
 return 0;//client;
}

int hoststate() {
 return host.type;
}

void packpacket(packet_t* packet) {
 int msgbufferlength = packet->messagecount * sizeof(message_t);
 
 transmission->len = msgbufferlength + sizeof(int);
 
 ((int*) transmission->data)[0] = packet->messagecount;
 
 memcpy(transmission->data + sizeof(int), packet->messages, msgbufferlength);
 
 LOGREPORT("Packed packet: %s", transmission->data);
}

void sendhostpacket() {
 if (host.type == HOST_CLIENT) {
  transmission->address = host.address;
  
  SDLNet_UDP_Send(host.socket, -1, transmission);
 }
}

void sendclientpacket(int client) {
 if (host.type == HOST_LOCAL) {
  transmission->address = clients[client].address;
  
  SDLNet_UDP_Send(host.socket, -1, transmission);
 }
}

void printpacketcontent() {
 printf("Packet content;\n Channel: %i\n Length: %i\n Data: ", transmission->channel, transmission->len);
 
 for (int i = 0; i < transmission->len; i++) {
  printf("%02x", transmission->data[i]);
 }
 
 printf("\n String: %s", transmission->data);
 
 printf("\n");
}

void receivepackets(packet_t* packet) {
 if (SDLNet_UDP_Recv(host.socket, transmission)) {
  
  printpacketcontent();
  
  packet->messagecount = ((int*) transmission->data)[0];
  
  memcpy(packet->messages, transmission->data + sizeof(int), packet->messagecount * sizeof(message_t));
  
  routepacket(packet);
  //parsepacket(packet);
 }
}

#endif

#if 1 // QUEUE

void renewqueue(queue_t* queue, const char* string) {
 queue->label = reprintstring(string);
 queue->queued = 0;
 queue->count = 0;
 
 return;
}

int findqueue(const char* string) {
 for (int i = 0; i < MAX_QUEUES; i++) {
  if (host.queues[i].label && !strcmp(string, host.queues[i].label)) {
   return i;
  }
 }
 
 return INVALIDQUEUE;
}

int givequeue(const char* string) {
 int a, i;
 
 a = INVALIDQUEUE;
 
 FALLTHROUGH(string);
 
 for (i = 0; i < MAX_QUEUES; i++) {
  if (host.queues[i].label && !strcmp(string, host.queues[i].label)) {
   return i;
  }
  else if (!host.queues[i].label && a < 0) {
   a = i;
  }
 }
 
 renewqueue(&host.queues[i], string);
 
 return a;
}

void directmessage(message_t* message, int id, int target, int d1, int d2, int d3, int d4) {
 message->type = id;
 message->queue = target;
 //message->data[0] = d1;
 //message->data[1] = d2;
 //message->data[2] = d3;
 //message->data[3] = d4;
}

void copymessage(message_t* to, message_t* from) {
 to->type = from->type;
 to->queue = from->queue;
 //to->data[0] = from->data[0];
 //to->data[1] = from->data[1];
 //to->data[2] = from->data[2];
 //to->data[3] = from->data[3];
}

void appendmessage(message_t* message, int index) {
 queue_t* queue;
 
 if (index < 0) {
  LOGREPORT("Invalid queue");
  return;
 }
 
 queue = &host.queues[index];
 
 if (queue->count > -1 && queue->count < MAX_MESSAGES) {
  copymessage(&queue->messages[queue->count], message);
  
  queue->count++;
  queue->queued = 1;
 }
 else {
  LOGREPORT("Queue overflowed");
 }
}

void readyqueue(int queue) {
 host.queues[queue].queued = 1;
}

void handlequeues() {
 packet_t packet;
 
 if (host.socket || host.type != HOST_UNMADE) {
  receivepackets(&packet);
 }
}

void updatequeues() {
 packet_t packet;
 int i, j, k;
 
 if (host.type == HOST_CLIENT) {
  packet.messagecount = 0;
  
  for (i = 0; i < MAX_QUEUES; i++) {
   if (host.queues[i].queued) {
	for (j = 0; j < host.queues[i].count; j++) {
	 copymessage(&packet.messages[packet.messagecount + j], &host.queues[i].messages[j]);
	}
	
	packet.messagecount += host.queues[i].count;
	
	host.queues[i].count = 0;
	host.queues[i].queued = 0;
   }
  }
  
  if (packet.messagecount > 0) {
   // TODO: implement client packet division and message pooling
   packpacket(&packet);
   sendhostpacket();
  }
 }
 else if (host.type == HOST_LOCAL) {
  for (k = 0; k < clients[BROADCASTCLIENT].queue.count; k++) {
   copymessage(&packet.messages[k], &clients[BROADCASTCLIENT].queue.messages[k]);
  }
  
  for (i = 1; i < MAX_CLIENTS; i++) {
   if (clients[i].usable && clients[i].queue.queued) {
	packet.messagecount = k;
	
	for (j = 0; j < clients[i].queue.count; j++) {
	 copymessage(&packet.messages[k + j], &clients[i].queue.messages[j]);
	}
	
	packet.messagecount += clients[i].queue.count;
	
	clients[i].queue.count = 0;
	clients[i].queue.queued = 0;
	
	if (packet.messagecount > 0) {
	 // TODO: implement host packet division and message pooling
	 packpacket(&packet);
	 sendclientpacket(i);
	}
   }
  }
 }
}

#endif

char* reprintstring(const char* string) {
 char* copy;
 int length;
 
 if (!string) {
  return 0;
 }
 
 length = strlen(string) + 1;
 
 copy = (char*) malloc(length);
 
 memcpy(copy, string, length);
 
 return copy;
}
