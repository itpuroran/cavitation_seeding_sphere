# 
variable	T0 equal 0.5
 

variable	P0 equal -1.45
variable	P1 equal -0.3


variable	rho0 equal 0.7989

variable	numb equal 50000
variable	abc0 equal (${numb}/${rho0})^(1/3)

variable	seed_date equal 11101
#---------------------------------------------------------------
units		lj
atom_style	atomic

log             logs/log3

lattice         fcc ${rho0}

region box 	block 0 ${abc0} 0 ${abc0} 0 ${abc0} 
create_box	1 box
create_atoms	1 random ${numb} ${seed_date} NULL
mass		1 1.0

read_dump    dump_start.txt 200000 x y z vx vy vz box yes replace yes 

pair_style	lj/cut  6.576
pair_coeff	1 1 1.0 1.0 6.576
neighbor	0.5 bin	
neigh_modify every 50 delay 0 check no
thermo_style custom step temp press pe etotal vol density
thermo		1000
timestep	0.00231832
#---------------------------------------------------------------

fix 3 all npt temp ${T0} ${T0} 0.231832 iso ${P0} ${P1} 2.31832 fixedpoint 0 0 0 
run 50000
unfix 3

reset_timestep 0

fix 3 all npt temp ${T0} ${T0} 0.231832 iso ${P1} ${P1} 2.31832
dump	all_dump all custom  100000 dump_liquid.txt id x y z vx vy vz 
run 100000
unfix 3
undump all_dump

#===============================================================
#=============<>     HAPPY END !!!      <>======================
#===============================================================
