#ifndef __EVENTMANAGER__
#define __EVENTMANAGER__
#include <stdio.h>
#include <stdlib.h>
#include "loader/eventloader.h"

#define _EVENT_LEN_		6


typedef void (*EVENT_PTR)(void*);

typedef enum EVENT_TYPE
{
	// init, scene events
	_EVENT_GAME_START_,
	_EVENT_SCENE_START_,
	_EVENT_SCENE_UPDATE_,
	_EVENT_NEXT_SCENE_,
	
	// battle events
	_EVENT_ON_COLLISION_,
	_EVENT_ON_DAMAGED_,
	_EVENT_ON_HIT_,
} EVENT_TYPE;

typedef struct EVENT_LISTENER
{
	void (*event)(void*);
	void* args;
	unsigned int id;
	struct EVENT_LISTENER *next;
} EVENT_LISTENER;


static EVENT_LISTENER eventListener[_EVENT_LEN_];	// HEAD of SinglyLinkedList


/* Init Func */
void initEventListener();
void onEvent(EVENT_TYPE et);
int addEventListener(EVENT_PTR e, void *args, EVENT_TYPE et);
EVENT_LISTENER* searchEventListener(unsigned int id, EVENT_TYPE et);

/* Terminate Func */
int removeEventListener(unsigned int id, EVENT_TYPE et);
void flushEvent(EVENT_TYPE et);
void releaseEventListener();
#endif