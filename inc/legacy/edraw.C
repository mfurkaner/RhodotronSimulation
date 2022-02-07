{
TCanvas *c1;
TH2D *h2;



c1 = new TCanvas("c1","c1",900,700);
h2 = new TH2D("h2", "", 500, -1000, 1000, 1000, 0, 1.5);

double x,y;

ifstream in;
in.open("efield.dat");
while (1) {
  in >> x >> y ;
  if (!in.good()) break;
  h2->Fill(x,y/1E6);

}

in.close();
h2->Draw("box");

TF1 *tf1= new TF1 ("a1","0.960000/(1+(-200-x)/200)",-750, -190);
tf1->Draw("same");

TF1 *tf3= new TF1 ("a3","0.960000/(1+(x-200)/200)",190,750);
tf3->Draw("same");
/*
TF1 *tf2= new TF1 ("a2","0.960000/(1+(740-x)/200)",190,750);
tf2->SetLineColor(2);
tf2->Draw("same");

TF1 *tf4= new TF1 ("a4","0.960000/(1+(+740+x)/200)",-750, -190);
tf4->SetLineColor(2);
tf4->Draw("same");
*/
}
