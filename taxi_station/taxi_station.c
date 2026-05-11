#define _CRT_SECURE_NO_WARNINGS
#include "taxi_station.h"

void print_taxi_welcome() {
	printf("\n");
	printf("        .-----------------.\n");
	printf("        |      TAXI       |\n");
	printf("     .--'-----------------'--.\n");
	printf("    /           [_]           \\\n");
	printf("   /      [#]         [#]      \\\n");
	printf("  |                             |\n");
	printf("  |     O - - - - - - - - O     |\n");
	printf("  |    / \\               / \\    |\n");
	printf("  '---'   '-------------'   '---'\n");
	printf("       \\_/               \\_/     \n");
	printf("\n");
	printf("  ===================================\n");
	printf("    TAXI STATION MANAGEMENT SYSTEM \n");
	printf("  ===================================\n");
	printf("\n");
}

void initstation(TaxiStation* mystation) {
	int active = 0;
	mystation->station_income = 0;

	printf("--- Taxi Station Setup ---\n");

	do {
		printf("How many active lines would you like to manage (1-5)? ");
		scanf("%d", &active);
		if (active < 1 || active > 5) {
			printf("Error: Station supports between 1 to 5 lines only.\n");
		}
	} while (active < 1 || active > 5);

	mystation->num_of_active_lines = active;

	for (int i = 0; i < mystation->num_of_active_lines; i++) {
		mystation->lines[i].line_number = i + 1;

		printf("\nSetting up Line %d:\n", i + 1);
		printf("Enter ticket price: ");
		scanf("%lf", &mystation->lines[i].rate);

		do {
			printf("Enter capacity (3-5): ");
			scanf("%d", &mystation->lines[i].max_capacity);
		} while (mystation->lines[i].max_capacity < 3 || mystation->lines[i].max_capacity > 5);

		printf("Enter trip duration (mins): ");
		scanf("%d", &mystation->lines[i].trip_duration);

		printf("\nReset counters for the line\n ");
		mystation->lines[i].total_earnings = 0;
		mystation->lines[i].total_passengers = 0;

		printf("\nAutomatic initialization of the schedule no need to receive from the user because it is fixed every hour\n");
		for (int h = 0; h < 24; h++) {
			mystation->lines[i].hourly_rides[h].current_passengers = 0;
			mystation->lines[i].hourly_rides[h].departure_time = h * 60;
		}
	}
}

void registerpassenger(TaxiStation* mystation) {
	int i = 0, h = 0, m = 0, approval = 0;

	printf("\n--- Passenger Registration ---\n");
	printf("Available lines: 1 to %d\n", mystation->num_of_active_lines);
	printf("Please select a line: ");
	scanf("%d", &i);

	if (i >= 1 && i <= mystation->num_of_active_lines) {
		int line_index = i - 1;

		printf("Please enter your desired departure time (HH:MM): ");
		scanf("%d:%d", &h, &m);

		int requested_time_mins = (h * 60) + m;

		if (h >= 0 && h <= 23 && m >= 0 && m <= 59) {

			if (check_availability(mystation, line_index, h)) {
				printf("\nFound a spot on Line %d for the %02d:00 departure.\n", i, h);
				printf("Ticket Price: %.2f NIS\n", mystation->lines[line_index].rate);

				printf("To confirm and pay, enter 1: ");
				scanf("%d", &approval);

				if (approval == 1) {
					processPayment(mystation, line_index, h);
				}
			}
		}
		else {
			printf("Invalid time format. Please use HH:MM (00:00 - 23:59).\n");
		}
	}
	else {
		printf("Error: Line %d is not active.\n", i);
	}
}

int check_availability(TaxiStation* mystation, int line_index, int h) {

	int current_passengers = mystation->lines[line_index].hourly_rides[h].current_passengers;
	int max_capacity = mystation->lines[line_index].max_capacity;

	if (current_passengers < max_capacity) {
		printf("\n>>> Line %d is AVAILABLE at %02d:00.\n",
			mystation->lines[line_index].line_number, h);
		printf(">>> Seats remaining: %d out of %d\n",
			max_capacity - current_passengers, max_capacity);
		return 1;
	}
	else {
		printf("\nSorry, the taxi for Line %d at %02d:00 is FULL (%d/%d).\n",
			mystation->lines[line_index].line_number, h, current_passengers, max_capacity);
		return 0;
	}
}

