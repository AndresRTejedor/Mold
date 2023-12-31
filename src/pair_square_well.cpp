/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing author: Jorge Ram�rez (jorge.ramirez@upm.es)
------------------------------------------------------------------------- */

#include "pair_square_well.h"
#include "atom.h"
#include "comm.h"
#include "error.h"
#include "force.h"
#include "integrate.h"
#include "math_const.h"
#include "memory.h"
#include "neigh_list.h"
#include "neigh_request.h"
#include "neighbor.h"
#include "respa.h"
#include "update.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace LAMMPS_NS;
using namespace MathConst;

/* ---------------------------------------------------------------------- */

PairPocillos::PairPocillos(LAMMPS *lmp) : Pair(lmp)
{
  respa_enable = 1;
  writedata = 1;
  single_enable = 1;
}

/* ---------------------------------------------------------------------- */

PairPocillos::~PairPocillos()
{
  if (allocated) {
    memory->destroy(setflag);
    memory->destroy(cutsq);

    memory->destroy(cut);
    memory->destroy(z0);
    memory->destroy(dwell);
    memory->destroy(epsilon);
  }
}

/* ---------------------------------------------------------------------- */

void PairPocillos::compute(int eflag, int vflag)
{
  int i, j, ii, jj, inum, jnum, itype, jtype;
  double xtmp, ytmp, ztmp, delx, dely, delz, evdwl, fpair;
  double rsq, r, r2inv, rinv, forceij, factor_ij, tanhij, r2min, r2max;
  int *ilist, *jlist, *numneigh, **firstneigh;

  evdwl = 0.0;
  if (eflag || vflag)
    ev_setup(eflag, vflag);
  else
    evflag = vflag_fdotr = 0;

  double **x = atom->x;
  double **f = atom->f;
  int *type = atom->type;
  int nlocal = atom->nlocal;
  double *special_lj = force->special_lj;
  int newton_pair = force->newton_pair;

  inum = list->inum;
  ilist = list->ilist;
  numneigh = list->numneigh;
  firstneigh = list->firstneigh;

  // loop over neighbors of my atoms

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
      factor_ij = special_lj[sbmask(j)];
      j &= NEIGHMASK;

      delx = xtmp - x[j][0];
      dely = ytmp - x[j][1];
      delz = ztmp - x[j][2];
      rsq = delx * delx + dely * dely + delz * delz;

      jtype = type[j];

      if (rsq < cutsq[itype][jtype]) {

        r2min = (z0[itype][jtype] - 10 * dwell[itype][jtype]);
        r2min *= r2min;
        r2max = (z0[itype][jtype] + 10 * dwell[itype][jtype]);
        r2max *= r2max;
        if (rsq < r2min) {
          // CALCULAR ENERGIA
          evdwl = -epsilon[itype][jtype];
          evdwl *= factor_ij;
        } else if (rsq > r2max)
          continue;
        else {

          r2inv = 1.0 / rsq;
          r = sqrt(rsq);
          rinv = 1.0 / r;
          tanhij = tanh((r - z0[itype][jtype]) / dwell[itype][jtype]);
          forceij =
              -0.5 * epsilon[itype][jtype] * (1 - tanhij * tanhij) * rinv / dwell[itype][jtype];
          fpair = factor_ij * forceij;

          f[i][0] += delx * fpair;
          f[i][1] += dely * fpair;
          f[i][2] += delz * fpair;
          if (newton_pair || j < nlocal) {
            f[j][0] -= delx * fpair;
            f[j][1] -= dely * fpair;
            f[j][2] -= delz * fpair;
          }

          if (eflag) {
            evdwl = -0.5 * epsilon[itype][jtype] * (1 - tanhij);
            evdwl *= factor_ij;
          }
        }

        if (evflag) ev_tally(i, j, nlocal, newton_pair, evdwl, 0.0, fpair, delx, dely, delz);
      }
    }
  }

  if (vflag_fdotr) virial_fdotr_compute();
}

/* ----------------------------------------------------------------------
   allocate all arrays
------------------------------------------------------------------------- */

