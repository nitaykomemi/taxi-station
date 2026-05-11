#ifndef TAXI_STATION_H
#define TAXI_STATION_H

#include <stdio.h>

typedef struct {
	int departure_time;
	int current_passengers;
} Ride;

typedef struct {
	int line_number;
	double rate;
	int trip_duration;			
	int max_capacity;			
	Ride hourly_rides[24];
	double total_earnings;
	int total_passengers;
} TaxiLine;

typedef struct {
	TaxiLine lines[5];
	int num_of_active_lines;
	double station_income;
} TaxiStation;

#define GET_HOUR(minutes) ((minutes) / 60)
#define GET_MIN(minutes) ((minutes) % 60)

void print_taxi_welcome();
void initstation(TaxiStation* mystation);
void registerpassenger(TaxiStation* mystation);
int check_availability(TaxiStation* mystation, int line_index, int h);
void processPayment(TaxiStation* mystation, int line_index, int h);
void updateStationData(TaxiStation* mystation, int line_index, int h, double rate);
void showSystemStatus(TaxiStation* mystation);
void showReports(TaxiStation* mystation);
void saveReportToFile(TaxiStation* mystation);

#endif

