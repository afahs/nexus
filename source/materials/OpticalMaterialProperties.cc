// ----------------------------------------------------------------------------
//  $Id$
//
//  Author:  <justo.martin-albo@ific.uv.es>
//           <francesc.monrabal@ific.uv.es>
//           <paola.ferrario@ific.uv.es>
//  Created: 17 August 2009
//
//  Copyright (c) 2009-2011 NEXT Collaboration
// ----------------------------------------------------------------------------

#include "OpticalMaterialProperties.h"
#include "XenonGasProperties.h"
#include "XenonGasProperties.h"
#include "SellmeierEquation.h"
#include <G4MaterialPropertiesTable.hh>

using namespace nexus;



G4MaterialPropertiesTable* OpticalMaterialProperties::Vacuum()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  
  const G4int entries = 2;
  G4double energy[entries] = {0.01*eV, 100.*eV};
  
  // REFRACTIVE INDEX
  G4double rindex[entries] = {1., 1.};
  mpt->AddProperty("RINDEX", energy, rindex, entries);
  
  // ABSORPTION LENGTH
  G4double abslen[entries] = {1.e8*m, 1.e8*m};
  mpt->AddProperty("ABSLENGTH", energy, abslen, entries);
  
  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::Epoxy()
{
  // Optical properties of Epoxy adhesives.
  // Obtained from 
  // http://www.epotek.com/SSCDocs/techtips/Tech%20Tip%2018%20-%20Understanding%20Optical%20Properties%20for%20Epoxy%20Apps.pdf

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  
  G4double energy[2] = 
    {1*eV, 10.751*eV};
  G4double rindex[2] = 
    {1.54, 1.54};
  
  mpt->AddProperty("RINDEX", energy, rindex, 2);

  return mpt;
}



G4MaterialPropertiesTable* OpticalMaterialProperties::FusedSilica()
{
  // Optical properties of Suprasil 311/312(c) synthetic fused silica.
  // Obtained from http://heraeus-quarzglas.com

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX //////////////////////////////////////////////////////////
  // The range is chosen to be up to ~10.7 eV because Sellmeier's equation 
  // for fused silica is valid only in that range
 
  const G4int ri_entries = 200;
  
  G4double ri_energy[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    ri_energy[i] = (1 + i*0.049)*eV;
  }

  // The following values for the refractive index have been calculated
  // using Sellmeier's equation:
  //    n^2 - 1 = B_1 * \lambda^2 / (\lambda^2 - C_1) +
  //            + B_2 * \lambda^2 / (\lambda^2 - C_2) +
  //            + B_3 * \lambda^2 / (\lambda^2 - C_3),
  // with wavelength \lambda in micrometers and 
  //    B_1 = 4.73E-1, B_2 = 6.31E-1, B_3 = 9.06E-1
  //    C_1 = 1.30E-2, C_2 = 4.13E-3, C_3 = 9.88E+1.

  G4double B_1 = 4.73e-1;
  G4double B_2 = 6.31e-1;
  G4double B_3 = 9.06e-1;
  G4double C_1 = 1.30e-2;
  G4double C_2 = 4.13e-3;
  G4double C_3 = 9.88e+1;

  G4double rindex[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
    G4double n2 = 1 + B_1*pow(lambda,2)/(pow(lambda,2)-C_1) 
      + B_2*pow(lambda,2)/(pow(lambda,2)-C_2)
      + B_3*pow(lambda,2)/(pow(lambda,2)-C_3);
    rindex[i] = sqrt(n2);
  }

  mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);

  // ABSORPTION LENGTH /////////////////////////////////////////////////////////

  const G4int abs_entries = 30;
  
  G4double abs_energy[abs_entries]=
    {1.*eV, 6.46499*eV, 6.54*eV, 6.59490*eV, 6.64*eV, 
     6.72714*eV, 6.73828*eV, 6.75*eV, 6.82104*eV, 6.86*eV, 
     6.88*eV, 6.89*eV, 7.*eV, 7.01*eV, 7.01797*eV, 
     7.05*eV, 7.08*eV, 7.08482*eV, 7.30*eV, 7.36*eV, 
     7.4*eV, 7.48*eV, 7.52*eV, 7.58*eV, 7.67440*eV, 
     7.76*eV, 7.89*eV, 7.93*eV, 8.*eV, 10.7*eV};
  G4double abslength[abs_entries] =
    {1500.*cm, 1500.*cm, 200.*cm, 200.*cm, 90.*cm, 
     45.*cm, 45*cm, 30*cm, 24*cm, 21*cm, 
     20*cm , 19*cm, 16*cm, 14.*cm, 13.*cm,
     8.5*cm, 8.*cm, 6.*cm, 1.5*cm, 1.2*cm, 
     1.*cm, .65*cm, .4*cm, .37*cm, .32*cm, 
     .28*cm, .220*cm, .215*cm, .00005*cm, .00005*cm}; 
 
  mpt->AddProperty("ABSLENGTH", abs_energy, abslength, abs_entries);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::GlassEpoxy()
{
  // Optical properties of Optorez 1330 glass epoxy.
  // Obtained from http://refractiveindex.info and http://www.zeonex.com/applications_optical.asp

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX //////////////////////////////////////////////////////////
 
  const G4int ri_entries = 200;
  
  G4double ri_energy[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    ri_energy[i] = (1 + i*0.049)*eV;
  }

  G4double rindex[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
    G4double n2 = 2.291142 - 3.311944E-2*pow(lambda,2) - 1.630099E-2*pow(lambda,-2) + 7.265983E-3*pow(lambda,-4) - 6.806145E-4*pow(lambda,-6) + 1.960732E-5*pow(lambda,-8);
    rindex[i] = sqrt(n2);
  }

  mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);

  // ABSORPTION LENGTH /////////////////////////////////////////////////////////

  const G4int abs_entries = 16;
  
  G4double abs_energy[abs_entries]=
    {1.*eV, 2.132*eV, 2.735*eV, 2.908*eV, 3.119*eV, 
     3.320*eV, 3.476*eV, 3.588*eV, 3.749*eV, 3.869*eV, 
     3.973*eV, 4.120*eV, 4.224*eV, 4.320*eV, 4.420*eV, 
     5.018*eV};
  G4double abslength[abs_entries] =
    {15000.*cm, 326.*mm, 117.68*mm, 85.89*mm, 50.93*mm, 
     31.25*mm, 17.19*mm, 10.46*mm, 5.26*mm, 3.77*mm, 
     2.69*mm, 1.94*mm, 1.33*mm, 0.73*mm, 0.32*mm,
     0.10*mm}; 
 
  mpt->AddProperty("ABSLENGTH", abs_energy, abslength, abs_entries);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::Glass()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX ////////////////////////////////////////////////

  const G4int entries = 10;
  
  G4double energy[entries] = {
    2.58300893*eV, 2.60198171*eV, 2.62289885*eV, 2.66175244*eV, 2.70767479*eV,
    2.80761840*eV, 2.84498460*eV, 3.06361326*eV, 3.40803817*eV, 3.53131384*eV
  };

  G4double rindex[entries] = 
    {1.52283, 1.52309, 1.52339, 1.52395, 1.52461,
     1.52611, 1.52668, 1.53024, 1.53649, 1.53894};
  
  mpt->AddProperty("RINDEX", energy, rindex, entries);

  // ABSORPTION LENGTH ///////////////////////////////////////////////
  
  
  return mpt;
}



