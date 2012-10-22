
// ToDo:
//	*) 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:solidWorld(0),logicWorld(0),physiWorld(0)
{
  // default parameter values

  ConcreteThick = 20*cm;		// concrete thickness
  CeilingHeight = 300*cm;		// experiment hall ceiling height
  BeamLineHeight = 75.0*cm;		// beam line height above floor

  WorldSizeX = 1000.*cm;		// horizontal (along beam direction)
  WorldSizeY =  500.*cm;		// horizontal (up on screen display)

  BeamCrossXlen = 11.43*cm;		// Beam Cross
  BeamCrossYlen = 27.6*cm;
  BeamCrossZlen = 27.6*cm;
  CrossThickX    = 2.54*cm;
  CrossThickYZ   = 5.0*cm;

/**************************************************************************************/

  // The beam stop length and OD are constrained by
  // len^2 + OD^2 = 58mm^2, where 58mm is the maximum
  // size that can safely fit through a 2.5" port.

  BeamStopLen = 4.5*cm;
  BeamStopOD  = sqrt( pow(58,2) - pow(BeamStopLen,2) );

  //BeamStopHoleLen = 0.01*cm;		// no hole...
  //BeamStopHoleOD = 0.01*cm;
  BeamStopHoleLen = 1.0*cm;
  BeamStopHoleOD = 0.5*cm;

  //G4double PolyThick = 0.1*mm;	// no poly...
  //G4double LeadThick = 0.1*mm;	// no lead...
  G4double PolyThick = 10.0*cm;
  G4double LeadThick =  2.5*cm;

/**************************************************************************************/

  BeamPipeOD    = 5.08*cm;		// Beam Pipe
  BeamPipeID    = 4.78*cm;		// 1.5 mm wall thickness - a guess...
  BeamPipe1Len  = 275.0*cm;		// leads to hypothetical far-upstream-mounted camera box
  BeamPipe2Len  =  50.0*cm;		// between hypothetical near-upstream-mounted camera box and EABT:DB1
  BeamPipe3Len  =  70.0*cm;		// between EABT:DB1 and EABT:DB2
  BeamPipe4Len  = 286.8*cm;		// between EABT:DB2 and EHAT:DB1

  BMagnetOD  = 36.0*cm;			// Bending Magnet
  BMagnetLen = 12.0*cm;

  QMagnetOD  = 32.0*cm;			// Quadrupole Magnets
  QMagnetLen = 10.0*cm;

  CameraPipeOD  =  6.0*cm;		// Camera Box Pipe
  CameraPipeID  =  5.3*cm;
  CameraPipeLen = 27.6*cm;
  GlassThick    =  0.5*cm;

  CameraBoxXlen = 13.0*cm;		// Camera Box
  CameraBoxYlen = 14.2*cm;
  CameraBoxZlen = 48.0*cm;
  CameraBoxThick = 0.6*cm;

  PolyTopThick = PolyThick;		// Poly Shielding
  PolyBottomThick = PolyThick;
  PolyFrontThick = PolyThick;
  PolyBackThick = PolyThick;
  PolyUpThick = PolyThick;
  PolyDownThick = PolyThick;

  LeadTopThick = LeadThick;		// Lead Shielding
  LeadBottomThick = LeadThick;
  LeadFrontThick = LeadThick;
  LeadBackThick = LeadThick;
  LeadUpThick = LeadThick;
  LeadDownThick = LeadThick;

  CameraBoxZoff = 15.0*cm;		// vertical offset from beam line height

  CCDXlen = 8.0*cm;			// CCD sizes
  CCDYlen = 0.5*cm;
  CCDZlen = 8.0*cm;

  CCDX =  0.0*cm;			// CCD placements
  CCDY =  0.0*cm;
  CCDZ = 11.5*cm;			// CCD height relative to centre of camera box

  ComputeCalorParameters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Define materials

  G4String name;
  G4double density;
  G4int nElements;

  G4NistManager* man = G4NistManager::Instance();
  defaultMaterial = man->FindOrBuildMaterial("G4_Galactic");

  G4Material* Air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* Aluminum = man->FindOrBuildMaterial("G4_Al");
  G4Material* Boron = man->FindOrBuildMaterial("G4_B");
  G4Material* Carbon = man->FindOrBuildMaterial("G4_C");
  G4Material* Concrete = man->FindOrBuildMaterial("G4_CONCRETE");
  G4Material* Copper = man->FindOrBuildMaterial("G4_Cu");
  G4Material* Chromium = man->FindOrBuildMaterial("G4_Cr");
  G4Material* Iron = man->FindOrBuildMaterial("G4_Fe");
  G4Material* Lead = man->FindOrBuildMaterial("G4_Pb");
  G4Material* Manganese = man->FindOrBuildMaterial("G4_Mn");
  G4Material* Nickel = man->FindOrBuildMaterial("G4_Ni");
  //G4Material* Nitrogen = man->FindOrBuildMaterial("G4_N");
  G4Material* Phosphorus = man->FindOrBuildMaterial("G4_P");
  G4Material* Polyethylene = man->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* Glass = man->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Material* Silicon = man->FindOrBuildMaterial("G4_Si");
  G4Material* Sulfur = man->FindOrBuildMaterial("G4_S");
  G4Material* Tungsten = man->FindOrBuildMaterial("G4_W");

  // Define Vacuum
  name = "Vacuum";
  density = 1.e-5*g/cm3;
  nElements = 1;
  G4double fAir = 1;
  G4Material* Vacuum = new G4Material(name, density, nElements, kStateGas,
                                      STP_Temperature, 2.e-2*bar);
  Vacuum->AddMaterial(Air,fAir);

  // Define StainlessSteel (314)
  name = "StainlessSteel";
  density = 7.8*g/cm3;
  nElements = 8;
  G4Material* StainlessSteel = new G4Material(name, density, nElements);

  G4double fCr = 0.24;
  G4double fNi = 0.21;
  G4double fC  = 0.0025;
  G4double fMn = 0.02;
  G4double fSi = 0.02;
  G4double fP  = 0.00045;
  G4double fS  = 0.0003;
  G4double fN  = 0.000;
  G4double fFe = 1 - (fCr + fNi + fC + fMn + fSi + fP + fS + fN);

  StainlessSteel->AddMaterial(Chromium,fCr);
  StainlessSteel->AddMaterial(Nickel,fNi);
  StainlessSteel->AddMaterial(Carbon,fC);
  StainlessSteel->AddMaterial(Manganese,fMn);
  StainlessSteel->AddMaterial(Silicon,fSi);
  StainlessSteel->AddMaterial(Phosphorus,fP);
  StainlessSteel->AddMaterial(Sulfur,fS);
  StainlessSteel->AddMaterial(Iron,fFe);

  // Roy's sample of borated polyethylene from Shieldwerx
  // is 5% boron (by mass) 15.98266 in^3 in size, and 0.64 lbs in weight,
  // corresponding to a density of 290.299/261.909 = 1.108 g/cm^3
  //
  name = "BoratedPolyethylene";
  density = 1.108*g/cm3;
  nElements = 2;
  G4double fBoron = 0.05;	// fraction of mass
  G4double fPoly = 0.95;	// fraction of mass
  //G4double fBoron = 0.20;	// fraction of mass
  //G4double fPoly = 0.80;	// fraction of mass
  G4Material* BoratedPolyethylene = new G4Material(name, density, nElements);
  BoratedPolyethylene->AddMaterial(Boron, fBoron);
  BoratedPolyethylene->AddMaterial(Polyethylene, fPoly);

  //
  // Experimental Layout
  //

  //     
  // World - made of concrete...
  //

  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
  logicWorld = new G4LogicalVolume(solidWorld,Concrete,"World");
  physiWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0);

  //
  // Air - inside the concrete walls
  //

  solidAir = new G4Box("Air",WorldSizeX/2-ConcreteThick,WorldSizeY/2-ConcreteThick,WorldSizeZ/2-ConcreteThick);
  logicAir = new G4LogicalVolume(solidAir,Air,"Air");
  physiAir = new G4PVPlacement(0,G4ThreeVector(),logicAir,"Air",logicWorld,false,0);

  //
  // Centre and upstream/downstream Beam Crosses - filled with vacuum
  //

  solidCentreBeamCross = new G4Box("Beam Cross",BeamCrossXlen/2,BeamCrossYlen/2,BeamCrossZlen/2);
  logicCentreBeamCross = new G4LogicalVolume(solidCentreBeamCross,StainlessSteel,"Centre Beam Cross");

  solidBeamCross = new G4Box("Beam Cross",BeamCrossXlen/2,BeamCrossYlen/2,BeamCrossZlen/2);
  logicBeamCross = new G4LogicalVolume(solidBeamCross,StainlessSteel,"Beam Cross");

  InnerCrossXlen = BeamCrossXlen - 2*CrossThickX;
  InnerCrossYlen = BeamCrossYlen - 2*CrossThickYZ;
  InnerCrossZlen = BeamCrossZlen - 2*CrossThickYZ;

  solidCentreInnerCross = new G4Box("Centre Inner Cross",InnerCrossXlen/2,InnerCrossYlen/2,InnerCrossZlen/2);
  logicCentreInnerCross = new G4LogicalVolume(solidCentreInnerCross,Vacuum,"Centre Inner Cross");
  physiCentreInnerCross = new G4PVPlacement(0,G4ThreeVector(),
                              logicCentreInnerCross,"Centre Inner Cross",logicCentreBeamCross,false,0);

  solidInnerCross = new G4Box("Inner Cross",InnerCrossXlen/2,InnerCrossYlen/2,InnerCrossZlen/2);
  logicInnerCross = new G4LogicalVolume(solidInnerCross,Vacuum,"Inner Cross");
  physiInnerCross = new G4PVPlacement(0,G4ThreeVector(),
                        logicInnerCross,"Centre Inner Cross",logicBeamCross,false,0);

  solidCrossPort = new G4Tubs("Cross Port",0,BeamPipeID/2,CrossThickX/2,0,2*pi);
  logicCrossPort = new G4LogicalVolume(solidCrossPort,Vacuum,"Cross Port");

  cprm0.rotateY(0.5*pi);
  CrossPort0X = - (InnerCrossXlen/2 + CrossThickX/2);
  physiCrossPort = new G4PVPlacement(G4Transform3D(cprm0,G4ThreeVector(CrossPort0X,0,0)),
                       logicCrossPort,"Cross Port",logicCentreBeamCross,false,0);
  physiCrossPort = new G4PVPlacement(G4Transform3D(cprm0,G4ThreeVector(CrossPort0X,0,0)),
                       logicCrossPort,"Cross Port",logicBeamCross,false,0);

  cprm1.rotateY(0.5*pi);
  CrossPort1X = InnerCrossXlen/2 + CrossThickX/2;
  physiCrossPort = new G4PVPlacement(G4Transform3D(cprm1,G4ThreeVector(CrossPort1X,0,0)),
                       logicCrossPort,"Cross Port",logicCentreBeamCross,false,1);
  physiCrossPort = new G4PVPlacement(G4Transform3D(cprm1,G4ThreeVector(CrossPort1X,0,0)),
                       logicCrossPort,"Cross Port",logicBeamCross,false,1);

  solidCrossPortGA = new G4Tubs("Cross Port GA",0,CameraPipeID/2,(CrossThickYZ-GlassThick)/2,0,2*pi);
  logicCrossPortGA = new G4LogicalVolume(solidCrossPortGA,Vacuum,"Cross Port");

  cprm2.rotateY(0.5*pi);
  cprm2.rotateZ(0.5*pi);
  CrossPort2Y = BeamCrossYlen/2 - (GlassThick + CrossThickYZ)/2;
  physiCrossPortGA = new G4PVPlacement(G4Transform3D(cprm2,G4ThreeVector(0,CrossPort2Y,0)),
                         logicCrossPortGA,"Cross Port GA",logicCentreBeamCross,false,0);
  physiCrossPortGA = new G4PVPlacement(G4Transform3D(cprm2,G4ThreeVector(0,CrossPort2Y,0)),
                         logicCrossPortGA,"Cross Port GA",logicBeamCross,false,0);

  solidCrossGlassPort = new G4Tubs("Cross Glass Port",0,CameraPipeID/2,GlassThick/2,0,2*pi);
  logicCrossGlassPort = new G4LogicalVolume(solidCrossGlassPort,Glass,"Cross Glass Port");

  CrossPort2Y = BeamCrossYlen/2 - GlassThick/2;
  physiCrossGlassPort = new G4PVPlacement(G4Transform3D(cprm2,G4ThreeVector(0,CrossPort2Y,0)),
                            logicCrossGlassPort,"Cross Glass Port",logicCentreBeamCross,false,0);
  physiCrossGlassPort = new G4PVPlacement(G4Transform3D(cprm2,G4ThreeVector(0,CrossPort2Y,0)),
                            logicCrossGlassPort,"Cross Glass Port",logicBeamCross,false,0);

  //
  // Beam Stop - centre cross only
  //

  solidBeamStop = new G4Tubs("Beam Stop",0,BeamStopOD/2,BeamStopLen/2,0,2*pi);
