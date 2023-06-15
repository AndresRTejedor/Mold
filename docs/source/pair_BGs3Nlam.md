# pair_style lj/BGNlcleavs3


## Syntax


```
pair_style lj/BGNlcleavs3args
```

- args=list of the possible arguments

```
lj/BGNlcleavs3 args = cutoff1 cutoff2 lambda Dfac 
    cutoff1 = global internal cut-off
    cutoff2 = global external cut-off
    lambda  = global scaling of the potential
    N       = exponent of the function lambda^N
    Dfac    = multiplicative factor to multiply the derivative of the interaction with respect to lambda 
```


`pair_coeff` accept the same arguments of the pair_style. However, keep in mind that parameters in pair coeff have a specific order

```
pair_coeff a b lambda Dfac cutoff1 cutoff2
```

where

```
    a       = atom of type a [mandatory]
    b       = atom of type b [mandatory]
    lambda  = scaling of the potential
    Dfac    = multiplicative factor for the derivative of the interaction with respect to lambda     
    cutoff1 = global internal cut-off
    cutoff2 = global external cut-off
```

the arguments in the `pair_coeff` are all optional except for the atom types. Note that the order is important. If we want to specify a new lambda for a different pair of types we can just write

```
pair_coeff 1 1 1.0
```

However, in order to specify a new (e.g., external) cutoff we need to rewrite all the preceeding arguments:

```
pair_coeff 1 1 lambda Dfac cutoff1 cutoff2
```

## Examples

```
pair lj/BGNlcleavs3 2.3 2.5 
pair coeff 1 1 1.0
pair coeff 1 2 1.0
pair coeff 1 3 -1.0
```

## Description

This pair style implements the Broughton and Gilmer modification to Lennard-Jones potential {footcite:t}`Broughton1983` (see [pair lj/BG]{pairBG.md}) to be used in the step3 of the cleaving algorithm. This pair style returns also an array with all the calculated interactions which are needed to calculate the work in the step3. This array can be accessed using the new compute [compute paircleav]}{compute_pcleav.md}. 


The potential implemented in this pair style is 

$$
	U(r_{ln},\lambda) =
		\begin{cases}
			\lambda^N\left[ 4\epsilon \left(\left(\frac{\sigma}{r_{ln}}\right)^{12} -\left(\frac{\sigma}{r_{ln}}\right)^{6}  \right)+C_1 \right],\;\mbox{if}\; r_{ln} \leq 2.3\sigma \\
			\lambda^N\left[ C_2\left(\frac{\sigma}{r_{ln}}\right)^{12} + C_3\left(\frac{\sigma}{r_{ln}}\right)^{6} + C_4\left(\frac{r_{ln}}{\sigma}\right)^2 + C_5\right],\;\mbox{if}\; r_{ln} \leq 2.5\sigma \\
				0, 		\; r_{ln} \geq 2.5\sigma		
		\end{cases}
$$

where $r_{ln}=|\mathbf{r}_l-\mathbf{r}_n|$ for each couple of atoms $l,n$ in the system, and $C_1, C_2, C_3, C_4, C_5$ are constants we used the values reported in {footcite:t}`davidchack2003direct`. 
The constants are hardcorded within the pair style and they not need to be defined.

````{note}
* $N$ must be an integer > 1
* There is no internal check that $\lambda$ is changing consistently (i.e., always decreasing or increasing)
````
   
The work performed on the system to switch from $\lambda=1$ to $\lambda=0$ is then given by

