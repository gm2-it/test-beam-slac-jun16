#ifndef RiderAna_h
#define RiderAna_h

#include "EventCollection.h"
#include <stdio.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TProfile.h>
#include <iostream>
#include <fstream>

class RiderAna : public EventCollection {
 public:
 RiderAna(TTree *tree=0):EventCollection(tree){};
  ~RiderAna() {};
  virtual void Loop(string const &filename);
  void initialize(string const &filename);
  void execute();
  void finalize(string const &filename);
  void useDelete();
  void clear();
  
 private:
  Long64_t nentries;
  
  Double_t out_FC7_ClockCounter;
  Double_t out_pmt_Amplitude;
  Double_t out_pin1_Amplitude;
  Double_t out_pin2_Amplitude;

  TTree *anatree;
};
#endif
