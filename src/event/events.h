#ifndef __EVENTS__
#define __EVENTS__
#include <stdio.h>
#include <stdlib.h>

#define _EVENT_LEN_		8


typedef void EVENT;
typedef void (*EVENT_PTR)(void*);

typedef enum EVENT_TYPE
{
	// init, scene events
	_EVENT_GAME_INIT_,
	_EVENT_SCENE_INIT_,
	_EVENT_NEXT_SCENE_,
	// ui events
	_EVENT_FADE_IN_,
	_EVENT_FADE_OUT_,
	_EVENT_DARK_EFFECT_,
	_EVENT_RED_EFFECT_,
	// battle events
	_EVENT_DAMAGE_SANS_DOT_,
} EVENT_TYPE;

typedef struct EVENT_LISTENER
{
	EVENT (*event)(void*);
	void* args;
	struct EVENT_LISTENER *next;
} EVENT_LISTENER;


static EVENT_LISTENER eventListener[_EVENT_LEN_];	// HEAD of SinglyLinkedList


void initEventListener();
void onEvent(EVENT_TYPE et);
int addEventListener(EVENT_PTR e, void *args, EVENT_TYPE et);
EVENT_LISTENER* searchEventListener(EVENT_PTR e, void *args, EVENT_TYPE et);
int removeEventListener(EVENT_PTR e, void *args, EVENT_TYPE et);
void releaseEventListener();
#endif