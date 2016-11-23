#!/usr/bin/env bash
#SBATCH --job-name=mijieux2
#SBATCH --output=out.2
#SBATCH --error=err.2
#SBATCH -p mistral
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task 20

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load
cd ${WORKDIR}/sequential

do_job() {
    size=$1
    file=mat_$size.txt
    ../genmat/genmat -b -s $size > $file
    ./matprod_seq -b -p $file $file
    rm $file
}

for i in $(seq 100 100 1000); do
    do_job $i
done

for i in $(seq 5000 5000 20000); do
    do_job $i
done
