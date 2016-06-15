#ifndef RiderAna_h
#define RiderAna_h

#include "RiderNtuple.h"
#include <stdio.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TProfile.h>
#include <iostream>
#include <fstream>

using namespace std;

class RiderAna : public RiderNtuple {
 public:
  RiderAna(TTree *tree=0):RiderNtuple(tree){};
  ~RiderAna() {};
  virtual void Loop(string const &filename);
  void initialize(string const &filename);
  void execute();
  void finalize(string const &filename);
  void useDelete();
  void clear();

private:
	Long64_t nentries;
	Int_t iter;

	Double_t *aFC7_clockcounter;
	Double_t *apmt_amplitude;
	Double_t *apin1_amplitude;
	Double_t *apin2_amplitude;

	TProfile *prof_pmt_vs_time;
	TProfile *prof_pin1_vs_time;
	TProfile *prof_pin2_vs_time;

	TProfile *prof_pin1_vs_pin2;
	TProfile *prof_ratio_pins;
	TProfile *prof_sum_pins;

	TH2D *h2_pmt_vs_time;
	TH2D *h2_pin1_vs_time;
	TH2D *h2_pin2_vs_time;

	TH2D *h2_pin1_vs_pin2;
	TH2D *h2_ratio_pins;
	TH2D *h2_sum_pins;

	TH1D *h1_pmt_charge;
	TH1D *h1_pin1_charge;
	TH1D *h1_pin2_charge;
	TH1D *h1_ratio_pins;
	TH1D *h1_sum_pins;
};
#endif
