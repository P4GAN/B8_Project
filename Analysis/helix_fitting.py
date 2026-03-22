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

# Karimaki method to fit circle parameters (x_c, y_c, R) from (x, y) points
def karamaki_fit(x, y):    
    x_m, y_m = np.mean(x), np.mean(y)
    u, v = x - x_m, y - y_m
    
    r2 = u**2 + v**2
    mean_r2 = np.mean(r2)
    
    C_uu = np.mean(u**2)
    C_uv = np.mean(u * v)
    C_vv = np.mean(v**2)
    C_ur2 = np.mean(u * r2)
    C_vr2 = np.mean(v * r2)
    
    alpha = 0.5 * C_ur2
    beta = 0.5 * C_vr2
    
    denominator = C_uu * C_vv - C_uv**2
    
    xc_rel = (alpha * C_vv - beta * C_uv) / denominator
    yc_rel = (beta * C_uu - alpha * C_uv) / denominator
    
    R = np.sqrt(xc_rel**2 + yc_rel**2 + mean_r2)
    
    xc = xc_rel + x_m
    yc = yc_rel + y_m
    
    return xc, yc, R

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

# Fit the helix parameters (d0, z0, phi0, pT, tanl) from (x, y, z) points
def fit_helix(x, y, z, B):
    xc, yc, R_abs = karamaki_fit(x, y)

    phi_c = np.unwrap(np.arctan2(y - yc, x - xc))

    dphi_mean = np.mean(np.diff(phi_c))

    rot_sign = np.sign(dphi_mean)  

    R = rot_sign * R_abs

    d0 = np.sqrt(xc ** 2 + yc ** 2) - R_abs

    phi0 = np.arctan2(-xc, yc)

    phi = np.unwrap(np.arctan2(x - xc, -(y - yc)))
    phi = phi - 2.0 * np.pi * np.round((phi[0] - phi0) / (2.0 * np.pi))

    u = R * (phi - phi0)
    A = np.vstack([u, np.ones_like(u)]).T
    tanl, z0 = np.linalg.lstsq(A, z, rcond=None)[0]

    pT = q_e * B * (abs(R) * 1e-3)
    pT_MeV = pT * c / (1e6 * q_e)

    return d0, z0, phi0, pT_MeV, tanl

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