# Mold method

Lammps package to implement Mold integration and Lattice mold techniques (USER-MOLD).

This package is presented as part of the consortium Simulation Interfacial Free Energy Techniques (SIFT) which include the packages:

  - [USER-CLEAVING](https://github.com/demonico85/cleaving/tree/master)
  - [Einstein IFE](https://github.com/syeandel/Einstein_IFE)

## Documentation

The documentation can be browsed online at [this link](https://andresrtejedor.github.io/Mold/) The documentation can also be generated locally through the following procedure, which requires a working python3 installation and an active internet connection.

```bash
cd docs
pip3 install -r docs_requirements.txt # use pip3 or pip depending on your local setup
make html
```
At the end of the generation point your browser to `build/html/index.html` to browse the docs.

NOTE: The repository contains a directory `/utils` which includes a miscellanea of programs useful for  mold calculations (e.g., extracting and averaging useful quantities). These simple programs are given without description and without warranty
