# Mold method

Lammps package to implement Mold integration and Lattice mold techniques (USER-MOLD).

This package is presented as part of the consortium Simulation Interfacial Free Energy Techniques (SIFT) which include the packages:

  - [USER-CLEAVING](https://github.com/demonico85/cleaving/tree/master)
  - [Einstein IFE](https://github.com/syeandel/Einstein_IFE)

![SIFT group](./docs/figs/final_logo.png)

## Documentation

The documentation can be browsed online at [this link](https://andresrtejedor.github.io/Mold/) The documentation can also be generated locally through the following procedure, which requires a working python3 installation and an active internet connection.

```bash
cd docs
pip3 install -r docs_requirements.txt # use pip3 or pip depending on your local setup
make html
```
At the end of the generation point your browser to `build/html/index.html` to browse the docs.

NOTE: The repository contains a directory `/utils` which includes a miscellanea of programs useful for  mold calculations (e.g., extracting and averaging useful quantities). These simple programs are given without description and without warranty

## Contributing

If you want to add functionalities (or fix issues) to MOLD, you are more than welcome to do so. Feel free to fork the repo and open a pull request: we will do our best to review it. Note that if we accept your changes you will be asked to update the documentation to reflect what you have done.

If you have any questions, find a bug or an issue or want to propose a new feature, please do so by creating a [new issue](https://github.com/AndresRTejedor/Mold/issues). 
