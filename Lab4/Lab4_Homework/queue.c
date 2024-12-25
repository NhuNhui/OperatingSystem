
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t *q)
{
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t *q)
{
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t *de_queue(struct pqueue_t *q)
{
	struct pcb_t *proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition

	// YOUR CODE HERE

	pthread_mutex_lock(&q->lock);
	if (q->head != NULL)
	{
		// kiem tra neu burst_time = 0 thi xoa khoi link list
		if (q->head->data->burst_time <= 0)
		{
			struct qitem_t *del = q->head;
			struct pcb_t *first_item = q->head->data;
			// printf("NODE DELETE: %d\n", del->data->pid);
			free(first_item);
			del->data = NULL;
			q->head = q->head->next;
			free(del);
			if (q->head == NULL)
			{
				q->tail = NULL;
			}
		}
		if (q->head != NULL)
		{
			proc = q->head->data;
		}
	}
	pthread_mutex_unlock(&q->lock);
	return proc;
}

// func call to get and delete first node in inqueue
struct pcb_t *de_queue2(struct pqueue_t *q)
{
	struct pcb_t *proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition

	// YOUR CODE HERE

	pthread_mutex_lock(&q->lock);
	if (q->head != NULL)
	{
		proc = q->head->data;
		struct qitem_t *del = q->head;
		del->data = NULL;
		q->head = q->head->next;

		free(del);

		if (q->head == NULL)
		{
			q->tail = NULL;
		}
	}
	pthread_mutex_unlock(&q->lock);
	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t *q, struct pcb_t *proc)
{
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition

	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	struct qitem_t *newItem = (struct qitem_t *)(malloc(sizeof(struct qitem_t)));
	newItem->data = proc;
	newItem->next = NULL;
	// khong co phan tu nao
	if (q->head == NULL)
	{
		q->head = newItem;
		q->tail = newItem;
	}
	else
	{
		struct qitem_t *finder = q->head;
		// newItem co priority nho hon so voi head (head la nho nhat trong list)
		if (q->head->data->priority > newItem->data->priority)
		{
			newItem->next = q->head;
			q->head = newItem;
		}
		else if (q->head->data->priority == newItem->data->priority)
		{
			// newItem co priority = priority cua head nhung den som hon thi van o truoc head
			if (q->head->data->arrival_time > newItem->data->arrival_time)
			{
				newItem->next = q->head;
				q->head = newItem;
			}
			else
			{
				while (finder->next != NULL) // chi chay den tail
				{
					if (finder->data->priority <= newItem->data->priority)
					{
						// priority cua finder <= hon va priority cua finder->next < hon
						if (finder->next->data->priority > newItem->data->priority)
						{
							newItem->next = finder->next;
							finder->next = newItem;
							break;
						}
						else
						{
							////priority cua finder <= hon va priority cua finder->next =
							if (finder->next->data->priority == newItem->data->priority)
							{
								// check thoi gian den
								if (finder->next->data->arrival_time > newItem->data->arrival_time)
								{
									newItem->next = finder->next;
									finder->next = newItem;
								}
							}
						}
					}
					finder = finder->next;
				}
				// neu finer la tail
				if (finder->next == NULL)
				{
					q->tail->next = newItem;
					q->tail = newItem;
				}
			}
		}
		else
		{
			// neu newItem co priority lon hon head thi chay vong lap
			//  tim phan tu <= priority cua newItem
			while (finder->next != NULL) // chi chay den tail
			{
				if (finder->data->priority <= newItem->data->priority)
				{
					// priority cua finder <= hon va priority cua finder->next < hon
					if (finder->next->data->priority > newItem->data->priority)
					{
						newItem->next = finder->next;
						finder->next = newItem;
						break;
					}
					else
					{
						////priority cua finder <= hon va priority cua finder->next =
						if (finder->next->data->priority == newItem->data->priority)
						{
							// check thoi gian den
							if (finder->next->data->arrival_time > newItem->data->arrival_time)
							{
								newItem->next = finder->next;
								finder->next = newItem;
							}
						}
					}
				}
				finder = finder->next;
			}
			// neu finer la tail
			if (finder->next == NULL)
			{
				q->tail->next = newItem;
				q->tail = newItem;
			}
		}
	}
	pthread_mutex_unlock(&q->lock);
}

// ham them vao inqueue (chua can theo thu tu priority)
void en_queue2(struct pqueue_t *q, struct pcb_t *proc)
{
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition

	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	struct qitem_t *newItem = (struct qitem_t *)(malloc(sizeof(struct qitem_t)));
	newItem->data = proc;
	newItem->next = NULL;
	// khong co phan tu nao
	if (q->head == NULL)
	{
		q->head = q->tail = newItem;
	}
	else
	{
		// them vao cuoi cua link list
		q->tail->next = newItem;
		q->tail = newItem;
	}
	pthread_mutex_unlock(&q->lock);
}