void PairPocillos::allocate()
{
  allocated = 1;
  int n = atom->ntypes;

  memory->create(setflag, n + 1, n + 1, "pair:setflag");
  for (int i = 1; i <= n; i++)
    for (int j = i; j <= n; j++) setflag[i][j] = 0;

  memory->create(cutsq, n + 1, n + 1, "pair:cutsq");

  memory->create(cut, n + 1, n + 1, "pair:cut");
  memory->create(z0, n + 1, n + 1, "pair:z0");
  memory->create(dwell, n + 1, n + 1, "pair:dwell");
  memory->create(epsilon, n + 1, n + 1, "pair:epsilon");
}

/* ----------------------------------------------------------------------
   global settings
------------------------------------------------------------------------- */

void PairPocillos::settings(int narg, char **arg)
{
  if (narg != 1) error->all(FLERR, "Illegal pair_style command");

  //cut_global = force->numeric(FLERR,arg[0]);
  cut_global = utils::numeric(FLERR, arg[0], false, lmp);

  // reset cutoffs that have been explicitly set

  if (allocated) {
    int i, j;
    for (i = 1; i <= atom->ntypes; i++)
      for (j = i; j <= atom->ntypes; j++)
        if (setflag[i][j]) cut[i][j] = cut_global;
  }
}

/* ----------------------------------------------------------------------
   set coeffs for one or more type pairs
------------------------------------------------------------------------- */

void PairPocillos::coeff(int narg, char **arg)
{
  if (narg < 5 || narg > 6) error->all(FLERR, "Incorrect args for pair coefficients");
  if (!allocated) allocate();

  int ilo, ihi, jlo, jhi;
  // force->bounds(FLERR, arg[0], atom->ntypes, ilo, ihi);
  // force->bounds(FLERR, arg[1], atom->ntypes, jlo, jhi);
  utils::bounds(FLERR, arg[0], 1, atom->ntypes, ilo, ihi, error);
  utils::bounds(FLERR, arg[1], 1, atom->ntypes, jlo, jhi, error);

  // double z0_one = force->numeric(FLERR, arg[2]);
  // double epsilon_one = force->numeric(FLERR, arg[3]);
  // double dwell_one = force->numeric(FLERR, arg[4]);
  double z0_one = utils::numeric(FLERR, arg[2], false, lmp);
  double epsilon_one = utils::numeric(FLERR, arg[3], false, lmp);
  double dwell_one = utils::numeric(FLERR, arg[4], false, lmp);

  double cut_one = cut_global;
  //if (narg == 6) cut_one = force->numeric(FLERR, arg[5]);
  if (narg == 6) cut_one = utils::numeric(FLERR, arg[5], false, lmp);

  int count = 0;
  for (int i = ilo; i <= ihi; i++) {
    for (int j = MAX(jlo, i); j <= jhi; j++) {
      z0[i][j] = z0_one;
      epsilon[i][j] = epsilon_one;
      dwell[i][j] = dwell_one;
      cut[i][j] = cut_one;
      setflag[i][j] = 1;

      z0[j][i] = z0_one;
      epsilon[j][i] = epsilon_one;
      dwell[j][i] = dwell_one;
      cut[j][i] = cut_one;
      setflag[j][i] = 1;

      count++;
    }
  }

  if (count == 0) error->all(FLERR, "Incorrect args for pair coefficients");
}

/* ----------------------------------------------------------------------
   init specific to this pair style
------------------------------------------------------------------------- */

void PairPocillos::init_style()
{
  // request regular or rRESPA neighbor list

  int list_style = NeighConst::REQ_DEFAULT;

  if (update->whichflag == 1 && utils::strmatch(update->integrate_style, "^respa")) {
    auto respa = dynamic_cast<Respa *>(update->integrate);
    if (respa->level_inner >= 0) list_style = NeighConst::REQ_RESPA_INOUT;
    if (respa->level_middle >= 0) list_style = NeighConst::REQ_RESPA_ALL;
  }
  neighbor->add_request(this, list_style);

  // set rRESPA cutoffs

  if (utils::strmatch(update->integrate_style, "^respa") &&
      (dynamic_cast<Respa *>(update->integrate))->level_inner >= 0)
    cut_respa = (dynamic_cast<Respa *>(update->integrate))->cutoff;
  else
    cut_respa = nullptr;
}

/* ----------------------------------------------------------------------
   init for one type pair i,j and corresponding j,i
------------------------------------------------------------------------- */

