program create_data
  ! для seeding 
 !=====================================================
 !Program
 implicit none
integer                            :: Nliquid, Nliquid2, i,j, atom_id, timestep,n, ncopy, count, timestep_wait
double precision                   :: x0_l, xL_l, y0_l, yL_l, z0_l, zL_l
double precision                   :: rad_start, x_c, y_c, z_c, delta_rad, rad_stop, rad_current
double precision, pointer          :: atom_liquid(:,:), rad(:), vel_liquid(:,:) 

character*100                       :: FileDumpL,temp
character*100                       :: FileDumpSL 
character*2                         :: FMT
character*50                         :: format
 

 open (1, file ='params_in.txt',position="rewind", action="read",status="old")
 
 read(1,*) temp, timestep_wait
 read(1,*) temp, rad_start
 read(1,*) temp, delta_rad
 read(1,*) temp, rad_stop
 read(1,*) temp, ncopy
 count = 0 
 rad_current = rad_start + count*delta_rad

 DO WHILE (rad_current <= rad_stop)
  
  DO n=0,ncopy-1
  write(*,*) n
   
  write(format, '(A6,A2,A6)') "(A18, ", FMT(n), " , A4)"
  write(FileDumpL,format) 'dumpL/dump_liquid_', n ,'.txt'
  open (2, file =FileDumpL,position="rewind", action="read",status="old") 
 
 !read dump file of liquid-----
    
  read(2,*)
  read(2,*) timestep 
  read(2,*)
  read(2,*) Nliquid

  IF (timestep.ne.timestep_wait) THEN
   DO i=1, Nliquid + 9
     read(2,*)
   ENDDO
  ENDIF

  read(2,*) temp
  read(2,*) x0_l, xL_l
  read(2,*) y0_l, yL_l
  read(2,*) z0_l, zL_l
  read(2,*) temp
 allocate(atom_liquid(1:Nliquid,1:3))
 allocate(vel_liquid(1:Nliquid,1:3))
 allocate(rad(1:Nliquid)) 
 DO i=1, Nliquid
 read(2,*) atom_id, atom_liquid(atom_id,1), atom_liquid(atom_id,2), atom_liquid(atom_id,3), vel_liquid(atom_id,1),&
  vel_liquid(atom_id,2), vel_liquid(atom_id,3)
 ENDDO
 !----------------------------
 
 
  x_c = x0_l + (xL_l-x0_l)/2.d0
  y_c = y0_l + (yL_l-y0_l)/2.d0
  z_c = z0_l + (zL_l-z0_l)/2.d0
  
 ! delete particles-----------------------  
  Nliquid2 = 0
  DO i=1, Nliquid
    rad(i) = DSQRT((atom_liquid(i,1) - x_c)**2.d0+(atom_liquid(i,2) - y_c)**2.d0+(atom_liquid(i,3) - z_c)**2.d0)
    IF (rad(i).gt.rad_current) THEN
    Nliquid2 = Nliquid2 +1
    ENDIF
  ENDDO
 !-----------------------------------------

  CALL SYSTEM("mkdir -p dumps_twophase")
  
  write(format, '(A5,A2,A4,A2,A4)') "(A35,", FMT(count), ",A1,", FMT(n), ",A4)"
  write(FileDumpSL,format) 'dumps_twophase/dump_liquid_cluster_',count, '_', n ,'.txt'
  open (4, file=FileDumpSL,position="rewind", action="write",status="replace")
  write(4,'(A14)') 'ITEM: TIMESTEP'
  write(4,*) 0
  write(4,'(A21)') 'ITEM: NUMBER OF ATOMS'
  write(4,*) Nliquid2
  write(4,'(A25)') 'ITEM: BOX BOUNDS pp pp pp'
  write(4,*) x0_l, xL_l
  write(4,*) y0_l, yL_l
  write(4,*) z0_l, zL_l
  write(4,'(A34)') 'ITEM: ATOMS id type x y z vx vy vz'
  
  j=1
  DO i=1,Nliquid    
    IF (rad(i).gt.rad_current) THEN
     write(4,'(I6, A1, I1, F10.6, F10.6, F10.6, F10.6, F10.6, F10.6)') j, ' ', 1, atom_liquid(i,1), atom_liquid(i,2),&
      atom_liquid(i,3), vel_liquid(i,1), vel_liquid(i,2), vel_liquid(i,3)
     j=j+1
    ENDIF
  ENDDO
  
 close(2,status='keep')
 close(3,status='keep')

 ENDDO
 count = count+1;
 rad_current = rad_start + count*delta_rad
 ENDDO
 close(1,status='keep')
  
end program create_data

 !================FORMAT===========
FUNCTION FMT(i)
  implicit none
  character*2   :: FMT
  integer    :: i
!-------------------------------------
 IF ((i.ge.0).AND.(i.le.9)) THEN
  FMT = "I1"
 ELSE IF ((i.ge.10).AND.(i.le.99)) THEN
  FMT = "I2"
 ELSE IF ((i.ge.100).AND.(i.le.999)) THEN
  FMT = "I3"
 ELSE IF ((i.ge.1000).AND.(i.le.9999)) THEN
  FMT = "I4"
ELSE IF ((i.ge.10000).AND.(i.le.99999)) THEN
  FMT = "I5"
ELSE IF ((i.ge.100000).AND.(i.le.999999)) THEN
  FMT = "I6"
END IF
 END FUNCTION FMT
!=====================================



