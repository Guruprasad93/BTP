#! usr/bin/python

#IMPORT FILES
import pickle
import os, time, sys
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from itertools import product, combinations, izip
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.set_aspect("equal")
counter = 1

l_matrix = 100
b_matrix = 100
h_matrix = 100


fig = plt.gcf()



def scatterplot(x,y,z):
    
    global counter, h_matrix, l_matrix, b_matrix
        
    ax = fig.add_subplot(111,projection='3d')
    ax.scatter(x,y,z,c='r',marker='o')

    a =[[0,0,0],[l_matrix,0,0],[l_matrix,b_matrix,0],[0,b_matrix,0],[0,0,0],[0,0,h_matrix],[0,b_matrix,h_matrix],[0,b_matrix,0],[0,0,0]]

    b= [[l_matrix,0,0],[l_matrix,0,h_matrix],[0,0,h_matrix],[0,b_matrix,h_matrix],[l_matrix,b_matrix,h_matrix],[l_matrix,b_matrix,0]]

    c = [[l_matrix,b_matrix,h_matrix],[l_matrix,0,h_matrix]]

    a= a+b+c
    plt.plot(*zip(*a))
    ax.view_init(elev=80, azim=0)
    ax.set_xticklabels([])
    ax.set_yticklabels([])
    ax.set_zticklabels([])
        



    
    #plt.show()
    print "Saving to file now" 
    path = os.getcwd()+'/pics_2'
    
    filename = 'Cellular Automaton'
    newname = '{}{:03d}.png'.format(filename, counter)

    newname = os.path.join(path,newname)
    fig.savefig(newname)
    counter = counter + 1
    
    
   
  
  
   
#Reading from .txt file

def read_file():
    
    x_file = open("/home/guru/Desktop/x_coordinates.txt", "r")
    y_file = open("/home/guru/Desktop/y_coordinates.txt", "r")
    z_file = open("/home/guru/Desktop/z_coordinates.txt", "r")
    time =0;
    ctr = 0;
    for linex, liney, linez in izip(x_file, y_file, z_file):
        linex = linex.strip()
        liney = liney.strip()
        linez = linez.strip()
        xcoords = [int(s) for s in linex.split(',')]
        ycoords = [int(s) for s in liney.split(',')]
        zcoords = [int(s) for s in linez.split(',')]
        #xcoords = linex.split(",")
        #ycoords = liney.split(",")
        #zcoords = linez.split(",")
        #print xcoords, "\n"
        #print ycoords, "\n", zcoords
        print "Viewing state at time =", time;
        time = time+1
        if ctr%2 == 0:     
        	scatterplot(xcoords,ycoords,zcoords)
        ctr = ctr+1
        
        
        
def main():
    read_file()

if __name__=='__main__':
    main()
    
      

