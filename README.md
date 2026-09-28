NeoVerse - Visual Studio 2022 Setup Guide
This project is a Windows console application built in C++17. It is designed to run in a normal terminal window and can be launched either from Visual Studio 2022 or by double-clicking the generated executable.

1. Project folder
Open this folder in Windows Explorer or in Visual Studio:

C:\Users\trist\Downloads\Prog622B Assignment\PROG622B-Assignment\NeoVerse

The project contains:

src/
include/
data/
README.md
NeoVerse.exe
2. Requirements
Install the following on your Windows PC:

Visual Studio 2022
Desktop development with C++ workload
Windows 10 or Windows 11
No extra external libraries are required for this project.

3. Run the project in Visual Studio 2022
Method A: Open the folder directly
Open Visual Studio 2022.
Click File > Open > Folder.
Select the project folder: C:\Users\trist\Downloads\Prog622B Assignment\PROG622B-Assignment\NeoVerse
Visual Studio will load the folder structure.
Open src/main.cpp.
Press Ctrl + F5 to run the application without debugging.
A console window will open and the program will start.
Method B: Create a new C++ project manually
Open Visual Studio 2022.
Click Create a new project.
Select Empty Project.
Name it NeoVerse.
Choose a location such as: C:\Users\trist\Downloads\Prog622B Assignment\PROG622B-Assignment\NeoVerse
Click Create.
Right-click Source Files > Add > Existing Item.
Add all .cpp files from src/.
Right-click Header Files > Add > Existing Item.
Add all .h files from include/.
Right-click the project in Solution Explorer > Properties.
Go to C/C++ > Language.
Set C++ Language Standard to C++17.
Build > Build Solution.
Press Ctrl + F5 to run it.
4. Run the standalone .exe file
A standalone Windows executable is already generated in the project folder:

NeoVerse.exe

To run it:

Open File Explorer.
Go to: C:\Users\trist\Downloads\Prog622B Assignment\PROG622B-Assignment\NeoVerse
Double-click NeoVerse.exe.
A terminal window will open and the program will start.
This works without opening Visual Studio.

5. Default login credentials
The program seeds default accounts if the engineer file is missing or empty.

Username: admin
Password: admin123
Additional default accounts:

Username: jsmith, Password: pass123
Username: tnandi, Password: city2035
6. Important file location note
The app expects to find the data files in a folder named data next to the executable.

Keep the executable in the same folder as:

data/engineers.dat
data/sensors.dat
data/city_logs.dat
data/events.dat
data/config.txt
If these files are missing, the application may recreate default data on first run.

7. Run it from PowerShell manually
You can also start it directly from PowerShell:

cd "C:\Users\trist\Downloads\Prog622B Assignment\PROG622B-Assignment\NeoVerse"
.\NeoVerse.exe
8. Troubleshooting
.exe does not open
Make sure NeoVerse.exe is in the same folder as the data folder.
Close any existing instance of the program if it is already running.
Rebuild the project in Visual Studio if needed.
Build errors in Visual Studio
Install the Desktop development with C++ workload.
Set the language standard to C++17.
Ensure all source and header files are included in the project.
9. Summary
You can run the project in two ways:

In Visual Studio 2022, using Ctrl + F5
Directly by opening NeoVerse.exe in the project folder
This gives a normal Windows terminal experience without needing an IDE open.
