#!/usr/bin/env bash
#SBATCH --job-name=mijieux2
#SBATCH --output=out.2
#SBATCH --error=err.2
#SBATCH -p mistral
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --nodes=9
#SBATCH --ntasks=9
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task 10

# mkl parallele 20 threads pur (pas de MPI)

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load

do_job() {
    size=$1
    file=mat2_$size.txt
    let "size=(size+9-(size%9))"
    ./genmat/genmat -b -s $size > $file
    mpirun -n 9 ./matprod -b -p $file $file
    rm $file
}

for i in $(seq 100 100 1000); do
    do_job $i
done

do_job 2500

for i in $(seq 5000 5000 20000); do
    do_job $i
done
