<div align="center">

# multiplayer-hub

*Developed in August 2023*

<br>

Use `Z` `Q` `S` `D` to move the player.

<br>

This project is a prototype developed in C++ using Object-Oriented Programming principles. It leverages the [raylib](https://www.raylib.com/) library for graphical rendering and the [ENet](http://enet.bespin.org/) library for network management.


<br>
<br>

</div>

---

# Building and Development

The game was developed on a Windows platform, utilizing CMake for build configuration and the MinGW compiler for code compilation.  
Follow the instructions below if you wish to compile using this setup.

<br>

## Compilation instructions

1. Open a PowerShell/cmd terminal and navigate to the client or server directory.

2. Create a build directory : 
```bash
mkdir build
```

3. Navigate to the build directory : 
```bash
cd build 
```

4. Generate the makefiles:
```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..  
```

5. Compile the project : (assuming that you use the minGW compiler)   
```bash
mingw32-make 
```

6. Run the game :   
```bash
.\..\bin\debug.exe
```
