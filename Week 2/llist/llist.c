#include "stdlib.h"
#include "stdio.h"
#include "llist.h"


static struct node *pHead = NULL;

void init()
{
	pHead = NULL;

}

/*
 * Aan de voorkant invoegen
 */
struct node* getNewNode(int data) {
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	if (NULL == newNode)
	{
		printf("Out of memory ...");
	}
	else
	{
		newNode->data = data;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	return newNode;
}
int add(int data)
{
	struct node *pn = getNewNode(data);
	if (pn == NULL)
	{
		return 0;
	}
	else
	{
		if (NULL == pHead)
		{
			pHead = pn;
			return 1;
		}
		pHead->previus = pn;
		pn->next = pHead;
		pHead = pn;
		
	}
	return 2;	
}

void show()
{
	struct node *p = pHead;
	int nr = 0;
	
	for( ; NULL !=  p->next ; p = p->next )
	{
		printf("node nr: %d heeft data [%d]\n",nr++,p->data);
	}
}
void reverseShow() {
	struct node* temp = head;
	if (temp == NULL) return;				
	while (temp->next != NULL) {
		temp = temp->next;
	}

	printf("tail-");
	while (temp != NULL) {
		printf("%d-", temp->data);
		temp = temp->previus;
	}
	printf("head");
	printf("\n");
}


