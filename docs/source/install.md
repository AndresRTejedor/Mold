# Installation

## Requirements

The package requires a version of LAMMPS $\geq$ Mar2023, which can be downloaded from [here](https://www.lammps.org/download.html). Older versions are **not** compatible.

## Compilation

1. Compile Lammps with `make` following the [instructions](https://docs.lammps.org/Build_make.html)
2. Copy all files in `/mold/src` directory to the lammps `src` folder (*e.g.* `/home/user/lammps-28Mar2023/src`)
3. Run again `make mpi` to patch Lammps with the mold package

### Known issues

## Testing
