#include <iostream>
#include <sstream>
#include "../SIM/fields/fields.h"

#include "TMultiGraph.h"
#include "TGraph.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

int main(){
    ImportedRFField importedField(90);

    importedField.Import("FieldReducer/examples/e.txt_clean.dat");
    importedField.update(0);

    double x,y,z;

    std::cin >> x >> y >> z;

    vector3d pos(x,y,z);

    vector3d field = importedField.getEfield(pos);


    std::cout << field << std::endl;

    return 0;
}


#include "../SIM/fields/fields.cpp"
#include "../SIM/basic/vector.cpp"
#include "../SIM/particles/particle.cpp"

void DrawXZPlaneData(){
    std::cout << "Importing field..." << std::endl;
    ImportedRFField importedField(90);
    importedField.Import("../bin/xz_data");
    importedField.update(0);
    //importedField.SetEmaxInLine(0, 1.0);

    auto vec = importedField.getEmaxPosition();
    auto Emax = importedField.getEfield(vec);
    std::cout << "Field imported Emax at " << vec <<  " |E| = " << Emax.magnitude() << std::endl;

    CoaxialRFField theoreticalField(90);
    theoreticalField.setEmax(1);
    theoreticalField.setR1(0.184);
    theoreticalField.setR2(0.736);

    theoreticalField.update(0);

    std::cout << "Theoretical field set" << std::endl;

    vector3d pos = vector3d(0.5, 0, 0);

    double et = theoreticalField.getEfield(pos).magnitude();
    double ei = importedField.getEfield(pos).magnitude();

    importedField.Multiply(et/ei);

    auto c0 = new TCanvas("c1","multigraph L3",200,10,700,500);

    TMultiGraph* tmg = new TMultiGraph("ivt", "Imported vs Theoretical |E|; z (m); |E| (MV/m)");
    TGraph* th = new TGraph();
    th->SetTitle("Theoretical");
    th->SetMarkerStyle(22);
    th->SetMarkerColor(kRed);


    TGraph* im = new TGraph();
    im->SetTitle("Imported");
    im->SetMarkerStyle(29);
    im->SetMarkerColor(kBlue);

    TGraph* diff = new TGraph();
    diff->SetTitle("#Delta|E| between Eth & Eim");
    diff->SetMarkerStyle(29);
    diff->SetMarkerColor(kRed);

    double step = 2*0.7/1000;

    std::cout << "Filling graphs" << std::endl;


    std::cout << "What radius do you want to draw? r (m) : ";
    double r;
    std::cin >> r;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << r;


    std::string title = "Imported vs Theoretical |E| for r = ";
    title += ss.str();
    title += "m";
    title += "; z (m);  |E| (MV/m)";

    tmg->SetTitle(title.c_str());

    for(double z = -0.02; z <= 0.02;){
        vector3d pos(r , 0, z);

        //pos.rotate(vector3d(0,0,1), 1);

        double t = theoreticalField.getEfield(pos).magnitude();
        double i = importedField.getEfield(pos).magnitude();

        th->AddPoint(z, t);
        im->AddPoint(z, i);
        diff->AddPoint(z, t - i);

        z += 0.001;
    }

    tmg->Add(th, "lp");
    tmg->Add(im, "lp");
    //tmg->Add(diff, "lp");

    tmg->Draw("a");

    auto leg = c0->BuildLegend();
    c0->SetGrid();
    c0->SetFrameLineWidth(3);

    leg->SetTextSize(0.03);
    leg->SetLineWidth(3);
    leg->SetY1(0.82);
    leg->SetY2(0.92);
    leg->SetX1(0.7);
    leg->SetX2(0.98);

    gPad->Modified();
    gPad->Update();
}


void DrawXYPlaneData(){
    std::cout << "Importing field..." << std::endl;
    ImportedRFField importedField(90);
    importedField.Import("../bin/xy_data");
    importedField.update(0);
    importedField.SetEmaxInLine(60, 1.0);


    auto vec = importedField.getEmaxPosition();
    auto Emax = importedField.getEfield(vec);
    std::cout << "Field imported Emax at " << vec <<  " |E| = " << Emax.magnitude() << std::endl;

    CoaxialRFField theoreticalField(90);
    theoreticalField.setEmax(1);
    theoreticalField.setR1(0.184);
    theoreticalField.setR2(0.736);

    theoreticalField.update(0);

    std::cout << "Theoretical field set" << std::endl;

    auto c0 = new TCanvas("c1","multigraph L3",200,10,700,500);

    TMultiGraph* tmg = new TMultiGraph("ivt", "Imported vs Theoretical |E|; r (m); |E| (MV/m)");
    TGraph* th = new TGraph();
    th->SetTitle("Theoretical");
    th->SetMarkerStyle(22);
    th->SetMarkerColor(kRed);


    TGraph* im = new TGraph();
    im->SetTitle("Imported");
    im->SetMarkerStyle(29);
    im->SetMarkerColor(kBlue);

    TGraph* diff = new TGraph();
    diff->SetTitle("#Delta|E| between Eth & Eim");
    diff->SetMarkerStyle(29);
    diff->SetMarkerColor(kRed);

    double step = 2*0.7/1000;

    std::cout << "Filling graphs" << std::endl;


    std::cout << "What angle on the xy plane do you want to draw? angle (degrees) : ";
    double angle;
    std::cin >> angle;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << angle;
    std::string title = "Imported vs Theoretical |E| for #theta = ";
    title += ss.str();
    title += "^{o}";
    title += "; r (m);  |E| (MV/m)";

    tmg->SetTitle(title.c_str());

    for(double x = -0.7; x <= 0.7;){
        vector3d pos(x, 0, 0);
        pos.rotate(vector3d(0,0,1), angle);

        double t = theoreticalField.getEfield(pos).magnitude();
        double i = importedField.getEfield(pos).magnitude();

        th->AddPoint(x, t);
        im->AddPoint(x, i);
        diff->AddPoint(x, t - i);

        x += step;
    }

    //tmg->Add(th, "lp");
    //tmg->Add(im, "lp");
    tmg->Add(diff, "lp");

    tmg->Draw("a");

    auto leg = c0->BuildLegend();
    c0->SetGrid();
    c0->SetFrameLineWidth(3);

    leg->SetTextSize(0.03);
    leg->SetLineWidth(3);
    leg->SetY1(0.82);
    leg->SetY2(0.92);
    leg->SetX1(0.7);
    leg->SetX2(0.98);

    gPad->Modified();
    gPad->Update();
}



void fieldview(){
    DrawXYPlaneData();
}