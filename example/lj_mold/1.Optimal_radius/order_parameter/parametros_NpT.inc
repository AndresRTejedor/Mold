c *******************************************************
c *** fichero include con los parametros del programa NpT            
c ***
c *** Filosofia: Ningun parametro que no sea estrictamente
c ***            local se define dentro de una subrutina.
c ***            Todos los parametros que se compartan
c ***	           entre dos o mas rutinas se definen aqui,
c ***		     una sola vez.   
c ***
c *** Ventajas: 
c ***
c ***   1. Resulta mucho mas comodo el cambio de parametros,
c ***      ya que solo se hace una vez en este fichero y
c ***	     luego no hay que andar busando donde estan
c ***      para cambiarlos.
c ***
c ***   2. Al tomar valor una sola vez, se evita el error
c ***      comun de que el mismo parametro tenga asignados
c ***      valores distintos en distintas subrutinas
c ***      
c *************************************************************

c *** Parametros del NpT
c ------------------------


	implicit double precision (a-h,o-z)
c *** nmmax: numero total de moleculas
c *** nsmax: numero total de sites por molecula
        parameter (ntipmolmax=1)
	parameter (nmmax=200000,nsmax=1)
        parameter (nsisperio=1)
        parameter (intidmax=11)
        parameter (rpesomolec=1.)

c *** numero total de atomos 

      parameter (natmax=nmmax*nsmax)

c *** ngrmax   : numero de nodos en los que se evalua g(r)
c *** max_pair : numero total de pares de sites posibles

	parameter(ngrmax=500,max_pair=ntipmolmax*
     > (ntipmolmax*nsmax*nsmax+nsmax)/2)

c *** nrhomax  : numero maximo de nodos en la funcion de distribucion
c		     de densidades

	parameter(nrhomax=1700)

c *** Parametros de la Linked-cell-list
c -------------------------------------

c *** numero de celdas en cada una de las direcciones
c *** (necesario numero impar)

	parameter (nxcell=23,nycell=23,nzcell=23)

c *** numero total de celdas

	parameter (ntcell=nxcell*nycell*nzcell)
        parameter (ntcell4=ntcell)

c *** valor del numero mas proximo a 1/2 en precision de maquina

      parameter (half=0.4999999)


c **** Parámetros del configurational bias
c ---------------------------------------------------
c .... nktrial: numero de posiciones trial generadas para cada site
	parameter (nktrial=8)


c *** Parámetros de las sumas de Ewald
c ----------------------------------------------------
c .... mxvct: numero máximo de vectores de espacio recíproco
c .... lmax,mmax,nmax: número máximo de vectores de espacio recíproco
c          en cada una de las tres direcciones del espacio recíproco.	

	 parameter(mxvct=20000)
	 parameter(lmx=13,mmx=13,nmx=13)
       parameter(tolfour=0.0000001)

c **** Parametros de widom
	parameter(nattest=1)

c------------------------------------------------------------
c  ... Algunas constantes de cierta utilidad
c
       parameter(rk_boltz=1.)
       parameter(Coul_e=1.60217733E-19)
       parameter(rN_avo=1.)
c  ... Parametros del estudio de nucleacion

c 1.25 	parametro per primi vicini 

       parameter(rneigcl=0.) !Tiene que ser igual o menor que rneig (antes era 2)
       parameter(rneig=1.35)
       parameter(umbq6=0.7)
       parameter(umbq4=0)
c      parameter(iumbneigh=6) ! if you use q6
       parameter (umbq3=-0.75)
       parameter (iumbneigh=3)! if you use q3
       parameter (cutkoos=0.) 
       parameter (umbsol=0.340)

	 integer seed