G4MaterialPropertiesTable* OpticalMaterialProperties::Sapphire()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX ////////////////////////////////////////////////
  
  G4double um2 = micrometer*micrometer;

  G4double B[3] = {1.4313493, 0.65054713, 5.3414021};
  G4double C[3] = {0.0052799261*um2, 0.0142382647*um2, 325.017834*um2};
  SellmeierEquation seq(B, C);


  G4double wlmin =  150. * nm;
  G4double wlmax = 1000. * nm;
  G4double step  =   10. * nm;

  const G4int entries = G4int((wlmax-wlmin)/step);
  
  G4double energy[entries];
  G4double rindex[entries];
  for (G4int i=0; i<entries; i++) {

    G4double wl = wlmin + i*step;
    energy[i] = h_Planck*c_light/wl;
    rindex[i] = seq.RefractiveIndex(wl);
  }

  /*const G4int ABS_NUMENTRIES = 18;
  G4double Energies[ABS_NUMENTRIES] = {1.000*eV, 1.231*eV, 1.696*eV, 2.045*eV, 2.838*eV, 3.687*eV, 
				       4.276*eV, 4.922*eV, 5.577*eV, 6.150*eV, 6.755*eV, 7.277*eV, 
				       7.687*eV, 7.872*eV, 7.993*eV, 8.186*eV, 8.406*eV, 8.915*eV};
  G4double SAPPHIRE_ABSL[ABS_NUMENTRIES] = {577.50*mm, 577.50*mm, 201.52*mm, 99.51*mm, 59.47*mm, 54.42*mm,
					    45.89*mm, 22.25*mm, 11.97*mm, 7.711*mm, 5.027*mm, 3.689*mm, 
					    2.847*mm, 1.991*mm, 1.230*mm, 0.923*mm, 0.763*mm, 0.664*mm};*/

  const G4int ABS_NUMENTRIES = 19;
  G4double Energies[ABS_NUMENTRIES] = {1.000*eV, 1.296*eV, 1.683*eV, 2.075*eV, 2.585*eV, 3.088*eV,
				       3.709*eV, 4.385*eV, 4.972*eV, 5.608*eV, 6.066*eV, 6.426*eV, 
				       6.806*eV, 7.135*eV, 7.401*eV, 7.637*eV, 7.880*eV, 8.217*eV};
  G4double SAPPHIRE_ABSL[ABS_NUMENTRIES] = {3455.0*mm, 3455.0*mm, 3455.0*mm, 3455.0*mm, 3455.0*mm, 3140.98*mm,
					    2283.30*mm, 1742.11*mm, 437.06*mm, 219.24*mm, 117.773*mm, 80.560*mm, 
					    48.071*mm, 28.805*mm, 17.880*mm, 11.567*mm, 7.718*mm, 4.995*mm};

    /*const G4int ABS_NUMENTRIES = 2;
  G4double Energies[ABS_NUMENTRIES] = {1*eV, 8.217*eV};
  G4double SAPPHIRE_ABSL[ABS_NUMENTRIES] = {150000.*mm, 150000*mm};*/
 
  mpt->AddProperty("RINDEX", energy, rindex, entries);
  mpt->AddProperty("ABSLENGTH", Energies, SAPPHIRE_ABSL, ABS_NUMENTRIES); 

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::GAr()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX ////////////////////////////////////////////////

  const G4int ri_entries = 200;
  
  G4double ri_energy[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    ri_energy[i] = (1 + i*0.049)*eV;
  }

  G4double rindex[ri_entries];

  for (int i=0; i<ri_entries; i++) {
    G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
    rindex[i] = 1 + 0.012055*(0.2075*pow(lambda,2)/(91.012*pow(lambda,2)-1) + 0.0415*pow(lambda,2)/(87.892*pow(lambda,2)-1) + 4.3330*pow(lambda,2)/(214.02*pow(lambda,2)-1)); // From refractiveindex.info
    //    std::cout << "rindex = " << rindex[i] << std::endl;
  }

  mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);

  // EMISSION SPECTRUM ////////////////////////////////////////////////

  // Sampling from ~110 nm to 150 nm <----> from ~11.236 eV to 8.240 eV
  const G4int sc_entries = 380;
  G4double sc_energy[sc_entries];
  G4double intensity[sc_entries];

  G4double Wavelength_peak = 128.*nm;
  G4double Wavelength_sigma = 2.929*nm;
  
  G4double Energy_peak = (h_Planck*c_light/Wavelength_peak);
  G4double Energy_sigma = (h_Planck*c_light*Wavelength_sigma/pow(Wavelength_peak,2));
  
  //  std::cout << "Energy_peak = " << Energy_peak << std::endl;

  for (int j=0;j<sc_entries;j++){
    sc_energy[j] = 8.240*eV + 0.008*j*eV;
    intensity[j] = exp(-pow(Energy_peak/eV-sc_energy[j]/eV,2)/(2*pow(Energy_sigma/eV, 2)))/(Energy_sigma/eV*sqrt(pi*2.));
    //    std::cout << "(energy, intensity) = (" << sc_energy[j] << "," << intensity[j] << ")" << std::endl;
  }

  mpt->AddProperty("ELSPECTRUM", sc_energy, intensity, sc_entries);

  // ABSORTION LENGTH ////////////////////////////////////////////////

  G4double energy[2] = {0.01*eV, 100.*eV};
  G4double abslen[2] = {1.e8*m, 1.e8*m};
  mpt->AddProperty("ABSLENGTH", energy, abslen, 2);

  G4double fano = 0.3;//An argon gas proportional scintillation counter with UV avalanche photodiode scintillation readout C.M.B. Monteiro, J.A.M. Lopes, P.C.P.S. Simoes, J.M.F. dos Santos, C.A.N. Conde 
  mpt->AddConstProperty("RESOLUTIONSCALE",fano);

  mpt->AddConstProperty("ELTIMECONSTANT", 1260.*ns);
  mpt->AddConstProperty("ATTACHMENT", 1000.*ms);
  
  //  G4double sc_yield = 13889/MeV;

  //mpt->AddProperty("FASTCOMPONENT", sc_energy, intensity, sc_entries);
  //mpt->AddProperty("SLOWCOMPONENT", sc_energy, intensity, sc_entries);
  //mpt->AddConstProperty("SCINTILLATIONYIELD", sc_yield);
  //7mpt->AddConstProperty("FASTTIMECONSTANT",7.*ns);
  //mpt->AddConstProperty("SLOWTIMECONSTANT",1260.*ns);
  //mpt->AddConstProperty("YIELDRATIO",.9);
  return mpt; 
}

