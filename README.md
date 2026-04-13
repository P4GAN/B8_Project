# ePIC SVT Simulation

This repository contains code for Monte Carlo simulations of particle transport in the ePIC Silicon Vertex Tracker using Geant4, along with Python code for track fitting and evaluating detector performance. This code is written for a B8 computational project as part of the MPhys Physics degree at Oxford. 

The /DetectorSimulation/ folder contains the Geant4 simulation of the ePIC SVT
The /CollisionSimulation/ folder contains a short Pythia8 code for simulating the result of a typical electron-proton collision at the EIC
The /Analysis/ folder contains Python files and Jupyter notebooks for track fitting from detector hits, as well as plots of tracking performance
The /Report/ folder contains the LaTeX files for the final report

## Setup

Download [Geant4-11.3.2](https://geant4.web.cern.ch/download/11.3.2.html)

Follow the steps for [building and installing from source](https://geant4.web.cern.ch/documentation/dev/ig_html/InstallationGuide/installguide.html)

Also download and install [ROOT](https://root.cern/install/build_from_source/)

Run the below command to build:

```
source /path/to/geant4-v11.3.2-install/bin/geant4.sh && cmake -S ./DetectorSimulation -B DetectorSimulation/build -DCMAKE_BUILD_TYPE=Debug && cmake --build DetectorSimulation/build -- -j'
```

Where /path/to/geant4-v11.3.2-install/ is the install directory that was used when installing Geant4

## Usage
The following runs the detector simulation for each of the macro files, and outputs the results to ROOT files in /DetectorSimulation/output/

```
    cd DetectorSimulation

    build/DetectorSimulation macros/default.mac
    build/DetectorSimulation macros/BField_0_5T.mac
    build/DetectorSimulation macros/BField_1_0T.mac
    build/DetectorSimulation macros/BField_2_5T.mac
    build/DetectorSimulation macros/gun_electrons.mac
    build/DetectorSimulation macros/gun_kaons.mac
    build/DetectorSimulation macros/gun_positrons.mac
    build/DetectorSimulation macros/gun_protons.mac
    build/DetectorSimulation macros/material_x2.mac
    build/DetectorSimulation macros/material_x3.mac
    build/DetectorSimulation macros/material_x4.mac
    build/DetectorSimulation macros/resolution_4um.mac
    build/DetectorSimulation macros/resolution_15um.mac
    build/DetectorSimulation macros/resolution_25um.mac
```

The following runs the Python track fitting and analysis on the ROOT files and exports the tracking performance results to /Analysis/output/

```
    cd ..
    python Analysis/fit_tracks.py default.root default.csv
    python Analysis/fit_tracks.py BField_0_5T.root BField_0_5T.csv 0.5
    python Analysis/fit_tracks.py BField_1_0T.root BField_1_0T.csv 1.0
    python Analysis/fit_tracks.py BField_2_5T.root BField_2_5T.csv 2.5
    python Analysis/fit_tracks.py gun_electrons.root gun_electrons.csv
    python Analysis/fit_tracks.py gun_kaons.root gun_kaons.csv
    python Analysis/fit_tracks.py gun_positrons.root gun_positrons.csv
    python Analysis/fit_tracks.py gun_protons.root gun_protons.csv
    python Analysis/fit_tracks.py material_x2.root material_x2.csv
    python Analysis/fit_tracks.py material_x3.root material_x3.csv
    python Analysis/fit_tracks.py material_x4.root material_x4.csv
    python Analysis/fit_tracks.py resolution_3um.root resolution_3um.csv
    python Analysis/fit_tracks.py resolution_15um.root resolution_15um.csv
    python Analysis/fit_tracks.py resolution_25um.root resolution_25um.csv
```