---
title: 'Mold: the square-like potential in a LAMMPS package to compute interfacial free energies and nucleation rates'
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
  - name: Nicodemo Di Pascuale
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

The precise knowledge of the structure and thermodynamical properties of interfaces between two coexisting phases is crucial for many physical phenomena. In particular, the coexistence of solid with other phase raises lot of attention due to the presence in many processes such as freezing, nucleation, or confinement. In that respect, a direct coexistence system with a solid presents key differences that impedes using some methods to calculate interfacial free energy that are valid for liquid-liquid or liquid-vapour interfaces. Additionally, the liquid-to-solid phase transition is a dynamical process that relies on a detailed knowledge of the crystal structure of the solid. The thermodynamical methods that we present here to tackle both problems use the presence of static well potentials with a square-like profile. On the one hand, the mold integration [@espinosa2014mold], to calculate interfacial free energies of liquid-solid or vapour-solid interfaces. On the other hand, the lattice mold method, that allows effective calculation of nucleation rates [@espinosa2016lattice]. The package we introduce here implements the square-like potential for the usage of such methods through the widely employed MD package LAMMPS (Large-Scale Atomic/Molecular Massively Parallel Simulator)[@Thompson2022].

# Statement of need

Phase transition and coexistence of phases is present in many physical as well as industrial processes such as freezing, nucleation, casting or welding. A profound knowledge of the underlying thermodynamics is fundamental to understand and manipulate such phenomena. In particular, the interfacial free energy is crucial to characterize the stability of a coexistence, understood as the amount of reversible work that allows creating a unit area of the interface between the two coexisting phases. The calculation of interfacial free energy of liquid-vapour or liquid-liquid phases can be easily attained through the anisotropy of the stress tensor [@walton1983pressure]. However, the same method leads to flawed results when calculating systems with solid phases [@DiPasquale2020shuttleworth]. 

The mold integration method was developed by Espinosa *et al.* [@espinosa2014mold] as a simple effective model to calculate the interfacial free energy of solid phases in contact with other phase. This technique uses ghost attractive particles defined as wells, that are arranged in the simulation box as a mold that replicates the solid crystalographic structure of the corresponding solid phase for one plane. The attractive wells are modeled through a square-like potential that is effectively characterize but the well radius $r_w$ and the well depth $\varepsilon_w$. The mold integration consists of the following steps:

1. Prepare de configuration introducing the plane mold into the liquid or vapour phase.
2. Calculation of the optimal well radius at a constant well depth and varying the radius.
3. Estimation of the interfacial free energy for different well radii above the optimal radius.
4. Linear extrapolation of the interfacial free energy to the optimal radius

![Top: Mold integration method thermodynamic path with sketch of the configuration. Bottom: Schematic representation of the Lattice mold technique with representative configurations. In both representations the black circles represent the wells that form the mold and the blue circles the fluid particles.\label{fig:mold}](figs/joss.png){ width=95% }

In the top part  of Fig. \autoref{fig:mold} we depict a sketch of the mold integration method with a generic solid plane and a liquid phase, showing the induction of the solid structure inside the liquid by the interactions with the wells that form the mold (black circles). For very attractive well (large well depth), changing the well radius implies lifting the free energy profile and the narrow the well radius the higher the free energy of the liquid, and thus the shorter the induction time. The optimal well radius is defined as the radius for which there is no induction time. The thermodynamical integration is perfomed then for values above the optimal radius by varying the well depth for each radius. Extrapolating the interfacial energy to the optimal radius provides the estimation of the interfacial free energy. 

The nucleation rate is also a fundamental observable to study the nucleation process. In this respect, the idea of using a mold to induce a nucleation in a fluid system leads to the lattice mold technique. In this method the nucleation rate is calculated by introducing a mold made of attractive wells whose positions are given by the crystallographic position of the solid that ones want to nucleate. In this case, the optimal radius of the well is reached when more than one fluid particle enters the well, so that the thermodynamical integration is carried out for smaller radius than the optimal one. Also, the mold must allow the systm to nucleate with a sufficiently long induction time so the integration of the system can be evaluated. This method can be summarized as:

