#include "NTPrimaryGeneratorAction.hh"
#include "NTPrimaryGeneratorMessenger.hh"

NTPrimaryGeneratorAction::NTPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), 
   fParticleGun(0) {
  fMessenger = new NTPrimaryGeneratorMessenger(this);
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  initialise = 0;
  Initialize();
  geomRead = false;
  printInfo = 0;
}

NTPrimaryGeneratorAction::~NTPrimaryGeneratorAction() {
  delete fParticleGun;
  delete fMessenger;
}

void NTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  if(geomRead==false) {
    int Geom1 = FindVolumeAndDimensions("physiWorld",parWorld,WorldCenter);
    if(printInfo) G4cout << "Geom1 " << Geom1 << ", WorldCenter:" << WorldCenter << G4endl;
    int Geom2 = FindVolumeAndDimensions("physiLake",parLake,LakeInWorld);
    if(printInfo) G4cout << "Geom2 " << Geom2 << ", LakeInWorld:" << LakeInWorld << G4endl;
    // int Geom3 = FindVolumeAndDimensions("physiDetSurf",parDetSurf,DetSurfInLake);
    // if(printInfo) G4cout << "Geom3 " << Geom3 << ", DetSurfInLake:" << DetSurfInLake << G4endl;

    if(Geom1==2 && Geom2==2) {// && Geom3==2) {
      geomRead = true;
    } else {
      geomRead = false;
      G4ExceptionDescription msg;
      msg << "Geometry Reading Incomplete.\n";
      G4Exception("OpNoviePrimaryGeneratorAction::GeneratePrimaries() geomRead = false;",
		  "GeomRead0001",JustWarning,msg);
    }
  }

  if(geomRead) {
    if(iGenFlag==0) {
      G4cout << "Event started " << G4endl;
      iPartID = 13;
      iPartDirection = G4ThreeVector(0,0,-1);
      iPartEnergy = 1*TeV;
      double vtxX = 5*m;
      double vtxY = 5*m;
      double vtxZ = parLake[2] + LakeInWorld.z();
      iPartVertex = G4ThreeVector(vtxX,vtxY,vtxZ);

      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
      G4String particleName;
      G4ParticleDefinition* particle
	= particleTable->FindParticle(iPartID);

      fParticleGun->SetParticleDefinition(particle);
      fParticleGun->SetParticleMomentumDirection(iPartDirection);
      fParticleGun->SetParticleEnergy(iPartEnergy);  
      fParticleGun->SetParticlePosition(iPartVertex);
      fParticleGun->GeneratePrimaryVertex(anEvent);
    } else if(iGenFlag==1) {
      double vtxZ = -parWorld[2]; //parLake[2] + LakeInWorld.z();
      G4ThreeVector newVtx = iPartVertex;
      newVtx.setZ(vtxZ);
      G4cout << "newVtx = " << G4BestUnit(newVtx,"Length") << G4endl;
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
      G4String particleName;
      G4ParticleDefinition* particle
	= particleTable->FindParticle(iPartID);

      fParticleGun->SetParticleDefinition(particle);
      fParticleGun->SetParticleMomentumDirection(iPartDirection);
      fParticleGun->SetParticleEnergy(iPartEnergy);  
      fParticleGun->SetParticlePosition(newVtx);
      fParticleGun->GeneratePrimaryVertex(anEvent);
    }
  } else {
    G4ExceptionDescription msg;
    msg << "Geometry Reading Problem.\n";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries() geomRead = false;",
		"GeomRead0002",JustWarning,msg);
  }
}

void NTPrimaryGeneratorAction::SetOptPhotonPolar() {
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}

void NTPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle) {
  if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton") {
    G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
      "the particleGun is not an opticalphoton" << G4endl;
    return;
  }
  
  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product*product;
  
  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
  
  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}

void NTPrimaryGeneratorAction::Initialize() {
  iGenFlag = 0;
  iPartID = 13;
  iPartEnergy = 100*GeV;
  iPartDirection = G4ThreeVector(0,0,-1);
  iPartVertex = G4ThreeVector(0,0,0);
  iPartEnergySmr = 0*GeV;
  iPartThetaSmr = 0;
  iPartPhiSmr = 0;
  iPartVertexSmr = G4ThreeVector(0,0,0);
  initialise = 1;
}

int NTPrimaryGeneratorAction::FindVolumeAndDimensions(G4String pVolName, G4double* dim, G4ThreeVector& pos) {
  int volStat = 0;
  G4VPhysicalVolume* PhysVol
    = G4PhysicalVolumeStore::GetInstance()->GetVolume(pVolName);
  if (PhysVol) {
    pos = PhysVol->GetTranslation();
    volStat = 1;
    G4LogicalVolume* LogVol = PhysVol->GetLogicalVolume();
    G4Box* fSolidBox = dynamic_cast<G4Box*>(LogVol->GetSolid());
    if (fSolidBox) {
      dim[0] = fSolidBox->GetXHalfLength();
      dim[1] = fSolidBox->GetYHalfLength();
      dim[2] = fSolidBox->GetZHalfLength();
      volStat = 2;
    } else {
      G4ExceptionDescription msg;
      msg << pVolName << " volume of box shape not found.\n"; 
      msg << "Perhaps you have changed geometry.\n";
      msg << "The gun will not shoot the particle.";
      G4Exception("B1PrimaryGeneratorAction::FindVolumeAndDimensions()",
		  "MyCode0001",JustWarning,msg);
      volStat = -1;
    }
  } else {
    G4ExceptionDescription msg;
    msg << pVolName << " volume not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will not shoot the particle.";
    G4Exception("B1PrimaryGeneratorAction::FindVolumeAndDimensions()",
		"MyCode0002",JustWarning,msg);
    volStat = 0;
  }
  return volStat;
}