#if TUNGSTEN==1
  logicBeamStop = new G4LogicalVolume(solidBeamStop,Tungsten,"Beam Stop");
#else
  logicBeamStop = new G4LogicalVolume(solidBeamStop,Copper,"Beam Stop");
#endif

  solidBeamStopHole = new G4Tubs("Beam Stop Hole",0,BeamStopHoleOD/2,BeamStopHoleLen/2,0,2*pi);
  logicBeamStopHole = new G4LogicalVolume(solidBeamStopHole,Vacuum,"Beam Stop Hole");
  Zplace = BeamStopHoleLen/2 - BeamStopLen/2;
  physiBeamStopHole = new G4PVPlacement(0,G4ThreeVector(0,0,Zplace),
                          logicBeamStopHole,"Beam Stop Hole",logicBeamStop,false,0);

  bsrm.rotateY(0.5*pi);
  physiBeamStop = new G4PVPlacement(G4Transform3D(bsrm,G4ThreeVector(0,0,0)),
                      logicBeamStop,"Beam Stop",logicCentreInnerCross,false,0);

  //
  // Beam Pipe 1 - filled with vacuum
  //

  solidBeamPipe1 = new G4Tubs("Beam Pipe 1",0,BeamPipeOD/2,BeamPipe1Len/2,0,2*pi);
  logicBeamPipe1 = new G4LogicalVolume(solidBeamPipe1,StainlessSteel,"Beam Pipe 1");
  
  solidInnerPipe1 = new G4Tubs("Inner Pipe 1",0,BeamPipeID/2,BeamPipe1Len/2,0,2*pi);
  logicInnerPipe1 = new G4LogicalVolume(solidInnerPipe1,Vacuum,"Inner Pipe 1");
  physiInnerPipe1 = new G4PVPlacement(0,G4ThreeVector(),
                        logicInnerPipe1,"Inner Pipe 1",logicBeamPipe1,false,0);

  //
  // Beam Pipe 2 - filled with vacuum
  //

  solidBeamPipe2 = new G4Tubs("Beam Pipe 2",0,BeamPipeOD/2,BeamPipe2Len/2,0,2*pi);
  logicBeamPipe2 = new G4LogicalVolume(solidBeamPipe2,StainlessSteel,"Beam Pipe 2");
  
  solidInnerPipe2 = new G4Tubs("Inner Pipe 2",0,BeamPipeID/2,BeamPipe2Len/2,0,2*pi);
  logicInnerPipe2 = new G4LogicalVolume(solidInnerPipe2,Vacuum,"Inner Pipe 2");
  physiInnerPipe2 = new G4PVPlacement(0,G4ThreeVector(),
                        logicInnerPipe2,"Inner Pipe 2",logicBeamPipe2,false,0);

  //
  // Beam Pipe 3 - filled with vacuum
  //

  solidBeamPipe3 = new G4Tubs("Beam Pipe 3",0,BeamPipeOD/2,BeamPipe3Len/2,0,2*pi);
  logicBeamPipe3 = new G4LogicalVolume(solidBeamPipe3,StainlessSteel,"Beam Pipe 3");
  
  solidInnerPipe3 = new G4Tubs("Inner Pipe 3",0,BeamPipeID/2,BeamPipe3Len/2,0,2*pi);
  logicInnerPipe3 = new G4LogicalVolume(solidInnerPipe3,Vacuum,"Inner Pipe 3");
  physiInnerPipe3 = new G4PVPlacement(0,G4ThreeVector(),
                        logicInnerPipe3,"Inner Pipe 3",logicBeamPipe3,false,0);

  //
  // Beam Pipe 4 - filled with vacuum
  //

  solidBeamPipe4 = new G4Tubs("Beam Pipe 4",0,BeamPipeOD/2,BeamPipe4Len/2,0,2*pi);
  logicBeamPipe4 = new G4LogicalVolume(solidBeamPipe4,StainlessSteel,"Beam Pipe 4");
  
  solidInnerPipe4 = new G4Tubs("Inner Pipe 4",0,BeamPipeID/2,BeamPipe4Len/2,0,2*pi);
  logicInnerPipe4 = new G4LogicalVolume(solidInnerPipe4,Vacuum,"Inner Pipe 4");
  physiInnerPipe4 = new G4PVPlacement(0,G4ThreeVector(),
                        logicInnerPipe4,"Inner Pipe 4",logicBeamPipe4,false,0);

  //
  // Bending Magnet
  //

  BMagnetID = BMagnetLen + sqrt(2)*BeamPipeOD + 1.0*cm;

  solidBMagnet = new G4Tubs("Bending Magnet",BMagnetID/2,BMagnetOD/2,BMagnetLen/2,0,2*pi);
  logicBMagnet = new G4LogicalVolume(solidBMagnet,Iron,"Bending Magnet");

  //
  // Quadrupole Magnets
  //

  QMagnetID = BeamPipeOD + 2.0*cm;

  solidQMagnet = new G4Tubs("Quadrupole Magnet",QMagnetID/2,QMagnetOD/2,QMagnetLen/2,0,2*pi);
  logicQMagnet = new G4LogicalVolume(solidQMagnet,Iron,"Quadrupole Magnet");

  //
  // Camera Pipe - filled with air
  //

  Ylen = CameraPipeLen - (PolyFrontThick + LeadFrontThick);

  solidCameraPipe = new G4Tubs("Camera Pipe",0,CameraPipeOD/2,Ylen/2,0,2*pi);
  logicCameraPipe = new G4LogicalVolume(solidCameraPipe,Aluminum,"Camera Pipe");

  solidInnerCameraPipe = new G4Tubs("Inner Camera Pipe",0,CameraPipeID/2,Ylen/2,0,2*pi);
  logicInnerCameraPipe = new G4LogicalVolume(solidInnerCameraPipe,Air,"Inner Camera Pipe");
  physiInnerCameraPipe = new G4PVPlacement(0,G4ThreeVector(),
                         logicInnerCameraPipe,"Inner Camera Pipe",logicCameraPipe,false,0);
  //
  // Camera Box - lead, poly, aluminum, air
  //

  Xlen = CameraBoxXlen + PolyUpThick + PolyDownThick + LeadUpThick + LeadDownThick;
  Ylen = CameraBoxYlen + PolyFrontThick + PolyBackThick + LeadFrontThick + LeadBackThick;
  Zlen = CameraBoxZlen + PolyTopThick + PolyBottomThick + LeadTopThick + LeadBottomThick;

  solidCameraBox = new G4Box("Camera Box",Xlen/2,Ylen/2,Zlen/2);
  logicCameraBox = new G4LogicalVolume(solidCameraBox,Lead,"Camera Box");

  Xlen = CameraBoxXlen + PolyUpThick + PolyDownThick;
  Ylen = CameraBoxYlen + PolyFrontThick + PolyBackThick;
  Zlen = CameraBoxZlen + PolyTopThick + PolyBottomThick;

  solidPolyBox = new G4Box("Poly Box",Xlen/2,Ylen/2,Zlen/2);
  logicPolyBox = new G4LogicalVolume(solidPolyBox,BoratedPolyethylene,"Poly Box");

  Xplace = (LeadUpThick - LeadDownThick) / 2;
  Yplace = (LeadFrontThick - LeadBackThick) / 2;
  Zplace = (LeadTopThick - LeadBottomThick) / 2;

  physiPolyBox = new G4PVPlacement(0,G4ThreeVector(Xplace,Yplace,Zplace),
                     logicPolyBox,"Poly Box",logicCameraBox,false,0);

  Xlen = CameraBoxXlen;
  Ylen = CameraBoxYlen;
  Zlen = CameraBoxZlen;

  solidAluminumBox = new G4Box("Aluminum Box",Xlen/2,Ylen/2,Zlen/2);
  logicAluminumBox = new G4LogicalVolume(solidAluminumBox,Aluminum,"Aluminum Box");

  Xplace = (PolyUpThick - PolyDownThick) / 2;
  Yplace = (PolyFrontThick - PolyBackThick) / 2;
  Zplace = (PolyTopThick - PolyBottomThick) / 2;

  physiAluminumBox = new G4PVPlacement(0,G4ThreeVector(Xplace,Yplace,Zplace),
                         logicAluminumBox,"Aluminum Box",logicPolyBox,false,0);

  InnerCameraBoxXlen = CameraBoxXlen - 2*CameraBoxThick;
  InnerCameraBoxYlen = CameraBoxYlen - 2*CameraBoxThick;
  InnerCameraBoxZlen = CameraBoxZlen - 2*CameraBoxThick;

  solidInnerCameraBox = new G4Box("Inner Camera Box",InnerCameraBoxXlen/2,InnerCameraBoxYlen/2,InnerCameraBoxZlen/2);
  logicInnerCameraBox = new G4LogicalVolume(solidInnerCameraBox,Air,"Inner Camera Box");

  physiInnerCameraBox = new G4PVPlacement(0,G4ThreeVector(),
                            logicInnerCameraBox,"Inner Camera Box",logicAluminumBox,false,0);

  Ylen = LeadFrontThick;
  lcprm.rotateY(0.5*pi);
  lcprm.rotateZ(0.5*pi);
  Xplace = ( (PolyUpThick+LeadUpThick) - (PolyDownThick+LeadDownThick) )/2;
  Yplace = LeadFrontThick/2 
           - (CameraBoxYlen + LeadFrontThick + PolyFrontThick + LeadBackThick + PolyBackThick)/2;
  Zplace = ( (PolyTopThick+LeadTopThick) - (PolyBottomThick+LeadBottomThick) )/2
           - CameraBoxZoff;

  solidLeadCameraPipe = new G4Tubs("Lead Camera Pipe",0,CameraPipeOD/2,Ylen/2,0,2*pi);
  logicLeadCameraPipe = new G4LogicalVolume(solidLeadCameraPipe,Aluminum,"Lead Camera Pipe");
  physiLeadCameraPipe = new G4PVPlacement(G4Transform3D(lcprm,G4ThreeVector(Xplace,Yplace,Zplace)),
                            logicLeadCameraPipe,"Lead Camera Pipe",logicCameraBox,false,0);

  solidInnerLeadCameraPipe = new G4Tubs("Inner Lead Camera Pipe",0,CameraPipeID/2,Ylen/2,0,2*pi);
  logicInnerLeadCameraPipe = new G4LogicalVolume(solidInnerLeadCameraPipe,Air,"Inner Lead Camera Pipe");
  physiInnerLeadCameraPipe = new G4PVPlacement(0,G4ThreeVector(),
                                 logicInnerLeadCameraPipe,"Inner Lead Camera Pipe",logicLeadCameraPipe,false,0);

  Ylen = PolyFrontThick;
  pcprm.rotateY(0.5*pi);
  pcprm.rotateZ(0.5*pi);
  Xplace = ( (PolyUpThick+LeadUpThick) - (PolyDownThick+LeadDownThick) )/2;
  Yplace = LeadFrontThick + PolyFrontThick/2 
           - (CameraBoxYlen + LeadFrontThick + PolyFrontThick + LeadBackThick + PolyBackThick)/2;
  Zplace = ( (LeadTopThick+PolyTopThick) - (LeadBottomThick+PolyBottomThick) ) / 2
           - CameraBoxZoff;

  solidPolyCameraPipe = new G4Tubs("Poly Camera Pipe",0,CameraPipeOD/2,Ylen/2,0,2*pi);
  logicPolyCameraPipe = new G4LogicalVolume(solidPolyCameraPipe,Aluminum,"Poly Camera Pipe");
  physiPolyCameraPipe = new G4PVPlacement(G4Transform3D(lcprm,G4ThreeVector(Xplace,Yplace,Zplace)),
                            logicPolyCameraPipe,"Poly Camera Pipe",logicPolyBox,false,0);

  solidInnerPolyCameraPipe = new G4Tubs("Inner Poly Camera Pipe",0,CameraPipeID/2,Ylen/2,0,2*pi);
  logicInnerPolyCameraPipe = new G4LogicalVolume(solidInnerPolyCameraPipe,Air,"Inner Poly Camera Pipe");
  physiInnerPolyCameraPipe = new G4PVPlacement(0,G4ThreeVector(),
                                 logicInnerPolyCameraPipe,"Inner Poly Camera Pipe",logicPolyCameraPipe,false,0);

  CameraBoxPortOD = CameraPipeID/2;

  solidCameraBoxPort = new G4Tubs("Camera Box Port",0,CameraBoxPortOD,CameraBoxThick/2,0,2*pi);
  logicCameraBoxPort = new G4LogicalVolume(solidCameraBoxPort,Air,"Camera Box Port");

  cbprm.rotateX(0.5*pi);

  CameraBoxPortX = ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  CameraBoxPortY = ( (LeadFrontThick+PolyFrontThick) - (LeadBackThick+PolyBackThick) ) / 2
                   - (CameraBoxYlen/2 - CameraBoxThick/2);
  CameraBoxPortZ = ( (LeadTopThick+PolyTopThick) - (LeadBottomThick+PolyBottomThick) ) / 2
                   - CameraBoxZoff;

  physiCameraBoxPort = new G4PVPlacement(G4Transform3D(cbprm,G4ThreeVector(CameraBoxPortX,CameraBoxPortY,CameraBoxPortZ)),
                           logicCameraBoxPort,"Camera Box Port",logicAluminumBox,false,0);

  //
  // CCD's
  //

  solidCCD = new G4Box("CCD",CCDXlen/2,CCDYlen/2,CCDZlen/2);
  logicCCD = new G4LogicalVolume(solidCCD,Silicon,"CCD");
  physiCCD = new G4PVPlacement(0,G4ThreeVector(CCDX,CCDY,CCDZ),
                 logicCCD,"CCD",logicInnerCameraBox,false,0);

  //
  // Component Placements
  //

  //
  // centre Beam Cross, Camera Pipe, and Camera Box (CCD0)
  //

  BeamCrossX[0] = 0.0*cm;
  physiCentreBeamCross = new G4PVPlacement(0,G4ThreeVector(BeamCrossX[0],0,BeamLineZ),
                             logicCentreBeamCross,"Centre Beam Cross",logicAir,false,0);

  cprm.rotateY(0.5*pi);
  cprm.rotateZ(0.5*pi);
  CameraPipeY = BeamCrossYlen/2 + CameraPipeLen/2 - (PolyFrontThick + LeadFrontThick)/2;
  physiCameraPipe = new G4PVPlacement(G4Transform3D(cprm,G4ThreeVector(0.,CameraPipeY,BeamLineZ)),
                        logicCameraPipe,"Camera Pipe",logicAir,false,0);

  CameraBoxX = - ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  CameraBoxY = BeamCrossYlen/2 + CameraPipeLen + CameraBoxYlen/2
               - ( (LeadFrontThick+PolyFrontThick) - (LeadBackThick+PolyBackThick) ) / 2;
  CameraBoxZ = BeamLineZ + CameraBoxZoff
               - ( (LeadTopThick+PolyTopThick) - (LeadBottomThick+PolyBottomThick) ) / 2;

  physiCameraBox = new G4PVPlacement(0,G4ThreeVector(CameraBoxX,CameraBoxY,CameraBoxZ),
                       logicCameraBox,"Camera Box",logicAir,false,0);

  //
  // far upstream Beam Cross, Camera Pipe, and Camera Box (CCD1)
  //

  BeamCrossX[1] = - (BeamCrossXlen/2 + BeamPipe1Len + BeamCrossXlen + BeamPipe2Len + BeamCrossXlen/2);
  physiBeamCross = new G4PVPlacement(0,G4ThreeVector(BeamCrossX[1],0,BeamLineZ),
                       logicBeamCross,"Beam Cross",logicAir,false,0);

  CameraPipeX = BeamCrossX[1];
  physiCameraPipe = new G4PVPlacement(G4Transform3D(cprm,G4ThreeVector(CameraPipeX,CameraPipeY,BeamLineZ)),
                        logicCameraPipe,"Camera Pipe",logicAir,false,1);

  CameraBoxX = BeamCrossX[1] - ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  physiCameraBox = new G4PVPlacement(0,G4ThreeVector(CameraBoxX,CameraBoxY,CameraBoxZ),
                       logicCameraBox,"Camera Box",logicAir,false,1);

  //
  // near upstream Beam Cross, Camera Pipe, and Camera Box (CCD2 EABT:DB1)
  //

  BeamCrossX[2] = - (BeamCrossXlen/2 + BeamPipe2Len + BeamCrossXlen/2);
  physiBeamCross = new G4PVPlacement(0,G4ThreeVector(BeamCrossX[2],0,BeamLineZ),
                       logicBeamCross,"Beam Cross",logicAir,false,1);

  CameraPipeX = BeamCrossX[2];
  physiCameraPipe = new G4PVPlacement(G4Transform3D(cprm,G4ThreeVector(CameraPipeX,CameraPipeY,BeamLineZ)),
                        logicCameraPipe,"Camera Pipe",logicAir,false,2);

  CameraBoxX = BeamCrossX[2] - ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  physiCameraBox = new G4PVPlacement(0,G4ThreeVector(CameraBoxX,CameraBoxY,CameraBoxZ),
                       logicCameraBox,"Camera Box",logicAir,false,2);

  //
  // near downstream Beam Cross, Camera Pipe, and Camera Box (CCD3 EABT:DB2)
  //

  BeamCrossX[3] = BeamCrossXlen/2 + BeamPipe3Len + BeamCrossXlen/2;
  physiBeamCross = new G4PVPlacement(0,G4ThreeVector(BeamCrossX[3],0,BeamLineZ),
                       logicBeamCross,"Beam Cross",logicAir,false,2);

  CameraPipeX = BeamCrossX[3];
  physiCameraPipe = new G4PVPlacement(G4Transform3D(cprm,G4ThreeVector(CameraPipeX,CameraPipeY,BeamLineZ)),
                        logicCameraPipe,"Camera Pipe",logicAir,false,3);

  CameraBoxX = BeamCrossX[3] - ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  physiCameraBox = new G4PVPlacement(0,G4ThreeVector(CameraBoxX,CameraBoxY,CameraBoxZ),
                       logicCameraBox,"Camera Box",logicAir,false,3);

  //
  // far downstream Beam Cross, Camera Pipe, and Camera Box (CCD4 EHAT:DB1)
  //

  BeamCrossX[4] = BeamCrossXlen/2 + BeamPipe3Len + BeamCrossXlen + BeamPipe4Len + BeamCrossXlen/2;
  physiBeamCross = new G4PVPlacement(0,G4ThreeVector(BeamCrossX[4],0,BeamLineZ),
                       logicBeamCross,"Beam Cross",logicAir,false,3);

  CameraPipeX = BeamCrossX[4];
  physiCameraPipe = new G4PVPlacement(G4Transform3D(cprm,G4ThreeVector(CameraPipeX,CameraPipeY,BeamLineZ)),
                        logicCameraPipe,"Camera Pipe",logicAir,false,4);

  CameraBoxX = BeamCrossX[4] - ( (LeadUpThick+PolyUpThick) - (LeadDownThick+PolyDownThick) ) / 2;
  physiCameraBox = new G4PVPlacement(0,G4ThreeVector(CameraBoxX,CameraBoxY,CameraBoxZ),
                       logicCameraBox,"Camera Box",logicAir,false,4);

  //
  // far upstream Beam Pipe (1)
  //

  bprm.rotateY(0.5*pi);
  BeamPipeX = - (BeamPipe1Len/2 + BeamCrossXlen + BeamPipe2Len + BeamCrossXlen/2);
  physiBeamPipe1 = new G4PVPlacement(G4Transform3D(bprm,G4ThreeVector(BeamPipeX,0.,BeamLineZ)),
                       logicBeamPipe1,"Beam Pipe 1",logicAir,false,0);

  //
  // near upstream Beam Pipe (2)
  //

  BeamPipeX = - (BeamPipe2Len/2 + BeamCrossXlen/2);
  physiBeamPipe2 = new G4PVPlacement(G4Transform3D(bprm,G4ThreeVector(BeamPipeX,0.,BeamLineZ)),
                       logicBeamPipe2,"Beam Pipe 2",logicAir,false,0);

  //
  // near downstream Beam Pipe (3)
  //

  BeamPipeX = BeamCrossXlen/2 + BeamPipe3Len/2;
  physiBeamPipe3 = new G4PVPlacement(G4Transform3D(bprm,G4ThreeVector(BeamPipeX,0.,BeamLineZ)),
                      logicBeamPipe3,"Beam Pipe 3",logicAir,false,0);

  //
  // far downstream Beam Pipe (4)
  // 

  BeamPipeX = BeamCrossXlen/2 + BeamPipe3Len + BeamCrossXlen + BeamPipe4Len/2;
  physiBeamPipe4 = new G4PVPlacement(G4Transform3D(bprm,G4ThreeVector(BeamPipeX,0.,BeamLineZ)),
                       logicBeamPipe4,"Beam Pipe 4",logicAir,false,0);

  //
  // Magnets
  //

  bmrm.rotateY(0.5*pi);
  bmrm.rotateZ(0.25*pi);

  // bending magnet downstream of EABT:DB1
  Xplace = BeamCrossXlen/2 + 26.2*cm;
  physiBMagnet = new G4PVPlacement(G4Transform3D(bmrm,G4ThreeVector(Xplace,0.,BeamLineZ)),
                      logicBMagnet,"Bending Magnet",logicAir,false,0);

  qmrm.rotateY(0.5*pi);

  // quadrupole magnet upstream of EABT:DB2
  Xplace = BeamCrossXlen/2 + BeamPipe3Len + BeamCrossXlen/2 - 15.0*cm;
  physiQMagnet = new G4PVPlacement(G4Transform3D(qmrm,G4ThreeVector(Xplace,0.,BeamLineZ)),
                     logicQMagnet,"Quadrupole Magnet",logicAir,false,0);

  // quadrupole magnet upstream of EHAT:DB1
  Xplace = BeamCrossXlen/2 + BeamPipe3Len + BeamCrossXlen + BeamPipe4Len + BeamCrossXlen/2 - 15.0*cm;
  physiQMagnet = new G4PVPlacement(G4Transform3D(qmrm,G4ThreeVector(Xplace,0.,BeamLineZ)),
                     logicQMagnet,"Quadrupole Magnet",logicAir,false,1);

  //
  // complete the computation of the experiment setup parameters
  //

  ComputeCalorParameters();

  //                                        
  // Visualization attributes
  //

  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  //logicCalor->SetVisAttributes(simpleBoxVisAtt);

  //
  //always return the physical World
  //

  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
