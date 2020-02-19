#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
	if(argc < 2) {

    		int fahr;

    		for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
			printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
	} else if(argc == 2) {
		printf("Farenheit:: %3d, Celcius: %6.1f\n", atoi(argv[1]), (5.0/9.0)*(atoi(argv[1])-32));
	} else {
		int f;
		int l = atoi(argv[1]);
		int u = atoi(argv[2]);
		int s = atoi(argv[3]);
		for(f = l; f <= u; f = f+s)
			printf("Fahrenheit: %3d, Celcius: %6.1f\n", f, (5.0/9.0)*(f-32));
	}
    return 0;
}
