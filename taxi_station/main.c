#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "taxi_station.h"

int main() {
	TaxiStation mystation;
	int choice = 0;

	print_taxi_welcome();
	initstation(&mystation);

	do {
		printf("\n=== TAXI STATION MENU ===\n");
		printf("1. Register for a ride (includes payment)\n");
		printf("2. Show system status (Real-time tracking)\n");
		printf("3. Show management reports (Daily summary)\n");
		printf("4. Exit system\n"); 
		printf("Please enter your choice (1-4): ");

		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("\n--- Registration ---\n");
			registerpassenger(&mystation);
			break;
		case 2:
			printf("\n--- System Status ---\n");
			showSystemStatus(&mystation);
			break;
		case 3:
			printf("\n--- Management Reports ---\n");
			showReports(&mystation);
			saveReportToFile(&mystation);
			break;
		case 4:
			printf("\nExiting the system. Have a great day!\n");
			saveReportToFile(&mystation);
			break;
		default:
			printf("\nInvalid choice! %d is not an option. Please try again.\n", choice);
		}

	} while (choice != 4); 
	return 0;
}

