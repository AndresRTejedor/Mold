# pair_style lj/BG

## Syntax

```
pair_style lj/BG cutoff1 cutoff2
```

* `cutoff1` = global internal cut-off
* `cutoff2` = global external cut-off

## Description

The {footcite:t}`Broughton1983` modification to Lennard-Jones potential is given by:

$$
	U(r_{ln}) =
		\begin{cases}
			4\epsilon\left(\left(\frac{\sigma}{r_{ln}}\right)^{12} -\left(\frac{\sigma}{r_{ln}}\right)^{6}  \right)+C_1,\;\mbox{if}\; r_{ln} \leq 2.3\sigma \\
							C_2\left(\frac{\sigma}{r_{ln}}\right)^{12} + C_3\left(\frac{\sigma}{r_{ln}}\right)^{6} + C_4\left(\frac{r_{ln}}{\sigma}\right)^2 + C_5,\;\mbox{if}\; r_{ln} \leq 2.5\sigma \\
				0, 		\; r_{ln} \geq 2.5\sigma		
		\end{cases}
$$
where $r_{ln}=|\mathbf{r}_l-\mathbf{r}_n|$ for each couple of atoms $l,n$ in the system, and $C_1, C_2, C_3, C_4, C_5$ are constants we used the values reported in {footcite:t}`davidchack2003direct`


The constants are hardcorded within the pair style and they do not need to be defined.

```{footbibliography}

```
