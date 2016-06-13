//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 13 04:45:56 2016 by ROOT version 5.34/36
// from TTree ntMonFrame/MonitorFrame
// found on file: ../Run_last/Run41_20160610173537.000.root
//////////////////////////////////////////////////////////

#ifndef NaDaqNtuple_h
#define NaDaqNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NaDaqNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           pmt_NBOF;
   Int_t           pmt_NTimeTrgBOF;
   Int_t           pmt_NtrgBOF;
   Int_t           pmt_BoardAdr;
   Int_t           pmt_boardTemp;
   Int_t           pmt_cspTemp;
   Int_t           pmt_extTemp;
   Int_t           pmt_Vbias;
   Int_t           pmt_ADCVal;
   Int_t           pmt_PulseType;
   Int_t           pmt_t_year;
   Int_t           pmt_t_mon;
   Int_t           pmt_t_day;
   Int_t           pmt_t_secday;
   Int_t           pmt_fired;
   Int_t           pin1_NBOF;
   Int_t           pin1_NTimeTrgBOF;
   Int_t           pin1_NtrgBOF;
   Int_t           pin1_BoardAdr;
   Int_t           pin1_boardTemp;
   Int_t           pin1_cspTemp;
   Int_t           pin1_extTemp;
   Int_t           pin1_Vbias;
   Int_t           pin1_ADCVal;
   Int_t           pin1_PulseType;
   Int_t           pin1_t_year;
   Int_t           pin1_t_mon;
   Int_t           pin1_t_day;
   Int_t           pin1_t_secday;
   Int_t           pin1_fired;
   Int_t           pin2_NBOF;
   Int_t           pin2_NTimeTrgBOF;
   Int_t           pin2_NtrgBOF;
   Int_t           pin2_BoardAdr;
   Int_t           pin2_boardTemp;
   Int_t           pin2_cspTemp;
   Int_t           pin2_extTemp;
   Int_t           pin2_Vbias;
   Int_t           pin2_ADCVal;
   Int_t           pin2_PulseType;
   Int_t           pin2_t_year;
   Int_t           pin2_t_mon;
   Int_t           pin2_t_day;
   Int_t           pin2_t_secday;
   Int_t           pin2_fired;

   // List of branches
   TBranch        *b_pmt;   //!
   TBranch        *b_pin1;   //!
   TBranch        *b_pin2;   //!

   NaDaqNtuple(TTree *tree=0);
   virtual ~NaDaqNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NaDaqNtuple_cxx
NaDaqNtuple::NaDaqNtuple(TTree *tree) : fChain(0) 
{
   Init(tree);
}

NaDaqNtuple::~NaDaqNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NaDaqNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NaDaqNtuple::LoadTree(Long64_t entry)
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

void NaDaqNtuple::Init(TTree *tree)
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

   fChain->SetBranchAddress("pmt", &pmt_NBOF, &b_pmt);
   fChain->SetBranchAddress("pin1", &pin1_NBOF, &b_pin1);
   fChain->SetBranchAddress("pin2", &pin2_NBOF, &b_pin2);
   Notify();
}

Bool_t NaDaqNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NaDaqNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NaDaqNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NaDaqNtuple_cxx
