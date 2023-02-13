import os

if not os.path.exists('create_cavity_files'):
    os.makedirs('create_cavity_files')

if not os.path.exists('logs'):
    os.makedirs('logs')

if not os.path.exists('dumps_twophase'):
    os.makedirs('dumps_twophase')


def create_ini_files(lammps_filename):
    in_lammps_file = open(lammps_filename, 'r')
    R_curr = R_start
    count = 0
    while R_curr <= R_stop:
        for i in range(ncopy):
            out_lammps_file = open('create_cavity_files/{}_r{}_c{}'.format(lammps_filename, count, i), 'w')
            for line in in_lammps_file:
                if line.find('R equal') != -1:
                    line = line.strip()
                    if not line.startswith('#'):
                        columns = line.split()
                        if len(columns) >= 1:
                            line = line.replace(columns[3], str(R_curr + delta_R))
                            line = line + '\n'
                if line.find('dump	all_dump') != -1:
                    line = line.strip()
                    if not line.startswith('#'):
                        columns = line.split()
                        if len(columns) >= 1:
                            line = line.replace(columns[5],
                                                'dumps_twophase/dump_liquid_cluster_r{}_c{}.txt'.format(count, i))
                            line = line + '\n'
                if line.find('log') != -1:
                    line = line.strip()
                    if not line.startswith('#'):
                        columns = line.split()
                        if len(columns) >= 0:
                            line = line.replace(columns[1], 'logs/log_r{}_c{}.txt'.format(count, i))
                            line = line + '\n'
                if line.find('read_dump') != -1:
                    line = line.strip()
                    if not line.startswith('#'):
                        columns = line.split()
                        if len(columns) >= 0:
                            line = line.replace(columns[1], 'dumpL/dump_liquid_{}.txt'.format(i))
                            line = line + '\n'
                if line.find('variable        self_name') != -1:
                    line = line.strip()
                    if not line.startswith('#'):
                        columns = line.split()
                        if len(columns) >= 0:
                            line = line.replace(columns[3],
                                                'create_cavity_files/{}_r{}_c{}'.format(lammps_filename, count, i))
                            line = line + '\n'
                out_lammps_file.write(line)
            out_lammps_file.close()
            in_lammps_file.seek(0)
        count += 1
        R_curr += delta_R
    in_lammps_file.close()


# User must set these variables
R_start = 1
R_stop = 4
delta_R = 1
ncopy = 2
input_filename = "create_cavity_sphere_repulse"

# Generate the new lammps input files
create_ini_files(input_filename)
