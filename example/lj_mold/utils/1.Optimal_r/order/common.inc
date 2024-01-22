cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c	  implicit double precision (a-h,o-z)
      common/accept/cacc,racc,pbacc,tbacc,
     >       biacc,vacc,gcacc,tiacc,rnucacc,
     > dcm(ntipmolmax),dcmcuad(ntipmolmax),contador(ntipmolmax)
      common/acctot/cacctot,racctot,tiacctot,vacctot,cbacctot
     >      ,rnucacctot
      common/atoms/xa(natmax),ya(natmax),za(natmax)
      common/attmpt/atmptcm,atmptro,atmptcb ,atmpttcb,atmptvo,atmpttest
     >             ,atmptgcb,atmptint,atmptnuc
      common/atmptot/atmptcmtot,atmptvotot,atmptrotot,atmptinttot
     >              ,atmptcbtot,atmptnuctot
      common/body/xb(ntipmolmax,nsmax),yb(ntipmolmax,nsmax)
     >           ,zb(ntipmolmax,nsmax)
      common/browniandyn1/ibrownian
      common/browniandyn2/fuerzaextxa(natmax),
     >  fuerzaextya(natmax),fuerzaextza(natmax),deltatiempo
      common /cell_map/ntb,neighbour_cell(ntcell4,ntcell)
      common/cont1/ntot,npr,nmax,neq,njob,kfrec,ntpc,nwr,iwr,npwr,
     >	 iwrmode
      common/cont2/rnpwr
      common /coordsfour/xfour(natmax), yfour(natmax), zfour(natmax)   
      common/coor_temp/xat(nsmax),yat(nsmax),zat(nsmax)
      common/corr/sgcc(500),sgss(500),sgss1(500),sgss2(500)
      common/delta/pshift,ashift,dela,dela1
      common/einstein1/xlan1,xlan2,ueinsold,ifree
      COMMON/einstein2/dxcm,dycm,dzcm,CMX,CMY,CMZ,cmx0,cmy0,cmz0
      common/einstein3/REDX(nmmax),REDY(nmmax),REDZ(nmmax)
      COMMON/einstein4/VSHX(nmmax),VSHY(nmmax),VSHZ(nmmax),
     >                 VRHX(nmmax),VRHY(nmmax),VRHZ(nmmax),
     >                 VSHX_0(nmmax),VSHY_0(nmmax),VSHZ_0(nmmax),
     >                 VRHX_0(nmmax),VRHY_0(nmmax),VRHZ_0(nmmax)
      COMMON/einstein5/ULATT(nmmax),ULATRR(nmmax),ULATRS(nmmax)
      common/einstein6/xcentro(nmmax),ycentro(nmmax),zcentro(nmmax)
      common/einstein7/unkttotet,unktsubet,unkttoterr,unktsuberr,
     >                 unkttoters,unktsubers
      common/einstein8/unktavet,unktavset,unktaverr,unktavserr,
     >                 unktavers,unktavsers
      common/energias/Uintra(ntipmolmax),Uinter(ntipmolmax,ntipmolmax)
      common/enerintra_o/eint_o
      common /ewald1/
     > cssum(mxvct), snsum(mxvct), csdel(mxvct), sndel(mxvct),
     > pewfou, pewslf, rkvec(mxvct),ewalpha
      common /ewald2/ktx(mxvct), kty(mxvct), ktz(mxvct),maxvec

      common/ewvega/alfa,sumaq2,ufold,b1mod,b2mod,b3mod,iewald
      common/ffs1/iffs,ifirstint,nextint,iseed0,irecordcumliq
      common/ffs2/  cumulative(0:nmmax)

      common/fracmv/frac_cm,frac_rot,frac_pcb,frac_tcb,frac_gcb
     >             ,frac_int,frac_cb
      common/geometry1/xnfi,rlstar(ntipmolmax),bangle
     >             ,cutoff,dtrans(ntipmolmax)
      common/geometry2/nsites(ntipmolmax)

      common/globalav/tot,volav,rhoav,UNKTAV,rho2av,soav,hav(3,3),
     >    gnav,rat_cm,rat_ro,rat_pcb,rat_tcb,rat_gcb
     >    ,rat_vo,rat_int,rat_cb,
     >     rhoerr,R1nav,R1nvav,enktav,rosav,sl1av,sl2av,sl3av

        common/histonuc1/Q_P(natmax),q6bartot(natmax)
     > ,q4bartot(natmax),q3bartot(natmax),q2bartot(natmax)

      common/histonuc2/nhistoq6(-100:100),nhistoq4(-100:100),
     > nvecinostot(0:nmmax),nnormal,nhistoclus(0:nmmax),nclusters
     > ,nhistoconex(0:nmmax),nhistoq3(-100:100),nhistoq2(-100:100)

      common/hklbig/maxvecrun
      common/julioLJ/ulrtot
      common/julvir/CONTA(ngrmax,max_pair),R12EU(ngrmax,max_pair)

      common/largo1/ntipmol,nmol(ntipmolmax),puntero1(ntipmolmax),
     1 puntero2(ntipmolmax),puntero3(ntipmolmax),
     1 class(nmmax),sigatom(natmax),epsiatom(natmax),cargatom(natmax)


      common/Lennard_Jones/epsilon(nsisperio),coreduro 
     >              ,sigma(nsisperio),carga(nsisperio),rcut2,U_lj
      common /link_list/head(ntcell),next(natmax),previous(0:natmax)
      common /link_par/xlcell,ylcell,zlcell
      common/ljenergy/u_ljktot,u_ljksub,u_ljkav,u_ljkavs
      common/metaclust/npuntcluster(natmax),ipartclust(natmax)
     >                 ,nclsistema,numpartclust(natmax)
      common/microstate/sl1,sl2,sl3,utot,etot,ngt
      common/mode/iflex,inpt,itest
      common/msqdis/cdx(nmmax),cdy(nmmax),cdz(nmmax)
      common/newatoms/xanew(natmax),yanew(natmax),zanew(natmax)
      common/neworientvect/vxnorr,vynorr,vznorr,vxnors,vynors,vznors
      common/numfile/nfile
      common/nlistal/nxold,nyold,nzold
      common/npt/rho,vol,rhosub,RHOTOT,RHO2TOT,volsub,voltot,gntot,gnsub
     >       ,UNKTSUB,UNKTTOT,enktsub,enkttot,sosub,sotot,R1ntot,R1nvtot
     >       ,rossub,rostot,sl1sub,sl2sub,sl3sub,sl1tot,sl2tot,sl3tot

        common/nucleacion1/ebias,ebiasktot, ebiasksub, ebiaskav, ebiaskavs, 
     > rkbias,rnclcero,sumexpbias,histoclbigunb(0:natmax)
     

      common/nucleacion2/nclbig,inucleacion,icontanuc
     >	,iunbias,iatsolvec(natmax),initialsolmol(natmax),icontsol,
     >  iseisolid(natmax),isolmrco(natmax),
     >	isolfcc(natmax),isolgra(natmax),nhistoclbig(0:nmmax)


      common/orden/nordcar(ntipmolmax,nsmax)
	common/parpotencial/parpot(intidmax,7)
      common/params/pstar,rho_gi,beta,part,pi,pi2,nmoltot,natoms
     >             ,isiteori(ntipmolmax)
      common/patmp/patmp_cm,patmp_rot,patmp_pcb,patmp_tcb,patmp_gcb
     >            ,patmp_int,patmp_cb
      common/primerallamada/ifirstcallxyzto,ifirstcallescri
      common/santander2/nsitestot

      common/savedconf1/volsa,utotsa,u_ljsa,xasa(natmax),yasa(natmax),
     > zasa(natmax),hsa(3,3),hinvsa(3,3),cssumsa(mxvct),snsumsa(mxvct),
     > sideasa,sidebsa,sidecsa,ebiassa,ufoldsa
     
       common/savedconf2/maxvecsa,nclbigsa, ncbsa,indmolclbigsa(natmax)

	common/shape1/sidea,sideb,sidec
	common/shape2/h(3,3),ht(3,3),subh(3,3),toth(3,3),hinv(3,3)
     >  ,hinvt(3,3)
      common/shape3/ctlatt,nx,ny,nz
      common/shape4/ dxt(13),dyt(13),dzt(13)
      common/shape5/ philatt,ilatt
      common/shape6/ iscale
      common/sistema/ nc,nfi,bl,tetain
      common/solidpart/nsolid,lsolid(nmmax),indmolclbig(nmmax),ncb
      common/start/kstart
      common/subav/volavs,rhoavs,UNKTAVS,soavs,havs(3,3),dsqx,dsqy,dsqz,
     >        gnavs,dsq,sideaav,sidebav,sidecav,cosabav,cosacav,cosbcav
     >         ,enktavs,rosavs,sl1avs,sl2avs,sl3avs
	common/umbrella/u_inicial,expotot,exposub,expoav,expoavs,iumbrella
      common/vecinoscl/listavcl(nmmax,20),nvecinoscl(nmmax)

      COMMON/VEGA1a/RMAX,deltar,GSS(ngrmax,max_pair),
     >   gsspro(ngrmax,max_pair),GSSinterna(ntipmolmax,ngrmax)  

      COMMON/VEGA1b/ngrid,IRUN

      COMMON/VEGA2/CONTARO(nrhomax),drho
      COMMON/VEGA3/BOXLMI,sangu1,sangu2,sangu3,sangu4,NEQHAL

	common/widom1a/iwidom,insertions,insperconf
     >              ,iewaldtest 

	common/widom1b/sumebdut,sumq2test

	common/widom2/sigmatest(nattest),epsitest(nattest)
     >             ,cargatest(nattest),coredurotest
	common/widom3/xtest0(nattest),ytest0(nattest),ztest0(nattest),
     >              xvref0(nattest),yvref0(nattest),zvref0(nattest)
	common/widom4/ulongtest,uintratest,selftest
	common/widom5/eperconfav,sumebdutconfav,insexito,insexitoconf
	common/readalpha1/rtimeblock,rtime0,rtimealpha,rporaquiva
        common/readalpha2/ibucle,ileaveout

        common/analizaestas/isolidatt(natmax)
	common/virpre/vir(natmax),virx(natmax),viry(natmax),virz(natmax)
	common/ibu/ibuclefin
