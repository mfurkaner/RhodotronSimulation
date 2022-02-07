{
TCanvas *c1;
TH2D *h2;



c1 = new TCanvas("c1","c1",900,700);
h1 = new TH1D("h1", "E1 MeV", 100, 0.4 , 0.5);
h2 = new TH2D("h2", "", 500, -1000, 1000, 1000, 0, 1.5);

double e1,ti,tf;

ifstream in;
in.open("test.dat");
while (1) {
  in >> e1 >> ti >> tf ;
  if (!in.good()) break;
  h1->Fill(e1);
 // h2->Fill(x,y/1E6);

}

in.close();
h1->Draw();

//TF1 *tf1= new TF1 ("a1","0.960000/(1+(-200-x)/200)",-750, -190);
//tf1->Draw("same");

}
