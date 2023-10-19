/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.

   Contributing author: Di Pasquale Nicodemo
   University of Leicester, March 2020
   email: nicodemo.dipasquale@gmail.com    
   
   The documentation for this pair potential can be browsed at the following link:
   https://demonico85.github.io/cleaving/
    


------------------------------------------------------------------------- */



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pair_lj_BG.h"
#include "atom.h"
#include "comm.h"
#include "force.h"
#include "neighbor.h"
#include "neigh_list.h"
#include "neigh_request.h"
#include "update.h"
#include "integrate.h"
#include "math_const.h"
#include "memory.h"
#include "error.h"

using namespace LAMMPS_NS;
using namespace MathConst;

// Definition coefficients 
// Davidchack, R. L. and Laird, B. B. Journal of Chemical Physics,  118, 7651:7657, (2003)

#define  C1  0.016132
#define  C2  3136.6
#define  C3 -68.069
#define  C4 -0.083312
#define  C5  0.74689


/* ---------------------------------------------------------------------- */

PairLJBG::PairLJBG(LAMMPS *lmp) : Pair(lmp)
{
  single_enable = 1;
  respa_enable  = 0;
  manybody_flag = 0;
  writedata     = 1; 
  restartinfo   = 0;


}


/* ---------------------------------------------------------------------- */

PairLJBG::~PairLJBG()
{
  if (allocated) {
    memory->destroy(setflag);
    memory->destroy(cutsq);
    memory->destroy(cut);
    memory->destroy(cutsq_in);
    memory->destroy(cut_in);
    memory->destroy(epsilon);
    memory->destroy(sigma);
    memory->destroy(lj1);
    memory->destroy(lj2);
    memory->destroy(lj3);
    memory->destroy(lj4);
    memory->destroy(lj5);
    memory->destroy(lj6);
    memory->destroy(lj7);
    memory->destroy(lj8);
    memory->destroy(lj9);
    memory->destroy(lj10);
    memory->destroy(c1);
    memory->destroy(c5);
    delete [] pvector;
  }
}

/* ----------------------------------------------------------------------
   allocate all arrays
------------------------------------------------------------------------- */

void PairLJBG::allocate()
{
  allocated = 1;
  int n = atom->ntypes;

  memory->create(setflag,n+1,n+1,"pair:setflag");
  for (int i = 1; i <= n; i++)
    for (int j = i; j <= n; j++)
      setflag[i][j] = 0;


  memory->create(cutsq,n+1,n+1,"pair:cutsq");
  memory->create(cut,n+1,n+1,"pair:cut");
  memory->create(cutsq_in,n+1,n+1,"pair:cutsq2");
  memory->create(cut_in,n+1,n+1,"pair:cut2");
  memory->create(epsilon,n+1,n+1,"pair:epsilon");
  memory->create(sigma,n+1,n+1,"pair:sigma");
  memory->create(lj1,n+1,n+1,"pair:lj1");
  memory->create(lj2,n+1,n+1,"pair:lj2");
  memory->create(lj3,n+1,n+1,"pair:lj3");
  memory->create(lj4,n+1,n+1,"pair:lj4");
  memory->create(lj5,n+1,n+1,"pair:lj5");
  memory->create(lj6,n+1,n+1,"pair:lj6");
  memory->create(lj7,n+1,n+1,"pair:lj7");
  memory->create(lj8,n+1,n+1,"pair:lj8");
  memory->create(lj9,n+1,n+1,"pair:lj9");
  memory->create(lj10,n+1,n+1,"pair:lj10");
  memory->create(c1,n+1,n+1,"pair:c1");
  memory->create(c5,n+1,n+1,"pair:c5");

}


/* ---------------------------------------------------------------------- */