double PairPocillos::init_one(int i, int j)
{
  if (setflag[i][j] == 0) {
    z0[i][j] = mix_distance(z0[i][i], z0[j][j]);
    dwell[i][j] = mix_distance(dwell[i][i], dwell[j][j]);
    epsilon[i][j] = mix_energy(epsilon[i][i], epsilon[j][j], z0[i][i], z0[j][j]);
    cut[i][j] = mix_distance(cut[i][i], cut[j][j]);
  }

  // check interior rRESPA cutoff

  if (cut_respa && cut[i][j] < cut_respa[3])
    error->all(FLERR, "Pair cutoff < Respa interior cutoff");

  // compute I,J contribution to long-range tail correction
  // count total # of atoms of type I and J via Allreduce

  if (tail_flag) {
    int *type = atom->type;
    int nlocal = atom->nlocal;

    double count[2], all[2];
    count[0] = count[1] = 0.0;
    for (int k = 0; k < nlocal; k++) {
      if (type[k] == i) count[0] += 1.0;
      if (type[k] == j) count[1] += 1.0;
    }
    MPI_Allreduce(count, all, 2, MPI_DOUBLE, MPI_SUM, world);

    //double sig2 = sigma[i][j]*sigma[i][j];
    //double sig6 = sig2*sig2*sig2;
    //double rc3 = cut[i][j]*cut[i][j]*cut[i][j];
    //double rc6 = rc3*rc3;
    //double rc9 = rc3*rc6;
    //etail_ij = 8.0*MY_PI*all[0]*all[1]*epsilon[i][j] *
    //  sig6 * (sig6 - 3.0*rc6) / (9.0*rc9);
    //ptail_ij = 16.0*MY_PI*all[0]*all[1]*epsilon[i][j] *
    //  sig6 * (2.0*sig6 - 3.0*rc6) / (9.0*rc9);
  }

  return cut[i][j];
}

/* ----------------------------------------------------------------------
   proc 0 writes to restart file
------------------------------------------------------------------------- */

void PairPocillos::write_restart(FILE *fp)
{
  write_restart_settings(fp);

  int i, j;
  for (i = 1; i <= atom->ntypes; i++)
    for (j = i; j <= atom->ntypes; j++) {
      fwrite(&setflag[i][j], sizeof(int), 1, fp);
      if (setflag[i][j]) {
        fwrite(&z0[i][j], sizeof(double), 1, fp);
        fwrite(&dwell[i][j], sizeof(double), 1, fp);
        fwrite(&epsilon[i][j], sizeof(double), 1, fp);
        fwrite(&cut[i][j], sizeof(double), 1, fp);
      }
    }
}

/* ----------------------------------------------------------------------
   proc 0 reads from restart file, bcasts
------------------------------------------------------------------------- */

void PairPocillos::read_restart(FILE *fp)
{
  read_restart_settings(fp);
  allocate();

  int i, j;
  int me = comm->me;
  for (i = 1; i <= atom->ntypes; i++)
    for (j = i; j <= atom->ntypes; j++) {
      //if (me == 0) fread(&setflag[i][j], sizeof(int), 1, fp);
      if (me == 0) utils::sfread(FLERR, &setflag[i][j], sizeof(int), 1, fp, nullptr, error);
      MPI_Bcast(&setflag[i][j], 1, MPI_INT, 0, world);
      if (setflag[i][j]) {
        if (me == 0) {
          // fread(&z0[i][j], sizeof(double), 1, fp);
          // fread(&dwell[i][j], sizeof(double), 1, fp);
          // fread(&epsilon[i][j], sizeof(double), 1, fp);
          // fread(&cut[i][j], sizeof(double), 1, fp);
          utils::sfread(FLERR, &z0[i][j], sizeof(double), 1, fp, nullptr, error);
          utils::sfread(FLERR, &dwell[i][j], sizeof(double), 1, fp, nullptr, error);
          utils::sfread(FLERR, &epsilon[i][j], sizeof(double), 1, fp, nullptr, error);
          utils::sfread(FLERR, &cut[i][j], sizeof(double), 1, fp, nullptr, error);
        }
        MPI_Bcast(&z0[i][j], 1, MPI_DOUBLE, 0, world);
        MPI_Bcast(&dwell[i][j], 1, MPI_DOUBLE, 0, world);
        MPI_Bcast(&epsilon[i][j], 1, MPI_DOUBLE, 0, world);
        MPI_Bcast(&cut[i][j], 1, MPI_DOUBLE, 0, world);
      }
    }
}

