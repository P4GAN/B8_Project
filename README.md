# ePIC Silicon Vertex Tracker — Simulation & Track Reconstruction

**Parkin Pham** · B8 Computational Physics Project, MPhys Physics, University of Oxford

Supervised by [Dr Samuel Henry](https://www.physics.ox.ac.uk/our-people/henry)

A full Monte Carlo simulation and offline analysis pipeline for the **ePIC Silicon Vertex Tracker (SVT)** — the innermost tracking detector of the ePIC experiment at the future **Electron-Ion Collider** (Brookhaven National Laboratory, 2030s). The project models charged-particle transport through a simplified SVT geometry, reconstructs trajectories from simulated hits, and quantifies how detector design choices drive tracking performance.

<p align="center">
  <img alt="Geant4 event display" src="https://github.com/user-attachments/assets/17fd1293-0e01-4925-83b0-96d4d7a8f4fa" />
  <img alt="Reconstructed track parameters vs pseudorapidity" src="https://github.com/user-attachments/assets/126b71e5-951f-4e52-94fa-7f2c9f817cce" />
</p>

## Overview

- Simulated the passage of charged pions through a 5-layer cylindrical SVT geometry in **Geant4** (C++), including realistic materials, magnetic field, and the `FTFP_BERT` physics list.
- Reconstructed particle trajectories from simulated hits using a custom **helical track fit** (Python/NumPy/SciPy), extracting momentum, position, and angular resolution by comparison with Monte Carlo truth.
- Ran a systematic detector design study, scanning **magnetic field strength (0.5–2.5 T), silicon layer thickness (1×–4×), and intrinsic hit resolution (3–25 μm)** to quantify their effect on tracking precision.
- Simulated realistic electron-proton collision events at EIC energies with **Pythia8/HepMC3** to generate physically representative event samples.

## Key Results

- **Material budget dominates resolution.** Increasing detector material thickness degrades momentum resolution across *all* momenta, driven by multiple Coulomb scattering — identified as the single most important lever in SVT design.
- **Magnetic field strength selectively improves momentum resolution** via tighter track curvature, with negligible impact on position/angular resolution.
- **Intrinsic hit resolution matters most at high momentum**, where multiple scattering no longer dominates the error budget.
- Reproduced the expected two-regime behaviour of momentum resolution: multiple-scattering-limited at low momentum, hit-resolution-limited at high momentum — consistent with the requirements set out in the EIC Yellow Report.

Full methodology, plots, and discussion are in the [final report](Report/B8_Report.pdf) and [presentation](Presentation/B8_Presentation.pdf).

## Repository Structure

| Folder | Description |
|---|---|
| [`DetectorSimulation/`](DetectorSimulation) | Geant4 (C++) simulation of the ePIC SVT — detector geometry, materials, sensitive detectors, and run macros |
| [`CollisionSimulation/`](CollisionSimulation) | Pythia8 simulation of electron-proton collisions at EIC energies, output as HepMC3 |
| [`Analysis/`](Analysis) | Python/Jupyter pipeline for helical track fitting, resolution extraction, and performance plots |
| [`Report/`](Report/B8_Report.pdf) | LaTeX source and PDF of the final report |
| [`Presentation/`](Presentation/B8_Presentation.pdf) | LaTeX source and PDF of the project presentation |

## Tech Stack

`C++` · `Geant4` · `Python` · `NumPy` / `SciPy` · `pandas` · `Jupyter` · `ROOT` · `Pythia8` / `HepMC3` · `CMake`

## Setup

1. Download and build [Geant4 11.3.2](https://geant4.web.cern.ch/download/11.3.2.html) from source, following the [installation guide](https://geant4.web.cern.ch/documentation/dev/ig_html/InstallationGuide/installguide.html).
2. Download and build [ROOT](https://root.cern/install/build_from_source/) from source.
3. Build the detector simulation:

```bash
source /path/to/geant4-v11.3.2-install/bin/geant4.sh
cmake -S ./DetectorSimulation -B DetectorSimulation/build -DCMAKE_BUILD_TYPE=Debug
cmake --build DetectorSimulation/build -- -j
```

(`/path/to/geant4-v11.3.2-install/` is the Geant4 install directory used above.)

## Usage

**1. Run the detector simulation** for each configuration, producing ROOT files in `DetectorSimulation/output/`:

```bash
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

**2. Run the track fitting and analysis**, exporting tracking performance results to `Analysis/output/`:

```bash
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
