# The Bank of Wall Street
This repository contains the source code of the Bank Data Management Program, developed by:

## Developers
Ramón Álvarez <a href="https://github.com/ralvarezdev">(ralvarezdev)</a>.

Ronald López <a href="https://github.com/RoLpz07">(RoLpz07)</a>.

## Programming Languages
Only C++.

## Installation

1. Clone this repository to your local machine using the following command: ```git clone https://github.com/ralvarezdev/ProyectoLaboratorio_RR```

2. Change your Current Working Directory to where the Repository was Cloned

3. There are Two Ways to Compile the Program: 

3.a. With *G++* (You must have Installed **MinGW Compiler**). Use the following command:

```g++ -o bin\WallStreetBank.exe src\main.cpp src\lib\namespaces.h src\lib\clients\clientsOp.h src\lib\clients\clientsOp.cpp src\lib\clients\transactionsOp.cpp src\lib\clients\transactionsOp.h src\lib\data\dataOp.h src\lib\data\dataOp.cpp src\lib\datatables\output.h src\lib\datatables\output.cpp src\lib\terminal\ansiEsc.h src\lib\terminal\ansiEsc.cpp src\lib\terminal\input.cpp src\lib\terminal\input.h``` .

3.b. With *CMake* (You must have Installed **CMake** and **Make**). First, Get to the ```bin``` Folder inside the Repository. Use the following command:

```cmake -S ..\ -G "MinGW Makefiles" -B .\ && make```

4. Run the program by clicking the ```WallStreetBank.exe```. 

*For a Better User Experience, Resize the Terminal Window to Fullscreen*

# Commands
This is the Full List of Commands for Each Action in the Program

![Screenshot (727)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/64c3d531-83a5-41cb-ae0d-61acc8b32571)
![Screenshot (728)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/baf61880-6bd1-4976-b485-82f785b9bb67)
![Screenshot (729)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/1d5af61f-b876-44c2-8c61-facf7163da1c)
# View Clients Command Examples
![Screenshot (730)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/32539e54-f008-40a4-8c13-1345234dbdb1)
# Filter Clients Command Examples
![Screenshot (752)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/d71efa50-c463-4b19-9869-51ea30575f7e)
