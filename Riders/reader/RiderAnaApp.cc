#include "RiderAna.h"
#include <iostream>
#include <fstream>
#include <string>
#include <TString.h>
#include <TCanvas.h>
#include <cstdlib>

int main (int argc, char **argv) {

  TChain *chain = new TChain ("slacAnalyzer/eventTree");
  
  for(int i=1; i < argc; i++) {
    cout << "Adding file: " << argv[i] << endl;
    chain->Add(argv[i]);
  }
  
  RiderAna *ana = new RiderAna(chain);
  
  ana->Loop("RiderPlots.root");
  
  return 0;
}