$$
	\frac{\partial U(r_{ln},\lambda)}{\partial \lambda} =
		\begin{cases}
			N\lambda^{N-1}\left[ 4\epsilon \left(\left(\frac{\sigma}{r_{ln}}\right)^{12} -\left(\frac{\sigma}{r_{ln}}\right)^{6}  \right)+C_1 \right],\;\mbox{if}\; r_{ln} \leq 2.3\sigma \\
			N\lambda^{N-1}\left[ C_2\left(\frac{\sigma}{r_{ln}}\right)^{12} + C_3\left(\frac{\sigma}{r_{ln}}\right)^{6} + C_4\left(\frac{r_{ln}}{\sigma}\right)^2 + C_5\right],\;\mbox{if}\; r_{ln} \leq 2.5\sigma \\
				0, 		\; r_{ln} \geq 2.5\sigma		
		\end{cases}
$$

In this case the expression of the potential is multiplied by the derivative of $\lambda$ with respect itself which is equal to 1 (i.e., $\partial \lambda / \partial \lambda=1$), and therefore the value of Dfrac must be set equal to 1.

Using this style we can represent different switches between different states of the system:

* We want to describe the switching between two different states of the system, e.g., $a$ and $a+b$. 
Let us assume the total Hamiltonian of the system, $H^{T}(\lambda)$, is defined as 

$$
	H^{T}(\lambda)= \lambda^N H_{a} + H_{b} 
$$  

for $\lambda \in [0,1]$, where we dropped the dependence on $\mathbf{p}$ and $\mathbf{q}$ for simplicity.

Let's consider this example:

```
variable lambda file lambda.dat

pair_style lj/BGNlcleavs3  2.3 2.5 1.0 1.0
pair_coeff 1 1 1.0 1.0
pair_coeff 2 2 1.0 1.0

pair_coeff 1 2 ${lambda} 1.0
```

The "self" interactions between atoms of the same type remains, but the mixed interactions between atoms of different types are `switched-on` (if $\lambda$ goes from 0 to 1) or `switched-off` (if $\lambda$ goes from 1 to 0).


* We want to describe the simultaneous switch between two different states of the system, e.g., $a$ and $b$. 
Let us assume the total Hamiltonian of the system, $H^{T}(\lambda)$, is defined as 

$$
	H^{T}(\lambda)= \lambda^N H_{a} + (1-\lambda)^N H_{b} 
$$  

for $\lambda \in [0,1]$, where we dropped the dependence on $\mathbf{p}$ and $\mathbf{q}$ for simplicity.
In this latter case, the command Dfrac needs to be used when we are passing to the pair_style an expression like $(1-\lambda)$. Here, we need to set Dfrac=-1.

```
variable N equal 4
variable lambda file lambda.dat
variable minl   equal 1-lambda

pair_style lj/BGNlcleavs3  2.3 2.5 1.0 $N 1.0
pair_coeff 1 1 1.0 1.0
pair_coeff 2 2 1.0 1.0
pair_coeff 3 3 1.0 1.0

pair_coeff 1 2 ${lambda} 1.0
pair_coeff 1 3 ${minl} 1.0
pair_coeff 2 3 1.0 1.0
```

The "self" interactions are not modified during the run, as well as the interactions between atoms of type 2 and 3. However, interactions between atoms of type 1 and 2 are `switched-on` (`switched-off`) and interactions between atoms of type 1 and 3 are  `switched-off` (`switched-on`) if $\lambda$ goes from 0 to 1 (from 1 to 0).


````{note}
The "complement" switching obtained with this pair style is $(1-\lambda)^N \neq (1-\lambda^N)$. In terms of Thermodynamic Integration they are both equivalent as long as, given a function of $\lambda$, i.e. $f(\lambda)$, we have $f(\lambda)=0$ for $\lambda=0$ and $f(\lambda)=1$ for $\lambda=1$.
````


The work performed in this step can be collected and printed out by using the compute [paircleav](./compute_pcleav.md). The work obtained is already the correct one as all the numerical coefficients (i.e., N and/or -1 coming from the derivation with respect to $\lambda$) are already included in the calculation.


````{note}
When $N=1& this pair style is equivalent to [pair_style lj/BGcleavs3](./pair_BGs3lam.md).
````


```{footbibliography}

```
