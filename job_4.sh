#!/usr/bin/env bash
#SBATCH --job-name=mijieux4
#SBATCH --output=out.4
#SBATCH --error=err.4
#SBATCH -p mistral
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks=9
#SBATCH --ntasks-per-node=9
#SBATCH --cpus-per-task 1

# 1 noeud / mkl séquentiel / 9 proc MPI

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load

do_job() {
    size=$1
    file=mat4_$size.txt
    let "size=(size+9-(size%9))"
    ./genmat/genmat -b -s $size > $file
    export MKL_NUM_THREADS=1
    export OMP_NUM_THREADS=1
    mpirun -n 9 ./matprod -b -p $file $file
    unset MKL_NUM_THREADS
    rm $file
}

for i in $(seq 100 100 1000); do
    do_job $i
done

do_job 2500
