import numpy as np

c = 299792458 
q_e = 1.602176634e-19

# Kasa method to fit circle parameters (x_c, y_c, R) from (x, y) points
def circle_fit(x, y):
    M = np.vstack([x, y, np.ones_like(x)]).T
    b = -(x*x + y*y)
    p, *_ = np.linalg.lstsq(M, b, rcond=None)
    A, B, C = p
    x_c = -A/2.0
    y_c = -B/2.0
    R = np.sqrt(x_c ** 2 + y_c ** 2 - C)
    return x_c, y_c, R

# Check if positive (RH) or negative (LH) spiral from sum of cross products
# of consecutive xy segments
def curvature_sign(x, y, z):
    cross_z = 0
    for i in range(1, len(x) - 1):
        x1, y1 = x[i] - x[i-1], y[i] - y[i-1]
        x2, y2 = x[i+1] - x[i], y[i+1] - y[i]
        cross_z += x1 * y2 - y1 * x2
    dir_z = z[-1] - z[0]
    return np.sign(cross_z * dir_z)

# Fit the helix parameters (x_c, y_c, R, tanl, z0) from (x, y, z) points
# x(phi) = xc + R cos(phi)
# y(phi) = yc + R sin(phi)
# z(phi) = z0 + tanl * R * (phi - phi0)
def fit_helix(x, y, z):
    x_c, y_c, R = circle_fit(x, y)

    phi = np.unwrap(np.arctan2(y - y_c, x - x_c))

    phi0 = phi[0]
    s = R * (phi - phi0)  

    A = np.vstack([s, np.ones_like(s)]).T
    tanl, z0 = np.linalg.lstsq(A, z, rcond=None)[0]

    return x_c, y_c, R, tanl, z0, phi

# Get pT and pz momenta (in MeV/c) from helix parameters
# Assume all particles have unit charge
def momentum_from_helix(R, tanl, B):
    pT = q_e * B * (R * 1e-3) # Convert R from mm to m
    pZ = pT * tanl
    return pT * c / (1e6 * q_e), pZ * c / (1e6 * q_e) 

# Get DCA from helix parameters, both input and output parameters in mm
def DCA_from_helix(x_c, y_c, R):
    return np.abs(np.sqrt(x_c ** 2 + y_c ** 2) - R) 

# Equation of the helix
def helix(x_c, y_c, z0, pZ, pT, R, phi):
    x = x_c + R * np.cos(phi)
    y = y_c + R * np.sin(phi)
    z = z0 + (pZ / pT) * R * (phi - phi[0])

    return x, y, z

# Calculate the root mean squared error of fit from data points
def RMSE(x, y, z, x_c, y_c, z0, pZ, pT, R):
    phi = np.unwrap(np.arctan2(y - y_c, x - x_c))
    x_fit, y_fit, z_fit = helix(x_c, y_c, z0, pZ, pT, R, phi)

    return np.sqrt(np.mean((x - x_fit) ** 2 + (y - y_fit) ** 2 + (z - z_fit) ** 2))