-------------------------------------------------------------------

     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                             g4NT
                            --------

This example presently illustrates the basic geometry of Baikal-GVD
like Neutrino Telescope. It uses a scaled down detector geometry 
which is borrowed from Baikal-GVD experiment. The simulation as of now
allows, propogation of muons with standard EM physics and emission of 
cerenkov photons. All the process of cerenkov optical photons are
activated. The optical photons are propogated all over the geometry
and recorded at the optical modules and the information is stored.
There is no PMT response included and the optical modules have 4pi
solid angle view.

The output file is stored in a binary format. The format will be 
included in the respository in future but as of now one can explore
it in the MCEvent class. A reading algorithm for the output datafile
and export it into ASCII, ROOT and HDF5 file format is in the works
but not yet completed. It will be again uploaded as it will be finished.

HOW TO START
------------
 
compile and link to generate an executable

To execute:

$ ./exampleNT -m optMuon.mac -o muTrial11

$ (executable) -m (macro) -o (outputFileName) -t (nThreads) -r (seed)

The -t option is available only in multi-threading mode
and it allows the user to override the Geant4 default number of
threads. The number of threads can be also set via G4FORCENUMBEROFTHREADS
environment variable which has the top priority.

The -m and -o are mandatory options to be provided for the program to run.

Due to heavy RAM requirements, it is not possible to visualize the propagation
of optical photons or muons as the event progressed. The visualization mode is
provided only for visualization of geometry.

To visualize the geometry:

$ ./exampleNT

main()
------

 ==> define Random Number Engine and initial seed

G4Optical Physics
-----------------

 ==> define particles; including *** G4OpticalPhoton     ***
     define processes; including *** G4Cerenkov          ***
                                 *** G4Scintillation     ***
                                 *** G4OpAbsorption      ***
                                 *** G4OpRayleigh        ***
                                 *** G4OpBoundaryProcess ***
 