G4MaterialPropertiesTable* OpticalMaterialProperties::LAr()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  // REFRACTIVE INDEX ////////////////////////////////////////////////

  const G4int ri_entries = 200;
  
  G4double ri_energy[ri_entries];
  for (int i=0; i<ri_entries; i++) {
    ri_energy[i] = (1 + i*0.049)*eV;
  }

  G4double rindex[ri_entries];

  for (int i=0; i<ri_entries; i++) {
    G4double epsilon;
    G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
    if(lambda*1000<110*nm)
      epsilon = 1.0e4;
    else{      
      epsilon = 1.0 / (lambda * lambda); // 1 / (lambda)^2
      epsilon = 1.2055e-2 * ( 0.2075 / (91.012 - epsilon) +
                  0.0415 / (87.892 - epsilon) +
                  4.3330 / (214.02 - epsilon) );
      epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
      G4double LArRho = 1400;
      G4double GArRho = 1.784;
      epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
      if (epsilon < 0.0 || epsilon > 0.999999) 
	epsilon = 4.0e6;
      else
	epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
    }
    rindex[i] = sqrt(epsilon);
  }

  mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);

  // ABSORTION LENGTH ////////////////////////////////////////////////

  G4double energy[2] = {0.01*eV, 100.*eV};
  G4double abslen[2] = {1.e8*m, 1.e8*m};
  mpt->AddProperty("ABSLENGTH", energy, abslen, 2);

  G4double fano = 0.11;// Doke et al, NIM 134 (1976)353
  mpt->AddConstProperty("RESOLUTIONSCALE",fano);


  // EMISSION SPECTRUM ////////////////////////////////////////////////

  // Sampling from ~110 nm to 150 nm <----> from ~11.236 eV to 8.240 eV
  const G4int sc_entries = 500;
  G4double sc_energy[sc_entries];
  G4double intensity[sc_entries];

  G4double Wavelength_peak = 128.*nm;
  G4double Wavelength_sigma = 2.929*nm;
  
  G4double Energy_peak = (h_Planck*c_light/Wavelength_peak);
  G4double Energy_sigma = (h_Planck*c_light*Wavelength_sigma/pow(Wavelength_peak,2));

  for (int j=0;j<sc_entries;j++){
    sc_energy[j] = 8.240*eV + 0.008*j*eV;
    intensity[j] = exp(-pow(Energy_peak/eV-sc_energy[j]/eV,2)/(2*pow(Energy_sigma/eV, 2)))/(Energy_sigma/eV*sqrt(pi*2.));
  }

  mpt->AddProperty("ELSPECTRUM", sc_energy, intensity, sc_entries);

  mpt->AddConstProperty("ELTIMECONSTANT", 1260.*ns);
  mpt->AddConstProperty("ATTACHMENT", 1000.*ms);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::GXe(G4double pressure, 
							  G4double temperature,
							  G4int sc_yield)
{
  XenonGasProperties GXe_prop(pressure, temperature);
  G4MaterialPropertiesTable* GXe_mpt = new G4MaterialPropertiesTable();

  const G4int ri_entries = 8;
  G4double ri_energy[ri_entries] 
    = {1*eV, 2*eV, 3*eV, 4*eV, 5*eV, 6*eV, 7*eV, 8*eV};

  G4double rindex[ri_entries];

  for (G4int i=0; i<ri_entries; i++) {
    rindex[i] = GXe_prop.RefractiveIndex(ri_energy[i]);
    //  G4cout << ri_energy[i] << ", " << rindex[i] << G4endl;
  }

  // Sampling from ~150 nm to 200 nm <----> from 6.20625 eV to 8.20625 eV
  const G4int sc_entries = 500;
  G4double sc_energy[sc_entries];
  for (int j=0;j<sc_entries;j++){
    sc_energy[j] =  6.20625*eV + 0.004*j*eV;
  }
  G4double intensity[sc_entries];
  GXe_prop.Scintillation(sc_entries, sc_energy, intensity);
    
  GXe_mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);
  GXe_mpt->AddProperty("FASTCOMPONENT", sc_energy, intensity, sc_entries);
  GXe_mpt->AddProperty("ELSPECTRUM", sc_energy, intensity, sc_entries);
  GXe_mpt->AddProperty("SLOWCOMPONENT", sc_energy, intensity, sc_entries);
  GXe_mpt->AddConstProperty("SCINTILLATIONYIELD", sc_yield);
  GXe_mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  GXe_mpt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  GXe_mpt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  GXe_mpt->AddConstProperty("ELTIMECONSTANT", 50.*ns);
  GXe_mpt->AddConstProperty("YIELDRATIO",.9);
  GXe_mpt->AddConstProperty("ATTACHMENT", 1000.*ms);
  
  G4double energy[2] = {0.01*eV, 100.*eV};
  G4double abslen[2] = {1.e8*m, 1.e8*m};
  GXe_mpt->AddProperty("ABSLENGTH", energy, abslen, 2);
 
  return GXe_mpt;
}


