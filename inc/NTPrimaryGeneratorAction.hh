#ifndef NTPrimaryGeneratorAction_h
#define NTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include <iostream>
#include "globals.hh"
#include "Randomize.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
using namespace std;
using namespace CLHEP;
class G4ParticleGun;
class G4Event;
class NTPrimaryGeneratorMessenger;

class NTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  NTPrimaryGeneratorAction();
  virtual ~NTPrimaryGeneratorAction();
  void Initialize();
  public:
  virtual void GeneratePrimaries(G4Event*);

  void SetOptPhotonPolar();
  void SetOptPhotonPolar(G4double);
  
  void SetGenFlag(G4int ij) {iGenFlag = ij;};
  void SetParticleID(G4int ij) {iPartID = ij;};
  void SetParticleEnergy(G4double ij) {iPartEnergy = ij;};
  void SetParticleEnergySmr(G4double ij) {iPartEnergySmr = ij;};
  void SetParticleDirection(G4ThreeVector ij) {iPartDirection = ij;};
  void SetParticleThetaSmr(G4double ij) {iPartThetaSmr = ij;};
  void SetParticlePhiSmr(G4double ij) {iPartPhiSmr = ij;};
  void SetParticleVtx(G4ThreeVector ij) {iPartVertex = ij;};
  void SetParticleVtxSmr(G4ThreeVector ij) {iPartVertexSmr = ij;};
  void SetPrintInfo(G4int ij) {printInfo = ij;};
  
private:
  NTPrimaryGeneratorMessenger* fMessenger;
  int FindVolumeAndDimensions(G4String pVolName, G4double* dim, G4ThreeVector& pos);
  G4ParticleGun* fParticleGun;
  bool geomRead;
  G4int iGenFlag;
  G4int iPartID;
  G4double iPartThetaSmr;
  G4double iPartPhiSmr;
  G4ThreeVector iPartDirection;
  G4double iPartEnergy;
  G4double iPartEnergySmr;
  G4ThreeVector iPartVertex;
  G4ThreeVector iPartVertexSmr;
  int initialise;

  G4double parLake[3];
  G4double parWorld[3];
  G4ThreeVector WorldCenter;
  G4ThreeVector LakeInWorld;
  int printInfo;
};

#endif