void PairLJBG::compute(int eflag, int vflag)
{

  int i,j,ii,jj,inum,jnum,itype,jtype,m;
  double xtmp,ytmp,ztmp,delx,dely,delz,evdwl,fpair;
  double rsq,r2inv,r6inv,forcelj,factor_lj;
  int *ilist,*jlist,*numneigh,**firstneigh;

  evdwl = 0.0;
  if (eflag || vflag) ev_setup(eflag,vflag);
  else evflag = vflag_fdotr = 0;

  double **x = atom->x;
  double **f = atom->f;
  int n = atom->ntypes;
  int *type = atom->type;
  int nlocal = atom->nlocal;
  double *special_lj = force->special_lj;
  int newton_pair = force->newton_pair;


  inum = list->inum;
  ilist = list->ilist;
  numneigh = list->numneigh;
  firstneigh = list->firstneigh;

  for (ii = 0; ii < inum; ii++) {

    i = ilist[ii];
    xtmp = x[i][0];
    ytmp = x[i][1];
    ztmp = x[i][2];
    itype = type[i];
    jlist = firstneigh[i];
    jnum = numneigh[i];

    for (jj = 0; jj < jnum; jj++) {
      j = jlist[jj];

      factor_lj = special_lj[sbmask(j)];
      j &= NEIGHMASK;

      delx = xtmp - x[j][0];
      dely = ytmp - x[j][1];
      delz = ztmp - x[j][2];
      rsq = delx*delx + dely*dely + delz*delz;
      jtype = type[j];


      if (rsq <= cutsq_in[itype][jtype]) {
        r2inv = 1.0/rsq;
        r6inv = r2inv*r2inv*r2inv;
        forcelj = r6inv * (lj1[itype][jtype]*r6inv - lj2[itype][jtype]);
        fpair = factor_lj*forcelj*r2inv;

        f[i][0] += delx*fpair;
        f[i][1] += dely*fpair;
        f[i][2] += delz*fpair;
        if (newton_pair || j < nlocal) {
          f[j][0] -= delx*fpair;
          f[j][1] -= dely*fpair;
          f[j][2] -= delz*fpair;
            }

         if (eflag) {
            m=jtype+(itype-1)*n;
            evdwl = r6inv*(lj3[itype][jtype]*r6inv-lj4[itype][jtype]) + c1[itype][jtype];
            evdwl *= factor_lj;
          }

        if (evflag) ev_tally(i,j,nlocal,newton_pair,
                             evdwl,0.0,fpair,delx,dely,delz);

        }
      else if (rsq < cutsq[itype][jtype]){

        r2inv = 1.0/rsq;
        r6inv = r2inv*r2inv*r2inv;
        forcelj = r6inv * (lj5[itype][jtype]*r6inv + lj6[itype][jtype]);
        fpair = factor_lj*(forcelj*r2inv - lj7[itype][jtype]);

        f[i][0] += delx*fpair;
        f[i][1] += dely*fpair;
        f[i][2] += delz*fpair;
        if (newton_pair || j < nlocal) {
          f[j][0] -= delx*fpair;
          f[j][1] -= dely*fpair;
          f[j][2] -= delz*fpair;
            }

        if (eflag) {
            m=jtype+(itype-1)*n;
            evdwl = r6inv*(lj8[itype][jtype]*r6inv + lj9[itype][jtype]) + lj10[itype][jtype]*rsq + c5[itype][jtype];
            evdwl *= factor_lj;
            }

        if (evflag) ev_tally(i,j,nlocal,newton_pair,
                             evdwl,0.0,fpair,delx,dely,delz);

      }
    }
  }

  if (vflag_fdotr) virial_fdotr_compute();


}


/* ----------------------------------------------------------------------
   proc 0 writes to restart file
------------------------------------------------------------------------- */

void PairLJBG::write_restart(FILE *fp)
{
  write_restart_settings(fp);

  int i,j;
  for (i = 1; i <= atom->ntypes; i++)
    for (j = i; j <= atom->ntypes; j++) {
      fwrite(&setflag[i][j],sizeof(int),1,fp);
      if (setflag[i][j]) {
        fwrite(&epsilon[i][j],sizeof(double),1,fp);
        fwrite(&sigma[i][j],sizeof(double),1,fp);
        fwrite(&cut_in[i][j],sizeof(double),1,fp);
        fwrite(&cut[i][j],sizeof(double),1,fp);
      }
    }
}

/* ----------------------------------------------------------------------
   proc 0 reads from restart file, bcasts
------------------------------------------------------------------------- */

void PairLJBG::read_restart(FILE *fp)
{
  read_restart_settings(fp);
  allocate();

  int i,j;
  int me = comm->me;
  for (i = 1; i <= atom->ntypes; i++)
    for (j = i; j <= atom->ntypes; j++) {
      if (me == 0)utils::sfread(FLERR,&setflag[i][j],sizeof(int),1,fp,nullptr,error); 
      
      MPI_Bcast(&setflag[i][j],1,MPI_INT,0,world);
      if (setflag[i][j]) {
        if (me == 0) {
          utils::sfread(FLERR,&epsilon[i][j],sizeof(double),1,fp,nullptr,error);
          utils::sfread(FLERR,&sigma[i][j],sizeof(double),1,fp,nullptr,error);
          utils::sfread(FLERR,&cut_in[i][j],sizeof(double),1,fp,nullptr,error);
          utils::sfread(FLERR,&cut[i][j],sizeof(double),1,fp,nullptr,error);                  
        }
        MPI_Bcast(&epsilon[i][j],1,MPI_DOUBLE,0,world);
        MPI_Bcast(&sigma[i][j],1,MPI_DOUBLE,0,world);
        MPI_Bcast(&cut_in[i][j],1,MPI_DOUBLE,0,world);
        MPI_Bcast(&cut[i][j],1,MPI_DOUBLE,0,world);
      }
    }
}


