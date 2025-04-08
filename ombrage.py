import numpy as np 
from scipy.signal import convolve2d
import matplotlib.pyplot as plt


def slope(dx,dy) :
    return np.arctan(np.sqrt((dx**2)+(dy**2)))
def aspect(dx,dy) :
    return np.arctan2(dy,dx)
def shade_aspect(slope, aspect, alti, azim):
    return 255*((np.cos(90-alti)*np.cos(slope))+(np.sin(90-alti)*np.sin(slope)*np.cos(azim-aspect)))


def shadeRender(array):
    #array = np.load("test.npy")
    print(array.shape)
    print(np.mean(array))

    # Noyau pour la dérivée partielle par rapport à x
    K_x = np.array([[-1, -2, -1],
                    [0, 0, 0],
                    [1, 2, 1]])
    # Noyau pour la dérivée partielle par rapport à y
    K_y = np.array([[-1, 0, 1],
                    [-2, 0, 2],
                    [-1, 0, 1]])
    dz_dx = convolve2d(array, K_x, mode='same', boundary='fill', fillvalue=0)/8
    dz_dy = convolve2d(array, K_y, mode='same', boundary='fill', fillvalue=0)/8
    print(dz_dx.shape)
    print(dz_dy.shape)
    slop = slope(dz_dx,dz_dy)
    aspec = aspect(dz_dx,dz_dy)
    print(slop.shape)
    print(aspec.shape)
    shade_aspec = shade_aspect(slop,aspec,45,45)
    print(shade_aspec.shape)
    plt.imshow(shade_aspec, cmap='Greys')  # You can change 'gray' to 'viridis' or other color maps
    plt.axis('off')  # Turn off axis labels
    plt.show()
    return shade_aspec