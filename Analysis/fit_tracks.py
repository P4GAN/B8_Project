import sys

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import ROOT

from helix_fitting import fit_helix

B = 1.7
min_hits_per_track = 4
cutoff_momentum = 50_000 # 50 GeV

if len(sys.argv) < 3:
    print("Usage: python Analysis/fit_tracks.py <input_root_file> <output_csv_file>")
    sys.exit(1)

if len(sys.argv) > 3:
    B = float(sys.argv[3])

input_root_file = "DetectorSimulation/output/" + sys.argv[1]
output_csv_file = "Analysis/output/" + sys.argv[2]

file = ROOT.TFile.Open(input_root_file)
tracks = file.Get("tracks")

# treat each entry as one primary track, with hit position vectors stored in branches
num_tracks = tracks.GetEntries()

# convenience for filling DataFrame
data = []

for i in range(num_tracks):
    tracks.GetEntry(i)
    if tracks.NumHits < min_hits_per_track:
        continue
    
    x = np.array(tracks.HitPositionX)
    y = np.array(tracks.HitPositionY)
    z = np.array(tracks.HitPositionZ)

    d0, z0, phi0, fitted_pT, tanl = fit_helix(x, y, z, B)

    fitted_pZ = tanl * fitted_pT
    fitted_p = np.sqrt(fitted_pT ** 2 + fitted_pZ ** 2)

    pX, pY, pZ = tracks.MomentumX, tracks.MomentumY, tracks.MomentumZ
    p = np.sqrt(pX ** 2 + pY ** 2 + pZ ** 2)
    eta = np.arctanh(pZ / p)

    if fitted_p > cutoff_momentum:
        continue

    data.append({
        "True p": round(p),
        "True pX": pX,
        "True pY": pY,
        "True pZ": pZ,
        "eta": eta,
        "Fit d0": d0,
        "Fit z0": z0,
        "Fit phi0": phi0,
        "Fit pT": fitted_pT,
        "Fit tanl": tanl,
        "NumHits": tracks.NumHits
    })


df = pd.DataFrame(data)
df.to_csv(output_csv_file, index=False)    