void processPayment(TaxiStation* mystation, int line_index, int h) {
	double payment = 0.0;
	double ticket_price = mystation->lines[line_index].rate;
	int departure_mins = mystation->lines[line_index].hourly_rides[h].departure_time;

	printf("\n--- Payment Process ---\n");
	printf("The price per ticket is %.2f NIS. Please enter the amount to pay: ", ticket_price);
	scanf("%lf", &payment);

	if (payment >= ticket_price) {
		double change = payment - ticket_price;

		printf("\nPayment Confirmed! Printing your ticket...\n\n");

		printf("***********************************\n");
		printf("* TAXI TICKET            *\n");
		printf("***********************************\n");
		printf("* Line Number:    %-15d *\n", mystation->lines[line_index].line_number);
		printf("* Departure Time: %02d:%02d           *\n", GET_HOUR(departure_mins), GET_MIN(departure_mins));
		printf("* Paid Amount:    %-10.2f NIS  *\n", payment);
		if (change > 0.0)
			printf("* Change:         %-10.2f NIS  *\n", change);
		printf("***********************************\n");
		printf("* Have a safe trip!         *\n");
		printf("***********************************\n\n");

		updateStationData(mystation, line_index, h, ticket_price);
	}
	else {
		printf("\nError: Insufficient payment. %.2f NIS required, you paid %.2f NIS.\n",
			ticket_price, payment);
		printf("Transaction cancelled. Please try again.\n");
	}
}

void updateStationData(TaxiStation* mystation, int line_index, int h, double rate) {
	mystation->lines[line_index].hourly_rides[h].current_passengers++;
	mystation->lines[line_index].total_earnings += rate;
	mystation->lines[line_index].total_passengers++;
	mystation->station_income += rate;
}

void showSystemStatus(TaxiStation* mystation) {
	int h = 0, m = 0, current_p = 0, departure = 0, arrival = 0;
	double ride_earnings = 0.0;

	printf("Please enter the current time (HH:MM): ");
	scanf("%d:%d", &h, &m);
	int now_time = (h * 60) + m;

	if (h >= 0 && h <= 23 && m >= 0 && m <= 59) {
		printf("\nStatus Report for %02d:%02d:\n", h, m);
		printf("--------------------------------------------------\n");

		for (int i = 0; i < mystation->num_of_active_lines; i++) {

			current_p = mystation->lines[i].hourly_rides[h].current_passengers;
			ride_earnings = current_p * mystation->lines[i].rate;

			printf("Line %d | Passengers: %d | Earnings: %.2f NIS \n",
				mystation->lines[i].line_number, current_p, ride_earnings);

			departure = mystation->lines[i].hourly_rides[h].departure_time;
			arrival = departure + mystation->lines[i].trip_duration;

			if (now_time < arrival && now_time >= departure) {
				int minutes_on_road = now_time - departure;
				printf("Status: IN TRANSIT (%d min in) | ETA: %02d:%02d\n",
					minutes_on_road, GET_HOUR(arrival), GET_MIN(arrival));
			}
			else if (now_time < departure) {
				printf("Status: WAITING at station (Departure at %02d:00)\n", h);
			}
			else {
				printf("Status: AT DESTINATION (Arrived at %02d:%02d)\n",
					GET_HOUR(arrival), GET_MIN(arrival));
			}
			printf("--------------------------------------------------\n");
		}
	}
	else {
		printf("Invalid time format.\n");
	}
}

void showReports(TaxiStation* mystation) {
	printf("\n======= DAILY SUMMARY REPORT =======\n");
	printf("%-10s | %-16s | %-15s\n", "Line", "Total Passengers", "Total Revenue");
	printf("----------------------------------------------------------\n");

	for (int i = 0; i < mystation->num_of_active_lines; i++) {
		int line_num = mystation->lines[i].line_number;
		int total_p = mystation->lines[i].total_passengers;
		double total_e = mystation->lines[i].total_earnings;

		printf("%-10d | %-16d | %-15.2f NIS\n", line_num, total_p, total_e);
	}

	printf("----------------------------------------------------------\n");
	printf("TOTAL STATION REVENUE: %.2f NIS\n", mystation->station_income);
	printf("==========================================================\n");
}

void saveReportToFile(TaxiStation* mystation) {
	FILE* file = fopen("station_report.txt", "w");

	if (file == NULL) {
		printf("Error: Could not create report file!\n");
		return;
	}

	fprintf(file, "======= DAILY SUMMARY REPORT =======\n");
	fprintf(file, "%-10s | %-16s | %-15s\n", "Line", "Total Passengers", "Total Revenue");
	fprintf(file, "----------------------------------------------------------\n");

	for (int i = 0; i < mystation->num_of_active_lines; i++) {
		fprintf(file, "%-10d | %-16d | %-15.2f NIS\n",
			mystation->lines[i].line_number,
			mystation->lines[i].total_passengers,
			mystation->lines[i].total_earnings);
	}

	fprintf(file, "----------------------------------------------------------\n");
	fprintf(file, "TOTAL STATION REVENUE: %.2f NIS\n", mystation->station_income);
	fprintf(file, "==========================================================\n");

	fclose(file);

	printf("\n[!] Report has been saved successfully to 'station_report.txt'\n");
}