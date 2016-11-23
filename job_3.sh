#!/usr/bin/env bash
#SBATCH --job-name=mijieux3
#SBATCH --output=out.3
#SBATCH --error=err.3
#SBATCH -p mistral
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task 1

# 1 noeud / mkl séquentiel / 16 proc MPI

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load

do_job() {
    size=$1
    file=mat3_$size.txt
    let "size=(size+15) & ~15" # nearest multiple of 16
    ./genmat/genmat -b -s $size > $file
    export MKL_NUM_THREADS=1
    mpiexec -n 16 ./matprod -b -p $file $file
    unset MKL_NUM_THREADS
    rm $file
}

mpd &

for i in $(seq 100 100 1000); do
    do_job $i
done

for i in $(seq 5000 5000 20000); do
    do_job $i
done
