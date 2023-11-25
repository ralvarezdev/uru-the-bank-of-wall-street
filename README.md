# The Bank of Wall Street
This repository contains the source code of the Bank Data Management Program, developed by:

## Developers
Ramón Álvarez <a href="https://github.com/ralvarezdev">(ralvarezdev)</a>.

Ronald López <a href="https://github.com/RoLpz07">(RoLpz07)</a>.

## Programming Languages
Only C++.

## Installation

<ol>
<li>Clone this repository to your local machine using the following command:

```git clone https://github.com/ralvarezdev/ProyectoLaboratorio_RR```
</li>

<li>Change your Current Working Directory to where the Repository was Cloned</li>

<li>There are Two Ways to Compile the Program:</li>

<ol>
<li>With <strong>G++</strong> (You must have Installed <strong>MinGW Compiler</strong>). Use the following command:

<details>
<summary>Command for Windows</summary>

```g++ -o bin\WallStreetBank.exe src\main.cpp src\lib\namespaces.h src\lib\clients\clientsOp.h src\lib\clients\clientsOp.cpp src\lib\clients\transactionsOp.cpp src\lib\clients\transactionsOp.h src\lib\data\dataOp.h src\lib\data\dataOp.cpp src\lib\datatables\output.h src\lib\datatables\output.cpp src\lib\terminal\ansiEsc.h src\lib\terminal\ansiEsc.cpp src\lib\terminal\input.cpp src\lib\terminal\input.h```

</details>

<details>
<summary>Command for Linux</summary>

```g++ -o bin/WallStreetBank.exe src/main.cpp src/lib/namespaces.h src/lib/clients/clientsOp.h src/lib/clients/clientsOp.cpp src/lib/clients/transactionsOp.cpp src/lib/clients/transactionsOp.h src/lib/data/dataOp.h src/lib/data/dataOp.cpp src/lib/datatables/output.h src/lib/datatables/output.cpp src/lib/terminal/ansiEsc.h src/lib/terminal/ansiEsc.cpp src/lib/terminal/input.cpp src/lib/terminal/input.h```

</details></li>

<li>With <strong>CMake</strong> (You must have Installed <strong>CMake and Make</strong>). First, Get to the <code>bin</code> Folder inside the Repository. Use the following command:

<details>
<summary>Command for Windows</summary>

```cmake -S ..\ -G "MinGW Makefiles" -B .\ && make```

</details>

<details>
<summary>Command for Linux</summary>

```cmake -S ../ -G "MinGW Makefiles" -B ./ && make```

</details></li></ol>

<li>Run the program by clicking the <code>WallStreetBank.exe</code>.</li>
</ol>

<strong>For a Better User Experience, Resize the Terminal Window to Fullscreen</strong>

# Commands
This is the Full List of Commands for Each Action in the Program

![Screenshot (843)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/93268cdb-84ed-42c2-8bf3-55ffa3efc55d)

![Screenshot (844)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/c61117d1-dab3-4993-a500-d2f856032ae5)

![Screenshot (863)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/386aec69-a9c0-49e1-ad53-1611f3423e8d)

# View Clients Command Examples

![Screenshot (730)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/32539e54-f008-40a4-8c13-1345234dbdb1)

# Filter Clients Command Examples

![Screenshot (752)](https://github.com/ralvarezdev/ProyectoLaboratorio_RR/assets/86166683/d71efa50-c463-4b19-9869-51ea30575f7e)
