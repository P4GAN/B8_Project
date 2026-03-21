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
python Analysis/fit_tracks.py resolution_4um.root resolution_4um.csv
python Analysis/fit_tracks.py resolution_15um.root resolution_15um.csv
python Analysis/fit_tracks.py resolution_25um.root resolution_25um.csv