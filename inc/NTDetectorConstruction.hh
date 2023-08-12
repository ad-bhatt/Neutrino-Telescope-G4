#ifndef NTDetectorConstruction_h
#define NTDetectorConstruction_h 1

#include <iostream>
#include <vector>
#include "globals.hh"
#include "NTConstantGeomPars.hh"
#include "NTOpticalModule.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
using namespace constpar;
using namespace std;

class NTDetectorConstruction : public G4VUserDetectorConstruction {
public:
  NTDetectorConstruction(G4String fname);
  virtual ~NTDetectorConstruction();
  
  public:
  virtual G4VPhysicalVolume* Construct();
  G4LogicalVolume* ConstructSection();
  G4LogicalVolume* ConstructString();
  G4LogicalVolume* ConstructLake();
  void DefineGeometricalParameters();
  void ConstructMaterials();
  void ConstructOM();
  void PrintPar(float* xx, int npt);
  void PrintGeometricalPar();
  void WriteGeometryOutput(G4String parName, float par);
  void WriteGeometryOutput(G4String parName, float* par, int npt);
  void WriteGeometryOutput(G4String parName, int rNum, G4ThreeVector par);

private:
  G4VPhysicalVolume* physiWorld;
  float parOmSection[3];
  float parOmString[3];
  float parLake[3];
  float parWorld[3];
  float oRadiusInnerOM;
  float oRadiusOuterOM;
  float distanceBtwnOM;
  float distToCentralString;
    
  G4Material* matAir;
  G4Material* matWater;
  ofstream fGeomOut;
};

#endif

