# Fix style wall


```
fix_style wallforce args
```

- args=list of the possible arguments

```
    wallforce args = eps sigma zw delta rw file <name file> 
    eps   = (energy units)
    sigma = (distance units)
    zw    = position of the walls respect to the cleaving plane
    delta = width of the interpolation region in the m function
    rw    = cut-off of the LJ potential
    < name file > = external file which contains the position of the walls
```

On each side of the cleaving plane we place two planes with fixed interaction sites. Each site interacts with surrounding atoms through a wall shaped potential which is constructed from the repulsive core of the LJ potential, defined using a standard Weeks–Chandler–Anderson splitting {footcite:t}`hansen2013theory`:

$$
\begin{equation}
	\phi(r) = 4\epsilon \cip{ \frac{\sigma^{12}}{r^{12}} - \frac{\sigma^{6}}{r^{6}}} + \epsilon
\end{equation}
$$

Interactions of the atom with the walls are given by

$$
\Phi(\mathbf{r};z_w)=m(\Phi_1, \Phi_2)
$$

where $m$ is a modified minimum function defined in {footcite:p}`davidchack2003direct` and $\Phi_1$, $\Phi_2$ are given by:

$$
\begin{align}
	\Phi_1(\mathbf{r};z_w) & = \sum_{j} \phi\cip{\abs{\mathbf{r} - \cip{\mathbf{r}_j^{(1)} - z_w\bfn} }} = \sum_{j} \phi\cip{\abs{\mathbf{r} - \mathbf{r}_j^{(1)} + z_w\bfn }} \\
	\Phi_2(\mathbf{r};z_w)  & = \sum_{j} \phi\cip{\abs{\mathbf{r} - \cip{\mathbf{r}_j^{(2)} + z_w\bfn} }}  =  \sum_{j} \phi\cip{\abs{\mathbf{r} - \mathbf{r}_j^{(2)} - z_w\bfn }} \.
\end{align}
$$


In the cleaving procedure, the strength of the interactions sites is varied during the simulation using the parameter $z_w$ which varies from an initial value $z_{w,i}$ to a final value $z_{w,f}$. There is no _a priori_ prescription for these two values except that the initial value  $z_{w,i}$ must be such that the walls do not interact with the atoms in the system.

Note that the way the wall potential is defined the atoms interacts with only one of the two walls (the one that gives the lowest interactions).

The format of the file is:

```
N1
x1 y1 z1 
x2 y2 z2
...
xN1 yN1 zN1
N2
x1 y1 z1 
x2 y2 z2
...
xN1 yN1 zN1
```

where $N_1$ is the total number of walls on one side of the cleaving plane, whereas  $N_2$ is the total number of walls on opposite side of the cleaving plane. 

E.g.:

```
132
  0.28595722359534864       0.16509748001949254        16.343816675003122
  0.85787167078604587        1.1556823601364479        16.343816675003122
   1.4297861179767433       0.16509748001949254        16.3438166750031
...
   12.296160614599994        10.401141241228032        17.277749056431873
   11.724246167409296        11.391726121344988        17.277749056431873
132
  0.28595722359534864       0.16509748001949254        16.343816675003122
  0.85787167078604587        1.1556823601364479        16.343816675003122
   1.4297861179767433       0.16509748001949254        16.3438166750031
...
   12.296160614599994        10.401141241228032        17.277749056431873
   11.724246167409296        11.391726121344988        17.277749056431873
```    
``` 


```{footbibliography}

```