/* ----------------------------------------------------------------------
   proc 0 writes to restart file
------------------------------------------------------------------------- */

void  PairLJBG::write_restart_settings(FILE *fp)
{

  fwrite(&cut_global_in,sizeof(double),1,fp);
  fwrite(&cut_global_out,sizeof(int),1,fp);

}

/* ----------------------------------------------------------------------
   proc 0 reads from restart file, bcasts
------------------------------------------------------------------------- */

void  PairLJBG::read_restart_settings(FILE *fp)
{
  int me = comm->me;
  if (me == 0) {
    utils::sfread(FLERR,&cut_global_in,sizeof(double),1,fp,nullptr,error);
    utils::sfread(FLERR,&cut_global_out,sizeof(double),1,fp,nullptr,error);
  }
  MPI_Bcast(&cut_global_in,1,MPI_DOUBLE,0,world);
  MPI_Bcast(&cut_global_out,1,MPI_INT,0,world);
}


/* ----------------------------------------------------------------------
   global settings
------------------------------------------------------------------------- */

void PairLJBG::settings(int narg, char **arg)
{
  if (narg != 2) error->all(FLERR,"Illegal pair_style command");


  cut_global_in  = utils::numeric(FLERR,arg[0],false,lmp);
  cut_global_out = utils::numeric(FLERR,arg[1],false,lmp);

  // reset cutoffs that have been explicitly set

  if (allocated) {
    int i,j;
    for (i = 1; i <= atom->ntypes; i++)
      for (j = i+1; j <= atom->ntypes; j++)
        if (setflag[i][j]) cut[i][j] = cut_global_out;
  }
}


/* ----------------------------------------------------------------------
   set coeffs for one or more type pairs
------------------------------------------------------------------------- */



void PairLJBG::coeff(int narg, char **arg)
{
  if (narg < 4 || narg > 6)
    error->all(FLERR,"Incorrect args for pair coefficients");

  if (narg == 5)
    error->all(FLERR,"Incorrect args for pair coefficients (you need two cut-offs)");


  if (!allocated) allocate();

  int ilo,ihi,jlo,jhi;
  utils::bounds(FLERR,arg[0],1,atom->ntypes,ilo,ihi,error);
  utils::bounds(FLERR,arg[1],1,atom->ntypes,jlo,jhi,error);

  double epsilon_one = utils::numeric(FLERR,arg[2],false,lmp);
  double sigma_one = utils::numeric(FLERR,arg[3],false,lmp);

  double cutinner = cut_global_in;
   if (narg > 4) cutinner = utils::numeric(FLERR,arg[4],false,lmp);
  double cutouter = cut_global_out;
  if (narg == 6) cutouter = utils::numeric(FLERR,arg[5],false,lmp);


  if(cutinner > cutouter)
         error->all(FLERR,"First cut_off must be smaller than second one in the definition");


  int count = 0;
  for (int i = ilo; i <= ihi; i++) {
    for (int j = MAX(jlo,i); j <= jhi; j++) {
      epsilon[i][j] = epsilon_one;
      sigma[i][j] = sigma_one;
      cut[i][j] = cutouter;
      cut_in[i][j] = cutinner;
      setflag[i][j] = 1;
      count++;
    }
  }

  if (count == 0) error->all(FLERR,"Incorrect args for pair coefficients");



}

/* ----------------------------------------------------------------------
   proc 0 writes to data file
------------------------------------------------------------------------- */

void PairLJBG::write_data(FILE *fp)
{
  for (int i = 1; i <= atom->ntypes; i++)
    fprintf(fp,"%d %g %g\n",i,epsilon[i][i],sigma[i][i]);
}

/* ----------------------------------------------------------------------
   proc 0 writes all pairs to data file
------------------------------------------------------------------------- */

void PairLJBG::write_data_all(FILE *fp)
{
  for (int i = 1; i <= atom->ntypes; i++)
    for (int j = i; j <= atom->ntypes; j++)
      fprintf(fp,"%d %d %g %g %g %g\n",i,j,epsilon[i][j],sigma[i][j],cut[i][j],cut_in[i][j]);
}

/* ----------------------------------------------------------------------
   init for one type pair i,j and corresponding j,i
------------------------------------------------------------------------- */