G4MaterialPropertiesTable* 
OpticalMaterialProperties::FakeGrid(G4double pressure, G4double temperature,
				    G4double transparency, G4double thickness,
				    G4int sc_yield)
{
  XenonGasProperties GXe_prop(pressure, temperature);
  G4MaterialPropertiesTable* FGrid_mpt = new G4MaterialPropertiesTable();

  const G4int ri_entries = 4;
  G4double ri_energy[ri_entries] = {0.01*eV, 1*eV, 10*eV, 100*eV};
  G4double rindex[ri_entries];

  for (G4int i=0; i<ri_entries; i++) {
    rindex[i] = GXe_prop.RefractiveIndex(ri_energy[i]);
  }

  const G4int sc_entries = 500;
  G4double sc_energy[sc_entries];
  for (int j=0;j<sc_entries;j++){
    sc_energy[j] =  6.20625*eV + 0.004*j*eV;
  }
  G4double intensity[sc_entries];
  GXe_prop.Scintillation(sc_entries, sc_energy, intensity);

  G4double abs_length = -thickness/log(transparency);
  const G4int NUMENTRIES  = 2;
  G4double abs_energy[NUMENTRIES] = { .1*eV, 100.*eV };
  G4double ABSL[NUMENTRIES]  = {abs_length, abs_length};

  FGrid_mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);
  FGrid_mpt->AddProperty("ABSLENGTH", abs_energy, ABSL, NUMENTRIES);
  FGrid_mpt->AddProperty("FASTCOMPONENT", sc_energy, intensity, sc_entries);
  FGrid_mpt->AddProperty("SLOWCOMPONENT", sc_energy, intensity, sc_entries);
  FGrid_mpt->AddConstProperty("SCINTILLATIONYIELD", sc_yield); 
  FGrid_mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  FGrid_mpt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  FGrid_mpt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  FGrid_mpt->AddConstProperty("YIELDRATIO",.9);
  FGrid_mpt->AddConstProperty("ATTACHMENT", 1000.*ms);

  return FGrid_mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PTFE()
{
  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  const G4int REFL_NUMENTRIES = 2;

  G4double ENERGIES[REFL_NUMENTRIES] = {1.0*eV, 30.*eV};
  /// This is for non-coated teflon panels
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.72, .72};
  G4cout << "0.72!!!!!!" << G4endl;
  G4double specularlobe[REFL_NUMENTRIES] = {0., 0.}; // specular reflection about the normal to a 
  //microfacet. Such a vector is chosen according to a gaussian distribution with 
  //sigma = SigmaAlhpa (in rad) and centered in the average normal.
  G4double specularspike[REFL_NUMENTRIES] = {0., 0.}; // specular reflection about the average normal 
  G4double backscatter[REFL_NUMENTRIES] = {0., 0.}; //180 degrees reflection
  // 1 - the sum of these three last parameters is the percentage of Lambertian reflection

  teflon_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, REFL_NUMENTRIES);
  teflon_mpt->AddProperty("SPECULARLOBECONSTANT",ENERGIES,specularlobe,REFL_NUMENTRIES);
  teflon_mpt->AddProperty("SPECULARSPIKECONSTANT",ENERGIES,specularspike,REFL_NUMENTRIES);
  teflon_mpt->AddProperty("BACKSCATTERCONSTANT",ENERGIES,backscatter,REFL_NUMENTRIES);

  return teflon_mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PTFE_with_TPB()
{
  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  const G4int REFL_NUMENTRIES = 6;

  G4double ENERGIES[REFL_NUMENTRIES] = {1.0*eV, 2.8*eV, 4.*eV, 6.*eV, 7.2*eV, 30.*eV};
  /// This is for TPB coated teflon panels
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.98, .98, .98, .72, .72, .72};

  G4double ENERGIES_2[2] = {1.0*eV, 30.*eV};
  G4double specularlobe[2] = {0., 0.}; // specular reflection about the normal to a 
  //microfacet. Such a vector is chosen according to a gaussian distribution with 
  //sigma = SigmaAlhpa (in rad) and centered in the average normal.
  G4double specularspike[2] = {0., 0.}; // specular reflection about the average normal 
  G4double backscatter[2] = {0., 0.}; //180 degrees reflection
  // 1 - the sum of these three last parameters is the percentage of Lambertian reflection

  teflon_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, REFL_NUMENTRIES);
  teflon_mpt->AddProperty("SPECULARLOBECONSTANT",ENERGIES_2,specularlobe,2);
  teflon_mpt->AddProperty("SPECULARSPIKECONSTANT",ENERGIES_2,specularspike,2);
  teflon_mpt->AddProperty("BACKSCATTERCONSTANT",ENERGIES_2,backscatter,2);

  return teflon_mpt;
}


