#include "NaDaqAna.h"
#include <iostream>
#include <fstream>
#include <string>
#include <TString.h>
#include <TCanvas.h>
#include <cstdlib>

int main (int argc, char **argv) {
  
  TChain *chain = new TChain ("ntMonFrame");
  
  for(int i=1; i < argc; i++) {
    cout << "Adding file: " << argv[i] << endl;
    chain->Add(argv[i]);
  }

  NaDaqAna *ana = new NaDaqAna(chain);

  ana->Loop("NapoliDaqPlots.root");
  
  exit(0);

}
