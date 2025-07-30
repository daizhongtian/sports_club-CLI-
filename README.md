# Sports Club CLI

This repository contains a C++ command line application for managing a sports club.  It allows multiple user roles (athlete, coach and administrator) to register, log in and manage club data stored in CSV files.  A GoogleTest suite is included for unit testing the core classes.

## Features

- **User Authentication** – Credentials are stored in `credentials.txt` and support registering, logging in and deleting users.
- **Role‑based Menus**
  - Athletes can view their profile and register for events.
  - Coaches can manage events and teams and register members for events.
  - Administrators can list/add/delete members, coaches, teams and events and manage user accounts.
- **Club Management** – Classes for members, coaches, teams and events with persistence using CSV files.
- **Club Management** – Classes for members, coaches, teams and events.
- **Persistent CSV Storage** – All entities are stored in CSV files.
- **Event Applications** – Athletes may apply for events and coaches can approve or reject applications.
- **Unit Tests** – GoogleTest based tests covering authentication, CSV conversion and club operations.
- **Unique ID Assignment** – New members, coaches, teams and events receive unique identifiers.
- **Event Scheduling** – Conflict detection prevents overlapping dates.
- **Team Management** – Members can be added or removed and coaches assigned to teams.
- **Administration** – List and edit users, teams and events.
- **Event Rescheduling** – Existing events can be rescheduled.

## Building

The project was originally developed with Visual Studio (`Project1.sln`), but it can also be built with any C++17 compiler.  A simple build command using `g++` might look like:

```bash
g++ -std=c++17 Project1/*.cpp -o sports_club_cli
```

Running the program starts an interactive menu:

```bash
./sports_club_cli
```

Data will be saved in CSV files (`members.csv`, `coaches.csv`, `events.csv`, etc.) in the `Project1` directory.

## Running Tests

Unit tests reside under the `Google test` folder and use GoogleTest.  After installing the GoogleTest development libraries for your platform, compile the test executable:

```bash
g++ -std=c++17 Google\ test/Google_test.cpp Project1/*.cpp -lgtest -lgtest_main -pthread -o sports_club_tests
./sports_club_tests
```
