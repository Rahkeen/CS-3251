#include <stdio.h>

int main()
{
	FILE *f;
	char output;

	f = fopen("test.txt", "r");
	if(!f)
	{
		printf("Error opening le file");
	}
	else
	{
	
		while(fscanf(f, "%c", &output) == 1)
		{
			printf("%c", output);
		}

		printf("%d", (int)sizeof(output));
	}
	fclose(f);
	return 0;
}