G4MaterialPropertiesTable* OpticalMaterialProperties::TPB()
{
  /// This is the simulation of the optical properties of TPB (tetraphenyl butadiene)
  /// a wavelength shifter which allows to converts VUV photons to blue photons.
  /// A WLS material is characterized by its photon absorption and photon emission spectrum
  /// and by a possible time delay between the absorption and re-emission of the photon.

  G4MaterialPropertiesTable* tpb_mpt = new G4MaterialPropertiesTable();
    
  const G4int RIN_NUMENTRIES  = 5;
  const G4int ABSL_NUMENTRIES = 8;
  const G4int WLSABSL_NUMENTRIES = 7;
  const G4int EMISSION_NUMENTRIES = 55;

  G4double Energies[RIN_NUMENTRIES] = {0.1*eV, 1.1*eV, 5.*eV, 10.*eV, 100.*eV};
  G4double TPB_RIND[ABSL_NUMENTRIES]  = {1., 1., 1., 1., 1.};
  

/* Right border of the bins*/
  G4double WLS_Emission_Energies[EMISSION_NUMENTRIES] = {1.96800306364 *eV,  1.98230541148 *eV,  1.99681716413 *eV,  2.01154295443 *eV,  2.0264875529 *eV,  2.04165587291 *eV,  2.05705297602 *eV,  2.07268407766 *eV,  2.08855455299 *eV,  2.10466994306 *eV,  2.12103596128 *eV,  2.13765850016 *eV,  2.15454363839 *eV,  2.17169764825 *eV,  2.18912700337 *eV,  2.2068383869 *eV,  2.2248387 *eV,  2.24313507089 *eV,  2.26173486418 *eV,  2.28064569081 *eV,  2.29987541838 *eV,  2.31943218215 *eV,  2.3393243964 *eV,  2.35956076661 *eV,  2.3801503021 *eV,  2.4011023294 *eV,  2.4224265064 *eV,  2.4441328371 *eV,  2.46623168735 *eV,  2.48873380128 *eV,  2.51165031879 *eV,  2.53499279387 *eV,  2.55877321408 *eV,  2.58300402103 *eV,  2.60769813212 *eV,  2.63286896352 *eV,  2.65853045439 *eV,  2.68469709272 *eV,  2.71138394255 *eV,  2.7386066729 *eV,  2.76638158844 *eV,  2.7947256621 *eV,  2.82365656957 *eV,  2.85319272616 *eV,  2.8833533258 *eV,  2.91415838269 *eV,  2.9456287756 *eV,  2.97778629498 *eV,  3.01065369338 *eV,  3.04425473906 *eV,  3.07861427337 *eV,  3.11375827192 *eV,  3.14971391017 *eV,  3.18650963341 *eV,  3.22417523192 *eV};
;

 G4double TPB_Emission[EMISSION_NUMENTRIES] = {5e-05 , 5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  0.0001 ,  0.0002 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0002 ,  0.0007 ,  0.0007 ,  0.001 ,  0.0011 ,  0.0013 ,  0.0016 ,  0.0018 ,  0.0022 ,  0.0029 ,  0.0035 ,  0.0034 ,  0.0041 ,  0.0046 ,  0.0053 ,  0.0061 ,  0.0069 ,  0.008 ,  0.0087 ,  0.0101 ,  0.0101 ,  0.0103 ,  0.0105 ,  0.0112 ,  0.0117 ,  0.0112 ,  0.011 ,  0.014 ,  0.008 ,  0.008 ,  0.007 ,  0.0038 ,  0.0012 ,  0.0008, 0.0004};

 
 // Values taken from Gehman's paper http://arxiv.org/pdf/1104.3259.pdf
 const G4int wls_numentries = 14;
 G4double WLS_Energies[wls_numentries] = 
   {1.*eV, 2.8*eV, 4.*eV, 4.95937*eV, 5.16601*eV, 5.39062*eV, 
    5.63565*eV, 5.90401*eV, 6.19921*eV, 6.52548*eV, 6.88801*eV, 7.29319*eV, 
    7.74901*eV, 8.26561*eV};
 G4double TPB_ConvEfficiency[wls_numentries] = 
   {0., 0., 0., .86, .90, .94, 
    .90, .80, .75, .70, .75, .82, 
    .85, .92};
 G4cout << "New TPB" << G4endl;

  tpb_mpt->AddProperty("RINDEX", Energies, TPB_RIND, RIN_NUMENTRIES);
  tpb_mpt->AddProperty("WLSCOMPONENT", WLS_Emission_Energies, TPB_Emission, EMISSION_NUMENTRIES);
  tpb_mpt->AddProperty("WLSCONVEFFICIENCY", WLS_Energies, TPB_ConvEfficiency, wls_numentries); 
  tpb_mpt->AddConstProperty("WLSTIMECONSTANT",1.*ns);
    
  return tpb_mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::TPBOld()
{
  /// This is the simulation of the optical properties of TPB (tetraphenyl butadiene)
  /// a wavelength shifter which allows to converts VUV photons to blue photons.
  /// A WLS material is characterized by its photon absorption and photon emission spectrum
  /// and by a possible time delay between the absorption and re-emission of the photon.

  G4MaterialPropertiesTable* tpb_mpt = new G4MaterialPropertiesTable();
    
  const G4int RIN_NUMENTRIES  = 5;
  const G4int ABSL_NUMENTRIES = 8;
  const G4int WLSABSL_NUMENTRIES = 7;
  const G4int EMISSION_NUMENTRIES = 55;

  G4double Energies[RIN_NUMENTRIES] = {0.1*eV, 1.1*eV, 5.*eV, 10.*eV, 100.*eV};
  G4double TPB_RIND[ABSL_NUMENTRIES]  = {1.3, 1.3, 1.3, 1.3, 1.3};

/* Right border of the bins*/
  G4double WLS_Emission_Energies[EMISSION_NUMENTRIES] = {1.96800306364 *eV,  1.98230541148 *eV,  1.99681716413 *eV,  2.01154295443 *eV,  2.0264875529 *eV,  2.04165587291 *eV,  2.05705297602 *eV,  2.07268407766 *eV,  2.08855455299 *eV,  2.10466994306 *eV,  2.12103596128 *eV,  2.13765850016 *eV,  2.15454363839 *eV,  2.17169764825 *eV,  2.18912700337 *eV,  2.2068383869 *eV,  2.2248387 *eV,  2.24313507089 *eV,  2.26173486418 *eV,  2.28064569081 *eV,  2.29987541838 *eV,  2.31943218215 *eV,  2.3393243964 *eV,  2.35956076661 *eV,  2.3801503021 *eV,  2.4011023294 *eV,  2.4224265064 *eV,  2.4441328371 *eV,  2.46623168735 *eV,  2.48873380128 *eV,  2.51165031879 *eV,  2.53499279387 *eV,  2.55877321408 *eV,  2.58300402103 *eV,  2.60769813212 *eV,  2.63286896352 *eV,  2.65853045439 *eV,  2.68469709272 *eV,  2.71138394255 *eV,  2.7386066729 *eV,  2.76638158844 *eV,  2.7947256621 *eV,  2.82365656957 *eV,  2.85319272616 *eV,  2.8833533258 *eV,  2.91415838269 *eV,  2.9456287756 *eV,  2.97778629498 *eV,  3.01065369338 *eV,  3.04425473906 *eV,  3.07861427337 *eV,  3.11375827192 *eV,  3.14971391017 *eV,  3.18650963341 *eV,  3.22417523192 *eV};
;

 G4double TPB_Emission[EMISSION_NUMENTRIES] = {5e-05 , 5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  0.0001 ,  0.0002 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0002 ,  0.0007 ,  0.0007 ,  0.001 ,  0.0011 ,  0.0013 ,  0.0016 ,  0.0018 ,  0.0022 ,  0.0029 ,  0.0035 ,  0.0034 ,  0.0041 ,  0.0046 ,  0.0053 ,  0.0061 ,  0.0069 ,  0.008 ,  0.0087 ,  0.0101 ,  0.0101 ,  0.0103 ,  0.0105 ,  0.0112 ,  0.0117 ,  0.0112 ,  0.011 ,  0.014 ,  0.008 ,  0.008 ,  0.007 ,  0.0038 ,  0.0012 ,  0.0008, 0.0004};

 
 /// Initially, the absorption efficiency (that is, the number of shifted photons/number 
  /// of photons impinguing on the material) was set to 90% for VUV light and 0 for 
  /// any other wavelength, for a thickness of 1 micron of TPB (that is the thickness 
  /// assigned to the light tube coating). 
  /// That means I/I_0 = .1 = exp-(1/lambda) --> lambda = .43 micron. The absorption spectrum 
  /// (WLSABSLENGTH property) is then assigned this value for the whole scintillation range of energies
  /// and no absorption for any other wavelegngth.
  /// The coating of the SiPMs is way thinner (100 nm), thus that efficiency was
  /// too low to have any shifting there. Therefore we set the absorption constant to
  /// .00000043 cm, which gives 100% shifting efficiency to a 100-nm thickness, thus to
  /// the 1-mu one, too.
   G4double WLS_Abs_Energies[WLSABSL_NUMENTRIES] = 
     {0.1*eV, 4.*eV, 6.20625*eV, 7.2*eV, 8.20625*eV, 9.*eV, 100.*eV};
   G4double TPB_WLSABSL[WLSABSL_NUMENTRIES]  = 
     {1000.*cm, 1000.*cm, 0.00000043*cm, 0.00000043*cm, 0.00000043*cm, 1000*cm, 1000.*cm};

   G4cout << "Old TPB" << G4endl;
  tpb_mpt->AddProperty("RINDEX", Energies, TPB_RIND, RIN_NUMENTRIES);
  tpb_mpt->AddProperty("WLSCOMPONENT", WLS_Emission_Energies, TPB_Emission, EMISSION_NUMENTRIES);
  tpb_mpt->AddProperty("WLSABSLENGTH", WLS_Abs_Energies, TPB_WLSABSL,WLSABSL_NUMENTRIES); 
  tpb_mpt->AddConstProperty("WLSTIMECONSTANT",1.*ns);
    
  return tpb_mpt;
}


G4MaterialPropertiesTable* OpticalMaterialProperties::PS()
{
  G4MaterialPropertiesTable* PS_mpt = new G4MaterialPropertiesTable();
     
  const G4int RIN_NUMENTRIES  = 3;
  const G4int Emission_NUMENTRIES = 7;
  const G4int WLS_ABS_NUMENTRIES = 5;
  const G4int ABS_NUMENTRIES = 7;

  //extrapolated from http://refractiveindex.info/?group=PLASTICS&material=PS
     
  G4double Energy[RIN_NUMENTRIES] = {1*eV, 3.1*eV, 7.3*eV};
  G4double PS_RIND[RIN_NUMENTRIES]  = {1.57, 1.62, 1.87};


  //    G4double PS_ABSL[RIN_NUMENTRIES] = {100.*m, 100.*m};

  G4double PS_Emission_Energies[Emission_NUMENTRIES] = {2.5*eV, 3.*eV, 3.7*eV, 3.8*eV, 3.9*eV, 4.*eV, 5.*eV};
  G4double PS_Emission[Emission_NUMENTRIES] = {0., 0., 0., 1., 0., 0., 0.};

  // from Lally et al., Nucl. Instr. and Meth.  Volume 117, Issue 4, 1 October 1996, Pages 421-427 

  G4double PS_WSLABS_Energies[WLS_ABS_NUMENTRIES] = {2.*eV, 7.*eV, 7.2*eV, 7.5*eV, 8*eV};
  //    G4double PS_WLSABS[WLS_ABS_NUMENTRIES] = {1000.*cm, 1000.*cm,  0.014*cm, 1000.*cm, 1000.*cm};  // quantum efficiency of 99.9%
  //    G4double PS_WLSABS[WLS_ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.448142*cm, 1000.*cm, 1000.*cm};  // quantum efficiency of 20% 
  G4double PS_WLSABS[WLS_ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, .0217147*cm, 1000.*cm, 1000.*cm};  // quantum efficiency of 99%
  G4double PS_ABS_Energies[ABS_NUMENTRIES] = {2.5*eV, 3.6*eV, 3.7*eV, 3.8*eV, 3.9*eV, 7.2*eV, 8*eV};
  // G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm,  0.14427*cm, 0.14427*cm,  0.14427*cm, 0.0621335*cm, 1000.*cm}; // absorption of 50%
  //    G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.04343*cm, 0.04343*cm, 0.04343*cm, 0.0621335*cm, 1000.*cm}; // absorption of 90% a 3.8 eV e 80% a 7.2 eV  
  //    G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.621335*cm, 0.621335*cm, 0.621335*cm, .144765*cm, 1000.*cm}; // absorption of 80% (a 10 mm) a 3.8 eV e 99.9% (for 10 mm) a 7.2 eV según Lally 
  G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 1800.*cm,  1800.*cm,  1800.*cm, 0.006*cm, 1000.*cm};// absorption of 85% for 3.8 and 99.9999999 % for 7.2 
  //  G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.04343*cm, 0.04343*cm, 0.04343*cm, 0.140184*cm, 1000.*cm}; // absorption of 90% a 3.8 eV e 51% a 7.2 eV
  //    G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 0.14427*cm, 0.02172*cm, 0.02172*cm, 0.02172*cm, 0.02172*cm}; // absorption of 99% until 3.7*eV, then decreases
  // G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.949122*cm, 1000.*cm, 1000.*cm}; absorption of 10%
  //G4double PS_ABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 9.94992*cm, 1000.*cm, 1000.*cm}; // absorption of 1% 0.448142


  PS_mpt->AddProperty("RINDEX", Energy, PS_RIND, RIN_NUMENTRIES);
  PS_mpt->AddProperty("WLSABSLENGTH", PS_WSLABS_Energies, PS_WLSABS, WLS_ABS_NUMENTRIES); 
  PS_mpt->AddProperty("ABSLENGTH", PS_ABS_Energies, PS_ABS, ABS_NUMENTRIES); 
  PS_mpt->AddProperty("WLSCOMPONENT", PS_Emission_Energies, PS_Emission, Emission_NUMENTRIES);
  PS_mpt->AddConstProperty("WLSTIMECONSTANT",0.5*ns);
    
  //  G4double abs1 = PS_mpt->GetProperty("ABSLENGTH")->GetProperty(7.21*eV);
    
  return PS_mpt;
}



