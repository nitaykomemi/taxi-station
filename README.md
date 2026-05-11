# Taxi Station Management System

A console-based taxi station management system written in C. Manage up to 5 taxi lines, register passengers, process payments, and generate daily revenue reports.

---

## Features

- **Multi-line support** — configure 1 to 5 independent taxi lines
- **Passenger registration** — check seat availability and book a ride by departure hour
- **Payment processing** — accepts any amount ≥ fare and prints change on the ticket
- **Real-time status** — enter the current time to see each line's status (waiting / in transit / arrived)
- **Daily reports** — summary of total passengers and revenue per line, saved to `station_report.txt`

---

## Project Structure

```
taxi_station/
├── main.c              # Entry point and menu loop
├── taxi_station.c      # All core logic
├── taxi_station.h      # Structs and function declarations
└── station_report.txt  # Auto-generated daily report (created at runtime)
taxi_station.sln        # Visual Studio solution file
```

---

## Data Model

```
TaxiStation
└── TaxiLine[5]
    ├── line_number
    ├── rate           (ticket price in NIS)
    ├── max_capacity   (3–5 passengers)
    ├── trip_duration  (minutes)
    └── Ride[24]       (one slot per hour of the day)
        ├── departure_time
        └── current_passengers
```

---

## Getting Started

### Requirements

- Windows with Visual Studio (or any C compiler, e.g. GCC/MinGW)

### Build & Run (Visual Studio)

1. Open `taxi_station.sln` in Visual Studio
2. Press **Ctrl+F5** to build and run

### Build & Run (GCC)

```bash
gcc taxi_station/main.c taxi_station/taxi_station.c -o taxi_station
./taxi_station
```

---

## Usage

On launch you configure the station:

```
How many active lines would you like to manage (1-5)? 2

Setting up Line 1:
Enter ticket price: 15.00
Enter capacity (3-5): 4
Enter trip duration (mins): 45
```

Then use the menu:

```
=== TAXI STATION MENU ===
1. Register for a ride (includes payment)
2. Show system status (Real-time tracking)
3. Show management reports (Daily summary)
4. Exit system
```

**Registering a passenger:**
- Select a line, enter a departure time (HH:MM)
- If a seat is available, confirm and pay
- A ticket is printed; change is returned if you overpay

**Sample ticket output:**
```
***********************************
* TAXI TICKET                     *
***********************************
* Line Number:    1               *
* Departure Time: 09:00           *
* Paid Amount:    20.00 NIS       *
* Change:         5.00  NIS       *
***********************************
* Have a safe trip!               *
***********************************
```

---

## Daily Report

Saved automatically to `station_report.txt` on exit or when viewing reports:

```
======= DAILY SUMMARY REPORT =======
Line       | Total Passengers | Total Revenue
----------------------------------------------------------
1          | 12               | 180.00 NIS
2          | 8                | 120.00 NIS
----------------------------------------------------------
TOTAL STATION REVENUE: 300.00 NIS
```

---

## License

This project is open source and available under the [MIT License](LICENSE).
