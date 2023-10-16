# pair_style square/well

## Syntax
```
pair_style square/well cutoff
```

- cutoff = cutoff for square/well interactions (distance units)

## Examples
```
pair_style    square/well 2.0 
pair_coeff    1 2 square/well 5.0 0.33 0.005 1.32
```

## Description

The square/well style computes a continuous version of the square-well potential given by:

$$
	U_{wp} (r)=-\frac{1}{2} \epsilon_w\left[1-\tanh\left(\frac{r-r_w}{\alpha}\right)\right]   \quad \text{for} \quad r<r_{c}
$$
where $r_{c}$ is the cutoff.

The following coefficients must be defined for each pair of atoms types via the pair_coeff command as in the example above, or in the data file or restart files read by the read_data or read_restart commands:

-  $\epsilon_w$ (energy units)
-  $r_{w}$ (distance units)
-  $\alpha$ (distance units)
-  $r_{c}$ (distance units)

This continuous square-well potential function was proposed in {footcite:t}`espinosa2014mold` to calculate liquid-crystal interfacial free energies through the Mold Integration, and later extended to the Lattice Mold technique ({footcite:t}`espinosa2016lattice` and {footcite:t}`sanchez2022homogeneous`). Also, it can be used to simulate attractive patches distributed along the surface of a sphere, known as “patchy particles” ({footcite:t}`espinosa2019breakdown`).


````{warning}
The value of $\alpha$ cannot be too small to avoid strong forces acting on the particles trapped inside the wells.
If small values are needed (steep potential), the simulation timestep must be reduced ({footcite:t}`sanchez2023direct`).
````

````{note}
In case $\epsilon$ is set to a negative value, the well potential would become fully repulsive.
````

## Restrictions

This pair style can only be used if LAMMPS was built with the [Mold package](https://github.com/AndresRTejedor/Mold). See the Build package doc page for more info.


```{footbibliography}

```
