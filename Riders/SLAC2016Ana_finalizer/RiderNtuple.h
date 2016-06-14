//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun  7 00:28:29 2016 by ROOT version 6.06/04
// from TTree anatree/antree
// found on file: analyzed_gm2slac_run02122.root
//////////////////////////////////////////////////////////

#ifndef RiderNtuple_h
#define RiderNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class RiderNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        FC7_clockcounter;
   Double_t        pmt_amplitude;
   Double_t        pin1_amplitude;
   Double_t        pin2_amplitude;

   // List of branches
   TBranch        *b_FC7_clockcounter;   //!
   TBranch        *b_pmt_amplitude;   //!
   TBranch        *b_pin1_amplitude;   //!
   TBranch        *b_pin2_amplitude;   //!

   RiderNtuple(TTree *tree=0);
   virtual ~RiderNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef RiderNtuple_cxx
RiderNtuple::RiderNtuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("analyzed_gm2slac_run02122.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("analyzed_gm2slac_run02122.root");
      }
      f->GetObject("anatree",tree);

   }
   Init(tree);
}

RiderNtuple::~RiderNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RiderNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RiderNtuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RiderNtuple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("FC7_clockcounter", &FC7_clockcounter, &b_FC7_clockcounter);
   fChain->SetBranchAddress("pmt_amplitude", &pmt_amplitude, &b_pmt_amplitude);
   fChain->SetBranchAddress("pin1_amplitude", &pin1_amplitude, &b_pin1_amplitude);
   fChain->SetBranchAddress("pin2_amplitude", &pin2_amplitude, &b_pin2_amplitude);
   Notify();
}

Bool_t RiderNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RiderNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RiderNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef RiderNtuple_cxx
