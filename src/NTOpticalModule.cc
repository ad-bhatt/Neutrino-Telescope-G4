
#include "NTOpticalModule.hh"

NTOpticalModule::NTOpticalModule() {

}

NTOpticalModule::NTOpticalModule(double rIn, double rOut) {
  oRadiusInnerOM = rIn;
  oRadiusOuterOM = rOut;
  checkOverlaps = true;
  
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* glassMat = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Material* airMat = nist->FindOrBuildMaterial("G4_AIR");
  
  fOMSolid =
    new G4Sphere("solidOuterOM",0*cm,oRadiusOuterOM,0*deg,360*deg,0*deg,180*deg);
  
  fOMLogicalVol =
    new G4LogicalVolume(fOMSolid,glassMat,"logicOM");
  
  G4Sphere* solidInnerOM =    
    new G4Sphere("solidInnerOM",0*cm,oRadiusInnerOM,0*deg,360*deg,0*deg,180*deg);
  G4LogicalVolume* logicInnerOM =                         
    new G4LogicalVolume(solidInnerOM,airMat,"logicInnerOM");
  new G4PVPlacement(0,G4ThreeVector(),logicInnerOM,"physiInnerOM",
		    fOMLogicalVol,false,0,checkOverlaps);

}

NTOpticalModule::~NTOpticalModule() {

}


