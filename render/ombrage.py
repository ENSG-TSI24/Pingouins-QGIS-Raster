import numpy as np

from scipy.signal import convolve2d

def slope(dx, dy):
    return np.arctan(np.sqrt(dx**2 + dy**2))

def aspect(dx, dy):
    return np.arctan2(dy, dx)

def shade_aspect(slope, aspect, alti, azim):
    # alti et azim doivent être en degrés, donc on les convertit ici
    zenith = np.radians(90 - alti)
    azimuth = np.radians(azim)
    return 255 * (
        np.cos(zenith) * np.cos(slope) +
        np.sin(zenith) * np.sin(slope) * np.cos(azimuth - aspect)
    )


def shadeRender(array, alti, azim):
    # Noyaux de Sobel pour les dérivées
    K_x = np.array([[-1, -2, -1],
                    [0,  0,  0],
                    [1,  2,  1]])
    K_y = np.array([[-1,  0,  1],
                    [-2,  0,  2],
                    [-1,  0,  1]])

    dz_dx = convolve2d(array, K_x, mode='same', boundary='fill', fillvalue=0) / 8
    dz_dy = convolve2d(array, K_y, mode='same', boundary='fill', fillvalue=0) / 8

    slop = slope(dz_dx, dz_dy)
    aspec = aspect(dz_dx, dz_dy)
    shade = shade_aspect(slop, aspec, alti, azim)

    # Normalisation (éventuelle)
    return np.clip(shade, 0, 255).astype(np.float32)