/* ----------------------------------------------------------------------
   proc 0 writes to restart file
------------------------------------------------------------------------- */

void PairPocillos::write_restart_settings(FILE *fp)
{
  fwrite(&cut_global, sizeof(double), 1, fp);
  //  fwrite(&offset_flag,sizeof(int),1,fp);
  fwrite(&mix_flag, sizeof(int), 1, fp);
  fwrite(&tail_flag, sizeof(int), 1, fp);
}

/* ----------------------------------------------------------------------
   proc 0 reads from restart file, bcasts
------------------------------------------------------------------------- */

void PairPocillos::read_restart_settings(FILE *fp)
{
  int me = comm->me;
  if (me == 0) {
    //fread(&cut_global, sizeof(double), 1, fp);
    utils::sfread(FLERR, &cut_global, sizeof(double), 1, fp, nullptr, error);
    //    fread(&offset_flag,sizeof(int),1,fp);
    //fread(&mix_flag, sizeof(int), 1, fp);
    utils::sfread(FLERR, &mix_flag, sizeof(int), 1, fp, nullptr, error);
    //fread(&tail_flag, sizeof(int), 1, fp);
    utils::sfread(FLERR, &tail_flag, sizeof(int), 1, fp, nullptr, error);
  }
  MPI_Bcast(&cut_global, 1, MPI_DOUBLE, 0, world);
  //  MPI_Bcast(&offset_flag,1,MPI_INT,0,world);
  MPI_Bcast(&mix_flag, 1, MPI_INT, 0, world);
  MPI_Bcast(&tail_flag, 1, MPI_INT, 0, world);
}

/* ----------------------------------------------------------------------
   proc 0 writes to data file
------------------------------------------------------------------------- */

void PairPocillos::write_data(FILE *fp)
{
  for (int i = 1; i <= atom->ntypes; i++) fprintf(fp, "%d %g\n", i, z0[i][i]);
  for (int i = 1; i <= atom->ntypes; i++) fprintf(fp, "%d %g\n", i, dwell[i][i]);
  for (int i = 1; i <= atom->ntypes; i++) fprintf(fp, "%d %g\n", i, epsilon[i][i]);
}

/* ----------------------------------------------------------------------
   proc 0 writes all pairs to data file
------------------------------------------------------------------------- */

void PairPocillos::write_data_all(FILE *fp)
{
  for (int i = 1; i <= atom->ntypes; i++)
    for (int j = i; j <= atom->ntypes; j++)
      fprintf(fp, "%d %d %g %g %g %g\n", i, j, z0[i][j], dwell[i][j], epsilon[i][j], cut[i][j]);
}

/* ---------------------------------------------------------------------- */

double PairPocillos::single(int i, int j, int itype, int jtype, double rsq, double factor_coul,
                            double factor_lj, double &fforce)
{
  double r, r2inv, rinv, tanhij, forceij, philj;
  r2inv = 1.0 / rsq;
  r = sqrt(rsq);
  rinv = 1.0 / r;
  tanhij = tanh((r - z0[itype][jtype]) / dwell[itype][jtype]);
  forceij = -0.5 * epsilon[itype][jtype] * (1 - tanhij * tanhij) * rinv / dwell[itype][jtype];
  //r6inv * lj1[itype][jtype] * r6inv + lj2[itype][jtype] * r*(pow(tanh((r - delta[itype][jtype]) / gamma[itype][jtype]), 2.0) + 1.0);
  fforce = factor_lj * forceij;

  philj = -0.5 * epsilon[itype][jtype] * (1 - tanhij);
  //r6inv*lj3[itype][jtype] * r6inv - lj4[itype][jtype] * (tanh((r - delta[itype][jtype]) / gamma[itype][jtype]) - 1.0);
  return factor_lj * philj;
}

/* ---------------------------------------------------------------------- */

void *PairPocillos::extract(const char *str, int &dim)
{
  dim = 2;
  if (strcmp(str, "z0") == 0) return (void *) z0;
  if (strcmp(str, "dwell") == 0) return (void *) dwell;
  if (strcmp(str, "epsilon") == 0) return (void *) epsilon;
  return NULL;
}
