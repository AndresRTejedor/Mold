---
title: 'Mold: a LAMMPS package to compute interfacial free energies and nucleation rates'
tags:
  - C++
  - LAMMPS
  - Molecular simulations
  - Phase Transitions
  - Nucleation theory
authors:
  - name: Andres R. Tejedor
    orcid:  0000-0002-9437-6169
    affiliation: "1,2"
  - name: Ignacio Sanchez-Burgos
    orcid: 0000-0002-1160-3945
    affiliation: 3
  - name: Eduardo Sanz
    orcid: 0000-0001-6474-5835
    affiliation: 2
  - name: Carlos Vega
    orcid: 0000-0002-2417-9645
    affiliation: 2
  - name: Felipe J. Blas
    orcid: 0000-0001-9030-040X
    affiliation: 4
  - name: Ruslan Davidchack
    orcid: 0000-0001-9418-5322
    affiliation: 5
  - name: Nicodemo Di Pasquale
    orcid:  0000-0001-5676-8527
    affiliation: 6
  - name: Jorge Ramirez^[Corresponding author]
    orcid: 0000-0002-8946-3786
    affiliation: 7
  - name: Jorge R. Espinosa^[Corresponding author]
    orcid: 0000-0001-9530-2658
    affiliation: 2
affiliations:
 - name: Yusuf Hamied Department of Chemistry, University of Cambridge, United Kingdom
   index: 1
 - name: Departamento de Química Física, Facultad de Ciencias Químicas, Universidad Complutense de Madrid, Spain
   index: 2
 - name: Cavendish Laboratory, Department of Physics, University of Cambridge, United Kingdom
   index: 3
 - name: Laboratorio de Simulacion Molecular y Quimica Computacional, CIQSO-Centro de Investigacion en Quimica Sostenible and Departamento de Ciencias Integradas, Universidad de Huelva, Spain
   index: 4
 - name: Department of Mathematics, University of Leicester, United Kingdom
   index: 5
 - name: Department of Chemical Engineering, Brunel University London, United Kingdom
   index: 6
 - name: Department of Chemical Engineering, Universidad Politécnica de Madrid, Spain
   index: 7
date: 01 Sep 2023
bibliography: paper.bib
---

# Summary

The determination of the free energy cost of forming an interface between two phases, *i.e.* the interfacial free energy, is critical for characterizing a phase transition. In the particular case of liquid-to-solid transitions, obtaining this quantity is not straightforward; neither experimentally nor computationally. Here, we present a computational package integrated in the Molecular Dynamics open-source software LAMMPS (Large-Scale Atomic/Molecular Massively Parallel Simulator)[@Thompson2022], which enables the direct calculation of the interfacial free energy between arbitrarily complex crystal structures and liquids/solutions at coexistence conditions through the Mold Integration method [@espinosa2014mold]. Furthermore, the extension of this method for determining crystal nucleation rates---i.e., the probablity of a critical nucleus to emerge per unit of volume and time within a metastable phase---is also incorporated in the package, termed Lattice Mold method [@espinosa2016lattice]. Altogether, we detail here the required codes, scripts, and instructions for evaluating the two most challenging quantities determining the feasibility of a liquid-to-solid transition, implemented to be included in the MD package LAMMPS.

# Statement of need

Liquid-to-solid phase transitions are present in many physical and industrial processes such as freezing [@espinosa2016ice], nucleation [@mer1952nucleation], casting [@asta2009solidification] or welding [@asta2009solidification]. A thorough knowledge of the underlying thermodynamic processes is essential for understanding and controlling such phenomena. In particular, the interfacial free energy, defined as reversible work required to form a unit area of the interface between two coexisting phases, is crucial for determining the conditions that lead to a phase transition. The calculation of the interfacial free energy of liquid-vapour or liquid-liquid phases can be easily attained by analyzing the anisotropy of the stress tensor [@kirkwood1949statistical],[@walton1983pressure]. However, the same method leads to flawed results when used in systems involving solid phases [@DiPasquale2020shuttleworth]. For that reason, we have introduced this two different computational methos to tackle that are directly incorporated in the LAMMPS code to facilitate their functionality. 

# Scientific background

The Mold Integration method was developed by Espinosa *et al.* [@espinosa2014mold] as a simple and effective approach to calculate the interfacial free energy of crystalline phases in coexistence with a liquid phase. This technique uses potential wells, that are arranged in the simulation box as a mold that replicates a given crystallographic plane of the solid of interest. The attractive wells are modeled through a square-well-like potential that is characterized by the well radius $r_w$, the well depth $\varepsilon_w$ and the well slope $\alpha$. The Mold Integration method consists of the following steps:

