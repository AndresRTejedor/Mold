# Fix style well

```
fix_style wellPforce args
```

- args=list of the possible arguments

```
    wellPforce args = dw rw P lambda file <name file> 
    dw = depth of the well 
    rw = interaction range
    P = power law of the wells
    lambda = strength of the interactions between wells and atoms
    < name file > = external file which contains the position of the wells
```

On each side of the cleaving plane we place two planes with fixed interaction sites. Each site interacts with surrounding atoms through a wells shaped potential. The main difference with the walls (see [walls](../fix_wall.md)) version is that the two planes are now fixed, and the strength of the potential is the quantity which is varied within the simulation.
 
The well potential we use in this work is  defined as {footcite:t}`handel2008direct`: 

$$
	\psi(r;\lambda) = 
		\begin{cases}
			\lambda dw\left[\left(\frac{r}{rw}\right)^2-1\right]^{P} \:\:\: r < rw \\
			0  \:\:\: r \geq rw
		\end{cases} 
$$
where $r=|\mathbf{r}_{atoms}-\mathbf{r}_{wells}|$ for each couple atom-well.

In the cleaving procedure, the strength of the interactions sites is varied during the simulation using the parameter $\lambda$ from 0 (no interactions among the wells and the atoms) and 1 (full interactions between the wells and the atoms).

In the wells version each atom interacts with wells on _both_ side of the cleaving plane, therefore the external fine containing the position of the wells include a single list of positions (the interaction points). 
The format of the file is

```
N
x1 y1 z1 
x2 y2 z2
...
xN yN zN
```

where $N$ is the total number of wells. 

E.g.:

```
264
  0.28595722359534864       0.16509748001949254        16.343816675003122
  0.85787167078604587        1.1556823601364479        16.343816675003122
   1.4297861179767433       0.16509748001949254        16.3438166750031
...
   12.296160614599994        10.401141241228032        17.277749056431873
   11.724246167409296        11.391726121344988        17.277749056431873
``` 



```{footbibliography}

```
