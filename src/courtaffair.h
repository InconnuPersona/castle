#pragma once

//typedef enum {} directive_e;

typedef enum {
 TREATY_NONE,
} treaty_e;

typedef struct {
 treaty_e type;
 int target;
} treaty_t;
