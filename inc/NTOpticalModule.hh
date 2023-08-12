#ifndef NTOpticalModule_h
#define NTOpticalModule_h 1

#include <iostream>
#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
using namespace std;
class G4LogicalVolume;
class G4Sphere;

class NTOpticalModule {
public:
  NTOpticalModule();
  NTOpticalModule(double rIn, double rOut);
  ~NTOpticalModule();

  void SetORadiusInnerOM(double xx) {oRadiusInnerOM=xx;};
  void SetORadiusOuterOM(double xx) {oRadiusOuterOM=xx;};
  // void SetX(double xx) {X=xx;};
  
  double GetORadiusInnerOM() {return oRadiusInnerOM;};
  double GetORadiusOuterOM() {return oRadiusOuterOM;};
  // double GetX() {return X;};

  G4LogicalVolume* GetLogicalVolume() {return fOMLogicalVol;};
  
private:
  G4LogicalVolume* fOMLogicalVol;
  G4Sphere* fOMSolid;
  
  // geometrical parameters
  double oRadiusInnerOM;
  double oRadiusOuterOM;
  bool checkOverlaps;
};

#endif

