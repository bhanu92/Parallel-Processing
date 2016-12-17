#!/bin/bash
#BSUB -n 32
#BSUB -o lsf.out
#BSUB -e lsf.err
#BSUB -q "windfall"
#BSUB -J histogram
#BSUB -R gpu
#---------------------------------------------------------------------

module load cuda

cd /home/u8/bhanu92/histo 

time ./histogram

###end of script
