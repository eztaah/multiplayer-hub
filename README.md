# Multiplayer Hub

Multiplayer hub made using SFML and Enet libraries.

### Controls
- Use `Z` `Q` `S` `D` to move the player.

---

# Building and Development

## Informations
- Project developed using the [SFML](https://www.sfml-dev.org/) and [ENet](http://enet.bespin.org/index.html) library.

## Prerequisites
- Any compiler
- CMake

## Compilation instructions

1. Open a PowerShell/cmd terminal and navigate to the project's directory.

2. Create a build directory : 
```
mkdir build
```

3. Navigate to the build directory : 
```
cd build 
```

4. Generate the makefiles:
```
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..  
```

5. Compile the project : (assuming that you use the minGW compiler)   
``` 
mingw32-make 
```

6. Run the game :   
```
.\..\bin\debug.exe
```
