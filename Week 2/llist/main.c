#include <stdio.h>
#include "llist.h"

int main()
{
	int idx;	

	init();

	for(idx = 0; idx < 10; idx++)
	{
		add(idx);
	}		
	show();
	reverseShow();

	return 1;
}
