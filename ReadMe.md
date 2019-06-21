# Bruteforce points of interest matching


## Getting Started ##

  * Clone repository

## Platforms ##

  * Linux (Manually tested on ArchLinux)
  
### Requirements

These are the base requirements to build

  * Cmake
  * A C++17-standard-compliant compiler
  * OpenCV2+
  * OpenCV-contrib
  * Python 3+
  * Tk
  
## Installing 

```bash
   chmod u+x build.sh
   ./build.sh
```  
File will be written to the folder "bin"

## How to use

```bash
   chmod u+x start.sh 
   ./start.sh "data" 10
```  

First argument for start.sh is path to the data folder, the second one - threshold for counting as good matching