1. Preparation of the configuration, by introducing the mold into the liquid phase.
2. Simulation of the liquid-mold system at constant well depth and different well radii to determine the optimal radius that keeps the liquid particles inside the wells.
3. Calculation of the reversible work required to form a crystal slab at different well radii above the optimal well radius.
4. Linear extrapolation of the interfacial free energy to the optimal well radius.

![Top: Sketch of the thermodynamic pathway to compute the interfacial free energy using the Mold Integration method. Bottom: Schematic representation of the different steps in the Lattice Mold technique to compute the nucleation rate. In both schemes the empty black circles represent the potential wells that form the mold and the blue spheres represent the fluid particles. An extrapolation to a given well radius based on calculations performed at different radii is required in both methods.\label{fig:mold}](figs/joss.png){ width=95% }

In the top part  of \autoref{fig:mold} we depict a sketch of the Mold Integration method with a generic solid plane and a liquid phase, showing the induction of the solid structure inside the liquid by the interactions of the liquid particles with the potential wells forming the mold (empty black circles). In stage 2, the depth of the wells is fixed to a value such that each well is filled with a fluid particle. Changes in the well radius leads to a modulation of the required free energy to create a crystal slab. A larger radius implies a longer induction time to form the interface so that the optimal radius is defined as the largest radius for which there is no induction time [@espinosa2014mold]. The thermodynamic integration (stage 3) is perfomed then for values above the optimal radius that allow reversibility (in the formation of the crystal slab) by varying the well depth for each radius. Extrapolating the interfacial energy to the optimal radius (stage 4) provides the estimation of the interfacial free energy. 

The nucleation rate is also a fundamental observable required to characterize nucleation phenomena. In this respect, the idea of using a mold to induce a crystal nucleus in the fluid leads to the Lattice Mold technique [@espinosa2016lattice]. In this method the nucleation rate is calculated by introducing a mold made of attractive wells whose positions are given by the crystallographic position of the solid at the same temperature for which the nucleation rate aims to be computed. In contrast with the Mold Integration method, the mold is not a crystal plane but a crystal cluster, typically with spherical shape, since this shape minimizes the interfacial area of the emerging cluster within the metastable fluid. In this case, the optimal radius of the well is reached in the limit beyond which more than a single fluid particle occupies a well. Hence, the thermodynamic integration is carried out for smaller radii than the optimal one. The mold must allow the system to nucleate (at maximum well depth) after a sufficiently long induction time, therefore ensuring reversibility across the thermodynamic integration step. This method can be summarized as follows:

1. Preparation of the configuration by introducing the crystal mold nucleus into the liquid phase.
2. Simulation with different well radii and a constant value of the well depth (which ensures that wells are filled) in order to determine the optimal well radius.
3. Evaluation of the free energy required to fill the mold with liquid particles and generate a pre-critical cluster at several well radii below the optimal one.
4. Calculation of the average nucleation time for each well radius over different independent trajectories.
5. Extrapolation of the nucleation rate to the optimal well radius.

In the bottom part of \autoref{fig:mold} we depict a schematic picture of the Lattice Mold technique, showing the different simulation steps to compute the nucleation rate of the system (see [@sanchez2022homogeneous] for further details). The nucleation free energy to form a sub-critical cluster is calculated from the well-occupancy vs well depth curve for each well radius. The average induction time is then calculated by running independent trajectories (at the highest well depth used in the previous step) and measure the time it takes for the pre-critical crystal nucleus to overcome the free energy barrier and percolate the whole system.

To date, the Mold Integration method has been widely used to calculate solid-liquid interfacial free energy of hard spheres [@espinosa2014mold], pseudo-hard spheres [@sanchez2021fcc], Lennard-Jones [@espinosa2014mold], SPC/E water with JC NaCl [@sanchez2023direct], NaCl (Tosi-Fumi) [@espinosa2015crystal], ice-water interfaces for TIP4P, TPI4P/2005 TIP4P/Ice and  mW models [@espinosa2016ice], and CO$_2$ hydrate water interfacial energy ([@zeron2022simulation], [@algaba2022simulation], [@romero2023effect]). The Lattice Mold method has been used to calculate the nucleation rate of pseudo-hard spheres and NaCl from its melt [@espinosa2016lattice], and mW and TIP4P/Ice water models [@sanchez2022homogeneous]. All these simulations have been carried out with GROMACS [@abraham2015gromacs] and using a tabulated potential for the square-like attractive interaction between the wells and the fluid particles. Additionally, the typical run is more efficient in terms of computational resources when running on LAMMPS with the analytic form of the potential [@sanchez2022homogeneous]. In fact, when using tabulated potentials in GROMACS, long-range corrections in energy and pressure cannot be applied across the calculation, and hence, larger cut-offs in the interaction potential need to be employed. This is even more notable when studying complex systems such as atomistic models of water [@sanchez2022homogeneous] or hydrates [@zeron2022simulation]. Moreover, a tabulated potential needs to be generated for each run with different well parameters, which makes the process of simulation more complex and tedious. This package implements the square-like potential in a explicit form on LAMMPS to speed up the simulation performance and simplify the implementation of both methods.

