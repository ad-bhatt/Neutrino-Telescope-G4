#ifndef NTPrimaryGeneratorMessenger_h
#define NTPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

#include "NTPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

class NTPrimaryGeneratorMessenger : public G4UImessenger {
public:
  NTPrimaryGeneratorMessenger(NTPrimaryGeneratorAction* NTGun);
  ~NTPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  NTPrimaryGeneratorAction* NTAction;

  G4UIdirectory*               NTDir;
  G4UIdirectory*               gunDir; 
  
  G4UIcmdWithAnInteger*        genFlagCmd;
  G4UIcmdWithAnInteger*        partIdCmd;
  G4UIcmdWithADoubleAndUnit*   partEnergyCmd;
  G4UIcmdWithADoubleAndUnit*   partEnergySmrCmd;
  G4UIcmdWith3Vector*          partDirectionCmd;
  G4UIcmdWithADoubleAndUnit*   partThetaSmrCmd;
  G4UIcmdWithADoubleAndUnit*   partPhiSmrCmd;
  G4UIcmdWith3VectorAndUnit*   partVertexCmd;
  G4UIcmdWith3VectorAndUnit*   partVertexSmrCmd;

};

#endif

