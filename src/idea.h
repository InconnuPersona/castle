#pragma once

#define MAX_IDEAS 256

typedef enum {
 IDEA_MONARCHSTATE,
 IDEA_MILITARYSTATE,
 IDEA_FEUDALSTATE,
 
 IDEA_STATEARMY,
 
 IDEA_ARITHMETIC,
 IDEA_WRITING,
 
 IDEA_UNIT_ARCHER,
 
 IDEA_SECRET_MAGIC,

} idea_e;

typedef struct {
 int prerequisite;
 char title[32];
 char desc[32];
} ideaword_t;

extern ideaword_t ideawords[MAX_IDEAS];

void setupideas();

int getidea(const char* title);
