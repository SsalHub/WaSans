#include "eventmanager.h"


/* Init Func */
void initEventListener()
{
	int i;
	for (i = 0; i < _EVENT_LEN_; i++)
	{
		eventListener[i].event = NULL;
		eventListener[i].args = NULL;
		eventListener[i].next = NULL;
	}
//	addEventListener(onStartGame, NULL, _EVENT_GAME_START_);
}

void onEvent(EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next;
	while (curr)
	{
		curr->event(curr->args);
		curr = curr->next;
	}
}

// if success, return 0.
int addEventListener(EVENT_PTR e, void *args, EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next, *prev = head;
	static int id = 0;
	while (curr)
	{
		prev = curr;
		curr = curr->next;
	}	// add node
	curr = (EVENT_LISTENER*)malloc(sizeof(EVENT_LISTENER));
	curr->event = e;
	curr->args = args;
	curr->id = id;
	curr->next = NULL;
	prev->next = curr;
	id++;
	return curr->id;
}

EVENT_LISTENER* searchEventListener(unsigned int id, EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next;
	while (curr)
	{
		if (curr->id == id)
			break;
		curr = curr->next;
	}
	if (!curr)
		return NULL;
	return curr;
}



/* Terminate Func */
// if success, return 0. or return -1.
int removeEventListener(unsigned int id, EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next, *prev = head;
	while (curr)
	{
		if (curr->id == id)
			break;
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return -1;
	prev->next = curr->next;
	free(curr);
	return 0;
}

// if success, return 0.
void flushEvent(EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next, *prev;
	while (curr)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	head->next = NULL;
}

void releaseEventListener()
{
	int i = 0;
	EVENT_LISTENER *head, *curr, *prev;
	for (i = 0; i < _EVENT_LEN_; i++)
	{
		head = &(eventListener[i]);
		curr = head->next;
		while (curr)
		{
			prev = curr;
			curr = curr->next;
			free(prev);
		}
	}
}