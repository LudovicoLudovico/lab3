#include "graph.hpp"
#include "latex.hpp"
#include "nums.hpp"

void ge() {
  TCanvas* c1 = new TCanvas();

  const int N_POINTS = 15;
  const double TACCHETTE_APPREZZABILI = 0.5;
  const double m_cal = 1.017;
  const double q_cal = 2.635;

  // Oscill (V)
  Double_t oscill[N_POINTS] = {80,  100, 120, 140, 150, 160, 180, 200,
                               220, 240, 260, 280, 300, 320, 340};
  // Mult (A)
  Double_t mult[N_POINTS] = {0.02, 0.03, 0.04, 0.07, 0.09, 0.12, 0.19, 0.28,
                             0.41, 0.59, 0.84, 1.16, 1.58, 2.1,  2.73};
  Double_t e_oscill[N_POINTS] = {
      20,  20,  20,  20,  50,  100, 100, 100,
      100, 100, 100, 100, 100, 100, 100};  // VOLTS/DIV
  Double_t e_mult[N_POINTS] = {};

  Double_t fondoscala[N_POINTS] = {};
  copy(e_oscill, e_oscill + N_POINTS, fondoscala);

  // Calc error
  calc_err("OSCILL", oscill, e_oscill, N_POINTS, 5);
  calc_err("MULT", mult, e_mult, N_POINTS, 5);

  // Gen Latex Table
  vector<const char*> titles = {"Fondo Scala (mV/Div)", "V(mV)", "I(mA)"};
  vector<ValErr> data = {ValErr(fondoscala), ValErr(oscill, e_oscill),
                         ValErr(mult, e_mult)};
  gen_latex_table(titles, data, N_POINTS);

  // Fix Oscilloscope measure
  for (int i = 0; i < N_POINTS; i++) {
    oscill[i] = (oscill[i] - q_cal) / m_cal;
  }

  TGraphErrors* gr = new TGraphErrors(N_POINTS, oscill, mult, e_oscill, e_mult);
  gr->SetTitle("Diodo al Ge");

  gr->SetMarkerStyle(4);
  gr->Draw("AP");
  c1->SetLogy();
  c1->SetGrid();
  gr->GetXaxis()->SetTitle("V(mV)");
  gr->GetYaxis()->SetTitle("I(mA)");

  TF1* f1 = new TF1("f1", "[0]*exp(x/[1])", 70, 200);
  f1->SetParameter(0, 1E-3);
  f1->SetParName(0, "I0");

  f1->SetParameter(1, 80.);
  f1->SetParName(1, "#etaV_{T}");

  gr->Fit("f1", "R");
  gStyle->SetOptFit(1111);
}
