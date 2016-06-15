//
// this code is not correct, according to Kim
// his code is evolving and he will provide us a good example code in the near future
//
#include "RiderAna.h"
#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>

#include <iostream>

void RiderAna::Loop(string const &filename){
  cout << "Loop()" << endl;
  
  if (fChain == 0) return;
  
  //nentries = fChain->GetEntriesFast();
  nentries = fChain->GetEntries();
  cout << "N. entries: " << nentries << endl;

  initialize(filename);
  
  Int_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    clear();
    execute();
  }
  
  finalize(filename);
}

void RiderAna::initialize(string const &filename){

  anatree = new TTree("anatree","antree");
  anatree->Branch("FC7_clockcounter",&out_FC7_ClockCounter, "FC7_clockcounter/D");
  anatree->Branch("pmt_amplitude",   &out_pmt_Amplitude,    "pmt_amplitude/D");
  anatree->Branch("pin1_amplitude",  &out_pin1_Amplitude,   "pin1_amplitude/D");
  anatree->Branch("pin2_amplitude",  &out_pin2_Amplitude,   "pin2_amplitude/D");
  
  cout << "initialize()" << endl;
}

void RiderAna::execute(){
  
  out_FC7_ClockCounter = 25E-9*FC7_ClockCounter->at(0);
  out_pmt_Amplitude = Italiano_Amplitude->at(0);
  out_pin1_Amplitude = Italiano_Amplitude->at(2);
  out_pin2_Amplitude = Italiano_Amplitude->at(4);
  
  anatree->Fill();
  
  // cout << "--> execute(), eventNum: "<< EventNum << endl;
}

void RiderAna::finalize(string const &filename){

  cout << "create  file " << filename << endl;
  TFile *OutF = new TFile(filename.c_str(), "recreate");
  OutF->cd();
  gROOT->GetList()->Write();
  OutF->Close();
  cout << "written file " << filename << endl;
  delete OutF;

  cout << "finalize()" << endl;
}

void RiderAna::useDelete(){
  // cout << "delete()" << endl;
}

void RiderAna::clear(){
  // cout << "clear()" << endl;
}
