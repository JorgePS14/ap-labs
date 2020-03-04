#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, char **pmonth, int *pday){
	int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if(year % 4 == 0)
		months[1] = 29;

	if(year % 4 != 0 && yearday == 366)
		return;

	int month;
	for(month = 0; months[month]; month++){
		if(yearday > months[month])
			yearday = yearday - months[month];
		else
			break;
	}

	*pmonth = monthNames[month];
	*pday = yearday;
}

int main(int argc, char* argv[]) {
	if(argc == 3) {
		int year = atoi(argv[1]);
		int yearday = atoi(argv[2]);
		char* pmonth = "no";
		int pday = 0;
		if(yearday > 1 && yearday < 367)
			month_day(year, yearday, &pmonth, &pday);
		if(pmonth != "no" && pday != 0)
			printf("Month: %s, day: %d, year: %d\n", pmonth, pday, year);
		else
			printf("Error: Yearday exceeded the number of days in that year\n");	
	} else {
		printf("Please check your input. Format: ./month_day <year> <yearday>\n");
	}
	return 0;
}
