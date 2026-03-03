import sys

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import ROOT

from helix_fitting import fit_helix, momentum_from_helix, DCA_from_helix, RMSE

if len(sys.argv) < 3:
    print("Usage: python fit_tracks.py <input_root_file> <output_csv_file>")
    sys.exit(1)

B = 1.7

if len(sys.argv) > 3:
    B = float(sys.argv[3])

input_root_file = "DetectorSimulation/output/" + sys.argv[1]
output_csv_file = "Analysis/output/" + sys.argv[2]

file = ROOT.TFile.Open(input_root_file)
hits = file.Get("hits")
tracks = file.Get("tracks")

tracks.BuildIndex("EventID")

num_hits = hits.GetEntries()

track_hits = {}

for i in range(num_hits):
    hits.GetEntry(i)
    eventID = hits.EventID
    if eventID not in track_hits:
        track_hits[eventID] = [np.array([]), np.array([]), np.array([])]
    track_hits[eventID][0] = np.append(track_hits[eventID][0], hits.PositionX)
    track_hits[eventID][1] = np.append(track_hits[eventID][1], hits.PositionY)
    track_hits[eventID][2] = np.append(track_hits[eventID][2], hits.PositionZ)

data = []

for eventID, track in track_hits.items():
    if len(track[0]) < 2:
        continue
    x, y, z = track
    x_c, y_c, R, tanl, z0, phi, sign = fit_helix(x, y, z)

    fitted_pT, fitted_pZ = momentum_from_helix(R, tanl, B)
    fitted_p = np.sqrt(fitted_pT ** 2 + fitted_pZ ** 2)
    fitted_DCA = DCA_from_helix(x_c, y_c, R)

    tracks.GetEntryWithIndex(eventID)
    pX, pY, pZ = tracks.MomentumX, tracks.MomentumY, tracks.MomentumZ
    p = np.sqrt(pX ** 2 + pY ** 2 + pZ ** 2)
    pT = np.sqrt(pX **2 + pY ** 2)
    eta = np.arctanh(pZ / p)

    momentum_accuracy = np.abs(fitted_p - p) / p * 100
    transverse_momentum_accuracy = np.abs(fitted_pT - pT) / pT * 100
    z_momentum_accuracy = np.abs(fitted_pZ - pZ) / np.abs(pZ) * 100

    error = RMSE(x, y, z, x_c, y_c, z0, fitted_pZ, fitted_pT, R)

    data.append({
        "EventID": eventID,
        "Momentum (MeV/c)": p,
        "Transverse Momentum (MeV/c)": pT,
        "Z Momentum (MeV/c)": pZ,
        "Pseudorapidity": eta,
        "Momentum Accuracy (%)": momentum_accuracy,
        "Transverse Momentum Accuracy (%)": transverse_momentum_accuracy,
        "Z Momentum Accuracy (%)": z_momentum_accuracy,
        "DCA (mm)": fitted_DCA,
        "RMSE": error
    })


df = pd.DataFrame(data)
df.to_csv(output_csv_file, index=False)    