///Properties of Scintillation TPH form 
///http://omlc.ogi.edu/spectra/PhotochemCAD/html/p-terphenyl.html
G4MaterialPropertiesTable* OpticalMaterialProperties::TPH()
{
  G4MaterialPropertiesTable* TPH_mpt = new G4MaterialPropertiesTable();
    
  const G4int SCIN_NUMENTRIES = 7;
  const G4int RIN_NUMENTRIES  = 2;
  const G4int Emission_NUMENTRIES = 7;
  const G4int ABS_NUMENTRIES = 5;

  G4double Energy[RIN_NUMENTRIES] = {0.1*eV, 100.*eV};
  G4double TPH_RIND[RIN_NUMENTRIES]  = {1.65, 1.65};

  //    G4double TPH_ABSL[RIN_NUMENTRIES] = {100.*m, 100.*m};

  G4double TPH_Emission_Energies[Emission_NUMENTRIES] = {2.5*eV, 3.*eV, 3.7*eV, 3.8*eV, 3.9*eV, 4.*eV, 5.*eV};
  G4double TPH_Emission[Emission_NUMENTRIES] = {0., 0., 0., 1., 0., 0., 0.};

  G4double TPH_WSLABS_Energies[ABS_NUMENTRIES] = {2.*eV, 7.*eV, 7.2*eV, 7.5*eV, 8*eV};
  G4double TPH_WLSABS[ABS_NUMENTRIES] = {1000.*cm, 1000.*cm, 0.0177*cm, 1000.*cm, 1000.*cm};


  TPH_mpt->AddProperty("RINDEX", Energy, TPH_RIND, RIN_NUMENTRIES);
  TPH_mpt->AddProperty("WLSABSLENGTH", TPH_WSLABS_Energies, TPH_WLSABS, ABS_NUMENTRIES); 
  //    TPH_mpt->AddProperty("ABSLENGTH", Energy, TPH_ABSL, RIN_NUMENTRIES); 
  TPH_mpt->AddProperty("WLSCOMPONENT", TPH_Emission_Energies, TPH_Emission, Emission_NUMENTRIES);
  TPH_mpt->AddConstProperty("WLSTIMECONSTANT",0.5*ns);

  return TPH_mpt;
}