Further to the applications of this method, the same well potential has been used for simulating 'patchy particles' [@russo2009reversible] that consist of hard spheres with several potential wells on their surface. The present LAMMPS package enables performing efficient simulation of patchy particles using an explicit potential and thus facilitating reproducibility. These works include the study of liquid-liquid phase separation of proteins and other biomolecules ([@espinosa2019breakdown], [@espinosa2020liquid], [@joseph2021thermodynamics], [@sanchez2021valency], [@sanchez2021size], [@sanchez2023surfactants]), as well as crystallization of such particles with different topologies ([@russo2009reversible], [@garaizar2022alternating], [@sciortino2011reversible], [@romano2011crystallization]).

# Functionality

MOLD is a LAMMPS package for the calculation of interfacial free energy and nucleation rates through both the Mold Integration and the Lattice Mold techniques, respectively. The package includes the implementation of the pair potential (with the characteristic syntax of LAMMPS) to simulate the square-like potential that is used in both methods as well as in the simulation of patchy particles.

The code presented here can be found on [Github](https://github.com/AndresRTejedor/Mold), and it provides a detailed manual with the instructions to compile and use the package as well as with step-by-step examples to implement both the Mold Integration and the Lattice Mold techniques using the explicit square well potential. The package uses the `pair_style square/well` with three parameters that control the radius, the depth and the slope of the interaction potential. Additionally, we provide two detailed worked examples to reproduce already published results. 

The first example explores [the Mold Integration](https://andresrtejedor.github.io/Mold/example_MI_lj.html) technique for the (100) plane of the fcc crystal for the Lennard-Jones potential. This example reproduces the results of the works of [@davidchack2003direct] using the cleaving technique and [@espinosa2014mold] using the Mold Integration method, and uses the Broughton-Gilmer modified Lennard-Jones potential [@Broughton1983] that can be found in the CLEAVING package [here](https://github.com/demonico85/cleaving/tree/master). The second example shows step-by-step instructions to perform [Lattice Mold](https://andresrtejedor.github.io/Mold/example_LM_mw.html) calculations of mW Ice Ih nucleation rate at T=220K, reproducing the results of [@sanchez2022homogeneous] using the same method that is compatible with other methods ([@haji2018forward],[@li2011homogeneous],[@haji2014suppression]). Both examples include all the LAMMPS scripts, the configuration files and simple bash and Python scripts to run the simulations and analyze the results.

Overall, this simulation package aims to simplify the simulation process of the presented methods and allow the community to perform simulations of solid-to-liquid phase transitions in a more efficient way.

# Acknowledgements

We kindly acknowledge Dr. Lorenzo Rovigatti for sharing a Github repository as a guide to build our documentation. This project was funded by projects PID2022-136919NA-C33, PID2022-136919NB-C31, and PID2022-136919NB-C32 and PID2022-136919NB-C33 of the MICINN, and the Oppenheimer Research Fellowship of the University of Cambridge. I.S.-B. acknowledges funding from the Oppenheimer Fellowship, Derek Brewer scholarship of Emmanuel College and EPSRC Doctoral Training Programme studentship, number EP/T517847/1. N. D. P. acknowledges support from the CECAM and CCP5 through the CECAM/CCP5 sandpit grant (2022) and the EP/V028537/1 grant. J. R. acknowledges funding from the Spanish Ministry of Economy and Competitivity (PID2019-105898GA-C22) and the Madrid Government (Comunidad de Madrid-Spain) under the Multiannual Agreement with Universidad Politécnica de Madrid in the line Excellence Programme for University Professors, in the context of the V PRICIT (Regional Programme of Research and Technological Innovation). J.R.E. also acknowledges funding from the Ramon y Cajal fellowship (RYC2021-030937-I). A.R.T. acknowledges funding from the European Research Council (ERC) under the European Union Horizon 2020 research and innovation programme (grant agreement 803326), and from the Ramon y Cajal fellowship. This work has been performed using resources provided by the Cambridge Tier-2 system operated by the University of Cambridge Research Computing Service (http://www.hpc.cam.ac.uk) funded by EPSRC Tier-2 capital grant EP/P020259/1.This project made use of time on HPC granted via the UK High–End Computing Consortium for Biomolecular Simulation, HECBioSim (http://hecbiosim.ac.uk), supported by EPSRC (grant no. EP/X035603/1). F. J. B. also acknowledges Ministerio de Ciencia e Innovación (Grant No. PID2021-125081NB-I00), Junta de Andalucía (P20-00363), and Universidad de Huelva (P.O. FEDER UHU-1255522 and FEDER-UHU-202034), all four cofinanced by EU FEDER funds.

# References
