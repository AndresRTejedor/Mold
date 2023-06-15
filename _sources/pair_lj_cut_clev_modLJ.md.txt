# pair_style lj/cleavcutsqlmod



- args=list of the possible arguments
```
<name pair style> args =  cut-off lambda i_IDflag orientation alpha
    cutoff        = global internal cut-off
    lambda        = global scaling of the potential
    i_IDflag      = new (integer) property to be added to the atoms
    orientation   = direction perpendicular to the cleaving plane
    alphaLJ       = damping parameter (inverse distance units)
```

`pair_coeff` accepts only the following arguments

```
pair_coeff a b lambda 
```

where

```
    a       = atom of type a [mandatory]
    b       = atom of type b [mandatory]
    epsilon = energy constant (energy units)
    sigma   = VdW radius (distance units)
    lambda  = scaling of the potential
    
```


This pair style is derived from the `pair/ljcut` in LAMMPS and we refer to the [LAMMPS documentation](https://docs.lammps.org/pair_lj.html) for the
description of its main features. Here, only the modifications needed for the cleaving calculations
will be considered. 


This pair style modifies the Lennard-Jones potential as proposed by {footcite:p}`Beutler1994` in order to avoid the "Lennard-Jones catastrophe". The potential is modified as:

$$
	U(r_{ln}) =
			\lambda 4\epsilon\left(\frac{1}{\left(\alpha_{LJ}(1-\lambda)^2+\left(\frac{r_{ln}}{\sigma}\right)^{6}\right)^2} -\frac{1}{\left(\alpha_{LJ}(1-\lambda)^2+\frac{r_{ln}}{\sigma}\right)^{6}\right)^2}  \right),\;\mbox{if}\; r_{ln} \leq r_c \\
$$

where $r_{ln}=|\mathbf{r}_l-\mathbf{r}_n|$ for each couple of atoms $l,n$ in the system and $r_c$ is the cut-off for the potential.


The description of the cleaving methodology using this pair style is analogous to the one described for the pair style [coul/dsf[cleav]](./pair_coul_cleav.md) to which we refer.


```{footbibliography}

```
