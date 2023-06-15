# pair_style square/well

# Syntax
```
pair_style square/well cutoff
```

- cutoff = cutoff for square/well interactions (distance units)

# Examples
```
pair_style    square/well 2.0 
pair_coeff    1 2 square/well 5.0 0.33 0.005 1.32
```

# Description

The square/well style computes continuous version of the square-well potential given by:

$$
	U_{wp} (r)=-\frac{1}{2} \epsilon\left[1-\tanh\left(\frac{r-r_w}{\alpha}\right)\right]     r<r_{c}
$$
where $r_{c}$ is the cutoff.

The following coefficients must be defined for each pair of atoms types via the pair_coeff command as in the example above, or in the data file or restart files read by the read_data or read_restart commands:

-  $\epsilon$ (energy units)
-  $r_{w}$ (distance units)
-  $\alpha$
-  $r_{c}$ (distance units)

This continuous square-well potential can be used to calculate liquid-crystal interfacial energies through the mold integration1 or the lattice mold techniques {footcite:t}`espinosa2016lattice`,3. Also, it can simulate attractive patches distributed along the surface of a sphere, known as “patchy particles” ({footcite:t}`davidchack2003direct`).


The constants are hardcorded within the pair style and they not need to be defined.

```{footbibliography}

```
