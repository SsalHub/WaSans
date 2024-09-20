#include "eventmanager.h"


void initEventListener()
{
	int i;
	for (i = 0; i < _EVENT_LEN_; i++)
	{
		eventListener[i].event = NULL;
		eventListener[i].args = NULL;
		eventListener[i].next = NULL;
	}
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
	int id = 0;
	while (curr)
	{
		id++;
		prev = curr;
		curr = curr->next;
	}
	// add node
	curr = (EVENT_LISTENER*)malloc(sizeof(EVENT_LISTENER));
	curr->event = e;
	curr->args = args;
	curr->next = NULL;
	prev->next = curr;
	return id;
}

EVENT_LISTENER* searchEventListener(EVENT_PTR e, void *args, EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next;
	while (curr)
	{
		if (curr->event == e && curr->args == args)
			break;
		curr = curr->next;
	}
	if (!curr)
		return NULL;
	return curr;
}



/* Terminate Func */
// if success, return 0. or return -1.
int removeEventListener(EVENT_PTR e, void *args, EVENT_TYPE et)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next, *prev = head;
	while (curr)
	{
		if (curr->event == e && curr->args == args)
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

// if success, return 0. or return -1.
int removeEventListenerAtIndex(EVENT_TYPE et, int index)
{
	EVENT_LISTENER *head = &(eventListener[et]), *curr = head->next, *prev = head;
	int i;
	for (i = 0; i < index; i++)
	{
		if (!curr)
			return -1;
		prev = curr;
		curr = curr->next;
	}
	if (i != index)
		return -1;
	prev->next = curr->next;
	free(curr);
	return 0;
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