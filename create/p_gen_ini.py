#! /usr/bin/env python

# This file is used to take a template input json file for forward flux
# and create a input file for ssages that uses multiple walkers.

import json 
import numpy as np
import os
from random import randint

class LessPrecise(float):
    def __repr__(self):
        return str(self)

if not os.path.exists('in_files'):
        os.makedirs('in_files');

if not os.path.exists('dumpL'):
        os.makedirs('dumpL');

if not os.path.exists('logs'):
        os.makedirs('logs');



def lammps_random_number(lammps_filename, nWalkers):
    """Takes a lammps input file, finds the seed number of langevin, replaces it with a random number, and
	generates new input files
	"""
    randon_numbers = set()
    while (len(randon_numbers) < nWalkers):
        randon_numbers.add(randint(0, 100000))
    randon_numbers = list(randon_numbers)

    in_lammps_file = open(lammps_filename, 'r')

    for i in range(0, nWalkers):
        out_lammps_file = open('in_files/'+lammps_filename + '-' + str(i), 'w')
        for line in in_lammps_file:
            if line.find('seed_date equal') != -1:
                line = line.strip()
                if not line.startswith('#'):
                    columns = line.split()
                    if len(columns) >= 3:
                        line = line.replace(columns[3], str(randon_numbers[i]))
                        line = line + '\n'
            if line.find('all_dump') != -1:
                line = line.strip()
                if not line.startswith('#'):
                    columns = line.split()
                    if len(columns) >= 10:
                        line = line.replace(columns[5], 'dumpL/dump_liquid_'+str(i)+'.txt')
                        line = line + '\n'
	    if line.find('read_dump') != -1:
                line = line.strip()
                if not line.startswith('#'):
                    columns = line.split()
                    if len(columns) >= 1:
                        line = line.replace(columns[1], 'dumpL_0.5_-1.45/dump_liquid_'+str(i)+'.txt')
                        line = line + '\n'
            if line.find('log') != -1:
                line = line.strip()
                if not line.startswith('#'):
                    columns = line.split()
                    if len(columns) >= 0:
                        line = line.replace(columns[1], 'logs/log'+str(i))
                        line = line + '\n'


            out_lammps_file.write(line)
        in_lammps_file.seek(0)
        out_lammps_file.close()

    in_lammps_file.close()


# User must set these variables
nWalkers = 50
input_filename = "in.l"
# Use if you have many equally-spaced interfaces

#Generate the new lammps input files
lammps_random_number(input_filename, nWalkers)

input = []
for i in range(0, nWalkers):
    input.append('ini_files/'+input_filename+ '-' + str(i))

	
