#include <stdio.h>
#include <io.h>

void main()
{
	// opdracht 1.3 en 1.4
	/*float fahr, celsius;
	int laagste, hoogste, interval;

	laagste = 0;
	hoogste = 300;
	interval = 20;

	fahr = laagste;
	printf("table far\n");
	while (fahr <= hoogste)
	{
		celsius = (5.0 / 9.0) * fahr - 32.0;
		printf("%3.0f %6.1f\n", fahr, celsius);
		fahr = fahr + interval;
	}
	printf("table cel\n");
	celsius = 0;
	while (celsius <= hoogste)
	{
		fahr = (9.0 / 5.0) * celsius + 32.0;
		printf("%3.0f %6.1f\n", celsius,fahr);
		celsius = celsius + interval;
	}*/

	// opdracht 1.6 en 1.7
	/*int c;
	while ((c = getchar()) != EOF)
	{
		putchar(c);
		printf("%d\n", c != EOF);

	}
	printf("%d\n", c != EOF);
	printf("%d\n", EOF);*/


	

}
// opdracht 5.3
void strCAT(char *s[], char *t[])
{
	char temp[10000] ={""};
	int f = (strlen(*s) + strlen(*t));
	for (int i = 0; i < strlen(*s); i++)
	{
		if (*s[i] == '\0')
		{
			for (int k = 0; k < strlen(*t); k++)
			{
				temp[i+k] = *t[k];
			}
			break;
		}
		else
		{
			temp[i] = *s[i];
		}

	}
	temp[f] = "\0";

}

// opdracht 5.4
int strEND(char *s[], char *t[])
{
	int temp = strlen(*t) - strlen(*s);
	for (int i = temp; i < strlen(*t); i++)
	{
		if (*t[i] != *s[i])
		{
			return 0;
		}
	}

	return 1 ;
}
