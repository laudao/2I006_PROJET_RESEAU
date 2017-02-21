#include "Hachage.h"

int main()
{
	//int M;
	int i;
	int j;
	for(i=1;i<=10;i++){
		for(j=1;j<=10;j++){
			//printf("%d\n",fonctionClef(i,j));
			printf("%d\n", fonctionHachage(fonctionClef(i,j),100));
		}
	}
	return 0;
		
}