1. Prepare de configuration introducing the plane mold into the liquid or vapour phase.
2. Calculation of the optimal well radius at a constant well depth and varying the radius.
3. Estimation of the free energy of .
4. Linear extrapolation of the interfacial free energy to the optimal radius
5. 

In the bottom part of Fig. \autoref{fig:mold} we depict a schematic picture of the lattice mold technique, where 

To date, the mold integration method has been widely utilized to calculate solid-liquid interfacial free energy of hard spheres and pseudo-hard spheres [@espinosa2014mold,@sanchez2021fcc], Lennard-Jones [@espinosa2014mold], SPC/E water with JC NaCl [@sanchez2023direct], Tosi-Fumi model for NaCl [@espinosa2015crystal], ice-water interfaces for TIP4P, TPI4P/2005 TIP4P/Ice and  mW models [@espinosa2016ice], and CO$_2$ hydrate water interfacial energy [@zeron2022simulation]. The lattice mold method has been used to calculate the nucleation rate of pseudo-hard spheres [@espinosa2016lattice], and mW and TIP4P/Ice water models [@sanchez2022homogeneous]. All these works have been carried out using a tabular potential for the square-like attractive interaction between the wells and the fluid particles, which makes the process more tedious. For this reason, only a few groups have used this technique since the methodology itself is indeed straightforward. This package implements the square-like potential in a explicit form on LAMMPS simulation package to ease the process of simulation for both methods. Indeed, this well potential has been also used for simulating pseudo-hard spheres with attractive well (termed as 'patches') on their surfaces. The works on this systems is mainly focused on the study of liquid-liquid phase separations of proteins and other biomolecules [@espinosa2019breakdown,espinosa2020liquid,joseph2021thermodynamics,sanchez2021valency], as well as crystallization of such particles [@garaizar2022alternating].


# Functionality

MOLD is a LAMMPS package for the calculation of IFE using Thermodynamic Integration. 


The package includes several new additional functions written in the style suited to be directly patched into LAMMPS, comprising interactions between atoms needed to run the MD simulation with the cleaving model and some auxiliary functionalities (in LAMMPS terms, new "fix" and "compute") needed to perform the operations required to carry out the different steps of the calculation. 

The version of the code we present here is hosted on GitHub, where a detailed instruction manual is complemented by some step-by-step examples reproducing some of the published results on the topic. 
The package includes new definitions of pair potentials not included in LAMMPS, as the Broughton-Gilmer modified Lennard-Jones potential [@Broughton1983] and modifications of some of the already existing pair_styles in LAMMPS. In particular, we included a modified version of the `lj/cut` and `coul/dsf` potentials. These modifications allow to execute the third step of the cleaving method, where the interactions between the phases are switched on and off. The difficulty in this case is to keep track of the interactions between atoms involved in the switching, since the work performed during this particular step depends on them. Moreover, these new `pair_style`s support different scaling (*i.e.*, different powers of the coupling parameter $\lambda$, see *e.g.*, [@DiPasquale2022cleaving]).

The processing of the interactions to generate the work in step 3 is carried out by the new compute `cleavpairs`, which generates the output file needed for the calculation of the IFE. The cleaving method owes its name to the fact that each bulk phase is "cleaved", *i.e.*, cut at a certain position to create the new interface. The cut is modelled with an external potential, that is, a potential depending on the absolute position of atoms within the simulation box. In the cleaving package there are two external potentials available: the "walls" and the "wells". These two external potentials are two additional fix styles, namely `wallforce` and `wellPforce`. We refer to the documentation for a detailed explanation of their use.
A new fix style `move/dupl` used in step 3 to switch the interactions between phases in contact is also included.

The new functionalities presented here are extremely general and can be applied to a variety of problems, making this package flexibile, thus answering one of the problem in the community, which is the lack of well maintained codes to carry out interfacial calculations to avoid relying on in-house codes and custom extensions. 

# Acknowledgements

We acknowledge support from the CECAM and CCP5 through the CECAM/CCP5 sandpit grant (2022) awarded to Nicodemo Di Pasquale and Lorenzo Rovigatti.

# References
