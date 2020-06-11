# Treedepth-Pace-2020
A solver for computing treedepth decompositions. Created for the PACE-Challenge 2020.

[Click here to read the paper](PACE_2020_Paper.pdf)

# Submission Overview
This repository contains a heuristic solver for computing treedepth decompositions, it's written entirely in c++. It's also capable of solving small graphs optimally. That's why we want to submit it to the heuristic and exact track.
Note that we do not have a proof of correctness for solving small graphs optimally.

# Installation
The folder 'solver' contains the source code. The folder also contains a CMakeLists.txt to compile the source code.
Running CMake will output a executable with the name "StrategicSolver", hence when submitting to PACE the folder 'solver' has to be a .tgz named "StrategicSolver.tgz".

# Requirements
To run the program boost needs to be installed.