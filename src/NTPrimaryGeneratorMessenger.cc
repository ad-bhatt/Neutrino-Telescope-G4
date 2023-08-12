#include "NTPrimaryGeneratorMessenger.hh"

NTPrimaryGeneratorMessenger::NTPrimaryGeneratorMessenger(NTPrimaryGeneratorAction* NTGun) : NTAction(NTGun) {
  
  NTDir = new G4UIdirectory("/NT/");
  NTDir->SetGuidance("UI commands of this example");

  gunDir = new G4UIdirectory("/NT/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");

  genFlagCmd = new G4UIcmdWithAnInteger("/NT/gun/genFlag",this);
  genFlagCmd->SetGuidance(" Choice 0(G4 Fixed Vtx), 1(G4 Variable Vtx)");
  genFlagCmd->SetParameterName("genFlag",true, true);
  genFlagCmd->SetDefaultValue(0);
  
  partIdCmd = new G4UIcmdWithAnInteger("/NT/gun/partId",this);
  partIdCmd->SetGuidance("Set incident energy of particle ID (PDG)");
  partIdCmd->SetParameterName("partId",true, true);
  partIdCmd->SetDefaultValue(13);
  
  partEnergyCmd = new G4UIcmdWithADoubleAndUnit("/NT/gun/partEnergy",this);
  partEnergyCmd->SetGuidance("Set incident energy of particle.");
  partEnergyCmd->SetParameterName("partEnergy",true, true);
  partEnergyCmd->SetDefaultUnit("TeV");
  partEnergyCmd->SetDefaultValue(1.);
  partEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV PeV");
  
  partEnergySmrCmd = new G4UIcmdWithADoubleAndUnit("/NT/gun/partEnergySmr",this);
  partEnergySmrCmd->SetGuidance("Set incident energy of particle.");
  partEnergySmrCmd->SetParameterName("partEnergySmr",true, true);
  partEnergySmrCmd->SetDefaultUnit("TeV");
  partEnergySmrCmd->SetDefaultValue(0.1);
  partEnergySmrCmd->SetUnitCandidates("eV keV MeV GeV TeV");

  partDirectionCmd = new G4UIcmdWith3Vector("/NT/gun/partDirection",this);
  partDirectionCmd->SetGuidance("Set the incident direction ");
  partDirectionCmd->SetParameterName("cosx","cosy","cosz",true, true);
  partDirectionCmd->SetDefaultValue(G4ThreeVector(0.,0.,1.));

  partThetaSmrCmd = new G4UIcmdWithADoubleAndUnit("/NT/gun/partThetaSmr",this);
  partThetaSmrCmd->SetGuidance("Set incident energy of particle.");
  partThetaSmrCmd->SetParameterName("partThetaSmr",true, true);
  partThetaSmrCmd->SetDefaultUnit("mrad");
  partThetaSmrCmd->SetDefaultValue(1.0);
  partThetaSmrCmd->SetUnitCandidates("degree rad mrad");

  partPhiSmrCmd = new G4UIcmdWithADoubleAndUnit("/NT/gun/partPhiSmr",this);
  partPhiSmrCmd->SetGuidance("Set incident energy of particle.");
  partPhiSmrCmd->SetParameterName("partPhiSmr",true, true);
  partPhiSmrCmd->SetDefaultUnit("mrad");
  partPhiSmrCmd->SetDefaultValue(1.0);
  partPhiSmrCmd->SetUnitCandidates("degree rad mrad");

  partVertexCmd = new G4UIcmdWith3VectorAndUnit("/NT/gun/partVertex",this);
  partVertexCmd->SetGuidance("Set the incident position ");
  partVertexCmd->SetParameterName("x","y","z",true, true);
  partVertexCmd->SetDefaultValue(G4ThreeVector(21.,21.,0.));
  partVertexCmd->SetDefaultUnit("m");
  partVertexCmd->SetUnitCandidates("microm mm cm m km");

  partVertexSmrCmd = new G4UIcmdWith3VectorAndUnit("/NT/gun/partVertexSmr",this);
  partVertexSmrCmd->SetGuidance("Set the incident position ");
  partVertexSmrCmd->SetParameterName("x","y","z",true, true);
  partVertexSmrCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.));
  partVertexSmrCmd->SetDefaultUnit("m");
  partVertexSmrCmd->SetUnitCandidates("microm mm cm m km");
}

NTPrimaryGeneratorMessenger::~NTPrimaryGeneratorMessenger() {
  if(NTDir) delete NTDir;          
  if(gunDir) delete gunDir;
  if(genFlagCmd) delete genFlagCmd;
  if(partIdCmd) delete  partIdCmd;
  if(partEnergyCmd) delete partEnergyCmd;
  if(partEnergySmrCmd) delete partEnergySmrCmd;
  if(partDirectionCmd) delete partDirectionCmd;
  if(partThetaSmrCmd) delete partThetaSmrCmd;
  if(partPhiSmrCmd) delete partPhiSmrCmd;
  if(partVertexCmd) delete partVertexCmd;
  if(partVertexSmrCmd) delete partVertexSmrCmd;
}

void NTPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) { 
  
  if(command == genFlagCmd)
    NTAction->SetGenFlag(genFlagCmd->GetNewIntValue(newValue));
  
  if(command == partIdCmd)
    NTAction->SetParticleID(partIdCmd->GetNewIntValue(newValue));
  
  if(command == partEnergyCmd)
    NTAction->SetParticleEnergy(partEnergyCmd->GetNewDoubleValue(newValue));

  if(command == partEnergySmrCmd)
    NTAction->SetParticleEnergySmr(partEnergySmrCmd->GetNewDoubleValue(newValue));

  if(command == partDirectionCmd)
    NTAction->SetParticleDirection(partDirectionCmd->GetNew3VectorValue(newValue));

  if(command == partThetaSmrCmd)
    NTAction->SetParticleThetaSmr(partThetaSmrCmd->GetNewDoubleValue(newValue));

  if(command == partPhiSmrCmd)
    NTAction->SetParticlePhiSmr(partPhiSmrCmd->GetNewDoubleValue(newValue));

  if(command == partVertexCmd)
    NTAction->SetParticleVtx(partVertexCmd->GetNew3VectorValue(newValue));

  if(command == partVertexSmrCmd)
    NTAction->SetParticleVtxSmr(partVertexSmrCmd->GetNew3VectorValue(newValue));
}