/*

lj1 = coefficient Force r^-12 first cut-off
lj2 = coefficient Force r^-6 first cut-off
lj3 = coefficient Potential r^-12 first cut-off
lj4 = coefficient Potential r^-6 first cut-off
lj5 = coefficient Force r^-12 second cut-off
lj6 = coefficient Force r^-6 second cut-off
lj7 = coefficient Force r^-2 second cut-off
lj8 = coefficient Potential r^-12 second cut-off
lj9 = coefficient Potential r^-6 second cut-off
lj10 = coefficient Force r^-2 second cut-off

*/

double PairLJBG::init_one(int i, int j)
{

  if (setflag[i][j] == 0) {
    epsilon[i][j] = mix_energy(epsilon[i][i],epsilon[j][j],
                               sigma[i][i],sigma[j][j]);
    sigma[i][j] = mix_distance(sigma[i][i],sigma[j][j]);
    cut[i][j] = mix_distance(cut[i][i],cut[j][j]);
    cut_in[i][j] = mix_distance(cut_in[i][i],cut_in[j][j]);
  }


  lj1[i][j] = 48.0 * epsilon[i][j] * pow(sigma[i][j],12.0);
  lj2[i][j] = 24.0 * epsilon[i][j] * pow(sigma[i][j],6.0);
  lj3[i][j] = 4.0 * epsilon[i][j] * pow(sigma[i][j],12.0);
  lj4[i][j] = 4.0 * epsilon[i][j] * pow(sigma[i][j],6.0);
  lj5[i][j] = 12.0 * C2 * epsilon[i][j] * pow(sigma[i][j],12.0);
  lj6[i][j] =  6.0 * C3 * epsilon[i][j] * pow(sigma[i][j],6.0);
  lj7[i][j] =  2.0 * C4 * epsilon[i][j] * 1./pow(sigma[i][j],2.0);
  lj8[i][j] =  C2 * epsilon[i][j] * pow(sigma[i][j],12.0);
  lj9[i][j] =  C3 * epsilon[i][j] * pow(sigma[i][j],6.0);
  lj10[i][j] = C4 * epsilon[i][j] * 1./pow(sigma[i][j],2.0);
  cutsq_in[i][j] = cut_in[i][j]*cut_in[i][j];


  lj1[j][i]  = lj1[i][j];
  lj2[j][i]  = lj2[i][j];
  lj3[j][i]  = lj3[i][j];
  lj4[j][i]  = lj4[i][j];
  lj5[j][i]  = lj5[i][j];
  lj6[j][i]  = lj6[i][j];
  lj7[j][i]  = lj7[i][j];
  lj8[j][i]  = lj8[i][j];
  lj9[j][i]  = lj9[i][j];
  lj10[j][i] = lj10[i][j];



  cutsq_in[j][i] = cutsq_in[i][j];

  c1[i][j] = C1*epsilon[i][j];
  c1[j][i] = c1[i][j];
  c5[i][j] = C5*epsilon[i][j];
  c5[j][i] = c5[i][j];


  return cut[i][j];
}

/* ----------------------------------------------------------------------
   init specific to this pair style
------------------------------------------------------------------------- */

void PairLJBG::init_style()
{

  // request regular neighbor lists

  int irequest;

    if (update->whichflag == 1 && strstr(update->integrate_style,"respa")) {
        error->all(FLERR,"There is no respa implementation yet");
    }

    irequest = neighbor->request(this,instance_me);
}

/* ----------------------------------------------------------------------
 *   
 *   Single subroutine 
 *
 * ------------------------------------------------------------------------- */


double PairLJBG::single(int i, int j, int itype, int jtype, double rsq,
                         double factor_coul, double factor_lj,
                         double &fforce)
{
  double r2inv,r6inv,forcelj,philj;


  r2inv = 1.0/rsq;
  r6inv = r2inv*r2inv*r2inv;

  if( rsq < cut_global_in){
  	philj = r6inv*(lj3[itype][jtype]*r6inv-lj4[itype][jtype]) + c1[itype][jtype];
	forcelj = r6inv * (lj1[itype][jtype]*r6inv - lj2[itype][jtype]);
  }
  else if(rsq < cut_global_out){
        philj   = r6inv*(lj8[itype][jtype]*r6inv + lj9[itype][jtype]) + lj10[itype][jtype]*rsq + c5[itype][jtype];
        forcelj = r6inv * (lj5[itype][jtype]*r6inv + lj6[itype][jtype]);
	}

  fforce = factor_lj*forcelj*r2inv;

  return factor_lj*philj;
}






