///Properties of WLS BC480  
///Ref: Optical properties of wavelength shifting panels. P .Soler and Z.H.Wang School ofPhysics, Unioersity of Sydney, Sydney, NSW, 2006, Australia
///Saint Gobain crystals
G4MaterialPropertiesTable* OpticalMaterialProperties::BC480()
{
  G4MaterialPropertiesTable* bc480_mpt = new G4MaterialPropertiesTable();
    
  const G4int SCIN_NUMENTRIES = 7;
  const G4int RIN_NUMENTRIES  = 30;
  const G4int Emission_NUMENTRIES = 5;
  const G4int WLS_ABS_NUMENTRIES = 7;
  const G4int ABS_NUMENTRIES = 9;

  G4double Energy[RIN_NUMENTRIES] =
    {0.1 *eV, 1.5488*eV, 1.5695*eV, 1.6288*eV, 1.7157*eV, 1.7851*eV, 1.8700*eV, 1.9635*eV, 2.0620*eV, 2.1498*eV, 
     2.2540*eV, 2.3911*eV, 2.5645*eV, 2.7564*eV, 2.9396*eV, 3.1210*eV, 3.3965*eV, 3.7095*eV, 3.9230*eV, 4.1526*eV, 
     4.3247*eV, 4.5000*eV, 4.6412*eV, 4.8173*eV, 5.1378*eV, 5.3703*eV, 5.5882*eV, 5.8445*eV, 6.1252*eV, 100.*eV};
  G4double BC480_RIND[RIN_NUMENTRIES]  = 
    {1.45025, 1.45025, 1.45075, 1.45104, 1.45208, 1.45310, 1.45438, 1.45541, 1.45718, 1.45820, 
     1.45972, 1.46198, 1.46500, 1.46875, 1.47250, 1.47624, 1.48247, 1.49018, 1.49614, 1.50285, 
     1.50781, 1.51352, 1.51774, 1.52369, 1.53238, 1.54031, 1.55220, 1.56162, 1.57203, 1.572};
    
  G4double BC480_Emission_Energies[Emission_NUMENTRIES] = {1.5*eV, 2.95*eV, 2.97*eV, 2.99*eV, 8.*eV};
  G4double BC480_Emission[Emission_NUMENTRIES] = {0., 0., 1., 0., 0.};

  G4double BC480_Absorption_Energies[WLS_ABS_NUMENTRIES] = {2.*eV, 3.6*eV, 3.7*eV, 3.8*eV, 3.9*eV, 4.*eV, 8.*eV};
  // G4double BC480_WLSABSL[ABS_NUMENTRIES]  = 
  //   {1000*cm, 1000*cm, 0.1097*cm, 0.1097*cm, 0.1097*cm, 1000*cm};
  G4double BC480_WLSABSL[WLS_ABS_NUMENTRIES]  = 
    {1000*cm, 1000*cm, 1.01912*cm,1.01912*cm,1.01912*cm, 1000*cm, 1000*cm};
  G4double BC480_Abs_Energies[ABS_NUMENTRIES]={1.*eV, 2.*eV, 2.9*eV, 3.4*eV ,3.7*eV, 3.8*eV, 3.9*eV, 4.*eV, 8.*eV};
  //    G4double BC480_ABSL[ABS_NUMENTRIES]={400.*cm, 400.*cm, 400*cm, 400.*cm, 0.259853*cm, 0.259853*cm,0.259853*cm, .01*cm, .001*cm};
  G4double BC480_ABSL[ABS_NUMENTRIES]={1000.*cm, 2500.*cm, 2500*cm, 2500.*cm, 0.237438*cm, 0.237438*cm, 0.237438*cm, .01*cm, .001*cm};

  //    G4cout << "Energia: " << h_Planck*c_light/(900*0.000001) << G4endl;
  // G4cout << "Wavelength per 7.2 eV (mm): " << h_Planck*c_light/(7.2*0.000001) << G4endl;
  // G4cout << "Wavelength per 3.8 eV (mm): " << h_Planck*c_light/(3.8*0.000001) << G4endl;
  // G4cout << "Wavelength per 2.9 eV (mm): " << h_Planck*c_light/(2.9*0.000001) << G4endl;

   
  // for(int i = 0; i<ABSL_NUMENTRIES; i++){
  //   BC480_ABSL[i] = BC480_ABSL[i]*100000.;
  // }

  // for (int i=0;i<ABSL_NUMENTRIES;i++){
  //   G4cout << h_Planck*c_light/BC480_Absorption_Energies[i]*1000000. << ", ";
  // }
  // G4cout << h_Planck*c_light/(417*0.000001) ;
  //  G4cout << G4endl;
  // for (int i=0;i<ABSL_NUMENTRIES;i++){
  //   G4cout << 1/(BC480_WLSABSL[i]*0.1) << ", ";
  // }

  // for (int i=0;i<ABSL_NUMENTRIES;i++){
  //   G4cout << "{" << h_Planck*c_light/BC480_Emission_Energies[i]*1000000 << "," <<  BC480_Emission[i]  << "},";
  // }

    
  bc480_mpt->AddProperty("RINDEX", Energy, BC480_RIND, RIN_NUMENTRIES);
  bc480_mpt->AddProperty("WLSABSLENGTH", BC480_Absorption_Energies, BC480_WLSABSL, WLS_ABS_NUMENTRIES); 
  bc480_mpt->AddProperty("ABSLENGTH",  BC480_Abs_Energies, BC480_ABSL, ABS_NUMENTRIES); 
  bc480_mpt->AddProperty("WLSCOMPONENT", BC480_Emission_Energies, BC480_Emission, Emission_NUMENTRIES);
  bc480_mpt->AddConstProperty("WLSTIMECONSTANT",0.5*ns);

  //  G4double abs = bc480_mpt->GetProperty("RINDEX")->GetProperty(2.9*eV);
  //  G4cout << "R index for 2.9 eV in bicron: " << abs << G4endl;
  return bc480_mpt;
}

