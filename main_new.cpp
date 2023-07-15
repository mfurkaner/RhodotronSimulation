#include "inc/simulation.h"
#include <chrono>
#include <sys/time.h>
#include <sys/resource.h>

int getrusage(int who, struct rusage *usage);
void plot(double);
std::pair<vector<double>, vector<double>> test_out_mag_rk_lf_dt_dE(vector<double> dts);
std::pair<vector<double>, vector<double>> test_out_staticE_rk_lf_dt_dE(vector<double> dts);

void TEST_MAG_LF_RK();



using namespace std::chrono;

void TEST_STATIC_E_LF_RK(){

    vector<double> dts;

    dts.push_back(5e-2);
    dts.push_back(1e-2);
    dts.push_back(1e-3);

    dts.push_back(9e-4);
    dts.push_back(8e-4);
    dts.push_back(7e-4);
    dts.push_back(6e-4);
    dts.push_back(5e-4);
    dts.push_back(4e-4);
    dts.push_back(3e-4);
    dts.push_back(2e-4);
    dts.push_back(1e-4);

    dts.push_back(9e-5);
    dts.push_back(8e-5);
    dts.push_back(7e-5);
    dts.push_back(6e-5);
    dts.push_back(5e-5);
    dts.push_back(4e-5);
    dts.push_back(3e-5);
    dts.push_back(2e-5);
    dts.push_back(1e-5);

    dts.push_back(1e-6);


    auto results = test_out_staticE_rk_lf_dt_dE(dts);


    ofstream test_result_stream("lf_rk_staticE_test_results.txt", std::ios::out);


    for(int i = 0; i < dts.size() && i < results.first.size() && i < results.second.size(); i++){
        test_result_stream << setprecision(6);

        test_result_stream << std::scientific << dts[i] << std::fixed << " " << results.first[i] <<  " " << results.second[i] << std::endl;
    }

    test_result_stream.close();
}


uint64_t STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1; // ns
int NUM_OF_ELECTRONS = 100;
bool NOTIFICATIONS = false;
int MAX_THREAD_COUNT = 1;
bool MULTI_THREAD = false;
double dT = 0.000001;     // ns
double dT_out = 0.00000; // ns

int main(){

    /*

    vector3d magnet_position1(R2 + 0.35, 0, 0);
    magnet_position1.rotate(vector3d(0,0,1), -5);
    vector3d magnet_position2(R2 + 0.39144, 0, 0);
    magnet_position2.rotate(vector3d(0,0,1), 165);
    vector3d magnet_position3(R2 + 0.4, 0, 0);
    magnet_position3.rotate(vector3d(0,0,1), -25);

    Magnet m1(-0.029796384, 0.15, magnet_position1);
    Magnet m2(-0.039, 0.15, magnet_position2);
    Magnet m3(-0.065, 0.15, magnet_position3);
    magnet_position3.rotate(vector3d(0,0,1), +170);
    Magnet m4(-0.065, 0.15, magnet_position3);
    magnet_position3.rotate(vector3d(0,0,1), +170);
    Magnet m5(-0.065, 0.15, magnet_position3);

    Simulator simulation(0);
    simulation.addMagnet(m1);
    simulation.addMagnet(m2);
    simulation.addMagnet(m3);
    simulation.addMagnet(m4);
    simulation.addMagnet(m5);
    simulation.setNumberofElectrons(NUM_OF_ELECTRONS);
    simulation.setEndTime(simulation_time);
    //simulation.enableMultiThreading(5);

    simulation.openLogs();
    simulation.run();
    simulation.logPaths();
    simulation.closeLogs();



    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(0, 3);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \"out.gif\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(simulation_time*10) + "] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
    

    
    vector3d magnet_position(R2 + 0.35, 0, 0);
    magnet_position.rotate(vector3d(0,0,1), -5);
    Magnet m(-0.13, 1, magnet_position);
    cout << setprecision(6) << m.getOptimalB(0.45, -0.1, -0.01, 0.00001) << endl;*/

    auto start = high_resolution_clock::now();
    int simulation_time = 6;

    
    vector<double> dts;

    dts.push_back(5e-2);
    dts.push_back(1e-2);
    dts.push_back(1e-3);

    dts.push_back(9e-4);
    dts.push_back(8e-4);
    dts.push_back(7e-4);
    dts.push_back(6e-4);
    dts.push_back(5e-4);
    dts.push_back(4e-4);
    dts.push_back(3e-4);
    dts.push_back(2e-4);
    dts.push_back(1e-4);

    dts.push_back(9e-5);
    dts.push_back(8e-5);
    dts.push_back(7e-5);
    dts.push_back(6e-5);
    dts.push_back(5e-5);
    dts.push_back(4e-5);
    dts.push_back(3e-5);
    dts.push_back(2e-5);
    dts.push_back(1e-5);

    dts.push_back(1e-6);

    vector<pair<vector<double>, vector<double>>> results;

    int RUN_COUNT = 5;

    for(int i = 0; i < RUN_COUNT; i++){
        results.push_back(test_out_staticE_rk_lf_dt_dE(dts));

        std::cout << "---> End of RUN : " << i + 1 << std::endl;
    }

    vector<double> dE_avg , dE_std, T_avg, T_std;

    std::cout << "Calculating averages..." << std::endl;

    for(int i = 0 ; i < dts.size() ; i++){
        double first_avg = 0, second_avg = 0;
        for(auto result : results){
            first_avg += result.first[i];
            second_avg += result.second[i];
        }
        dE_avg.push_back(first_avg/results.size());
        T_avg.push_back(second_avg/results.size());
        std::cout << "dt : " << dts[i] << " = dE_ave : " << dE_avg[i] << " T_ave : " << T_avg[i] << std::endl;
    }

    std::cout << "Calculating stds..." << std::endl;

    for(int i = 0 ; i < dts.size() ; i++){
        double first_std = 0, second_std = 0;
        for(auto result : results){
            first_std += (result.first[i] - dE_avg[i])*(result.first[i] - dE_avg[i]);
            second_std += (result.second[i] - T_avg[i])*(result.second[i] - T_avg[i]);
        }
        first_std = sqrt(first_std/(results.size() - 1));
        second_std = sqrt(second_std/(results.size() - 1));

        dE_std.push_back(first_std);
        T_std.push_back(second_std);
        std::cout << "dt : " << dts[i] << " = dE_std : " << dE_std[i] << " T_std : " << T_std[i] << std::endl;
    }

    ofstream test_result_stream("lf_rk_staticE_test_results.txt", std::ios::out);
    ofstream raw_data_stream("raw_lf_rk_staticE_test_results.txt", std::ios::out);

    for(int i = 0; i < dts.size() && i < dE_avg.size() && i < dE_std.size(); i++){
        test_result_stream << "dt(ns) dE_ave(MeV) sdE(MeV) Tsim(s) sTsim(s)" << std::endl <<  setprecision(6);

        test_result_stream << std::scientific << dts[i] << 
        std::fixed << " " << dE_avg[i] <<  " " << dE_std[i] <<
         " " << T_avg[i] <<  " " << T_std[i] <<  std::endl;
    }

    raw_data_stream << "dt(ns) dE_ave(MeV) sdE(MeV) Tsim(s) sTsim(s)" << std::endl <<  setprecision(6);

    for(int i = 0; i < results.size() ; i++){
        raw_data_stream << "   --- RUN : " << i + 1 << " ---\n";

        for(auto result : results){
            raw_data_stream << std::scientific << dts[i] << 
            std::fixed << " " << result.first[i] <<  " " << result.second[i] << std::endl;
        }
    }

    test_result_stream.close();
    raw_data_stream.close();

    //plot(simulation_time);




    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Simulation finished in : " << duration.count() << " us     ( "<<duration.count()/1000000.0 << " s )" << endl;



    return 0;
}


void TEST_MAG_LF_RK(){

    vector<double> dts;

    dts.push_back(5e-2);
    dts.push_back(1e-2);
    dts.push_back(1e-3);

    dts.push_back(9e-4);
    dts.push_back(8e-4);
    dts.push_back(7e-4);
    dts.push_back(6e-4);
    dts.push_back(5e-4);
    dts.push_back(4e-4);
    dts.push_back(3e-4);
    dts.push_back(2e-4);
    dts.push_back(1e-4);

    dts.push_back(9e-5);
    dts.push_back(8e-5);
    dts.push_back(7e-5);
    dts.push_back(6e-5);
    dts.push_back(5e-5);
    dts.push_back(4e-5);
    dts.push_back(3e-5);
    dts.push_back(2e-5);
    dts.push_back(1e-5);

    dts.push_back(1e-6);



    auto results = test_out_mag_rk_lf_dt_dE(dts);


    ofstream test_result_stream("lf_rk_test_results.txt", std::ios::out);


    for(int i = 0; i < dts.size() && i < results.first.size() && i < results.second.size(); i++){
        test_result_stream << setprecision(6);

        test_result_stream << std::scientific << dts[i] << std::fixed << " " << results.first[i] <<  " " << results.second[i] << std::endl;
    }

    test_result_stream.close();

}

std::pair<vector<double>, vector<double>> test_out_mag_rk_lf_dt_dE(vector<double> dts){
    vector<double> E_tries;
    vector<double> t_sims;
    for(double dt : dts){
        dT = dt;

        auto start = high_resolution_clock::now();
        int simulation_time = 5;

        Simulator simulation(0);
        simulation.setEin(1);
        simulation.setNumberofElectrons(NUM_OF_ELECTRONS);
        simulation.setEmax(0);
        simulation.setEndTime(simulation_time);

        vector3d v(0.05,0,0);
        Magnet m(0.1,10,v);
        simulation.addMagnet(m);
        simulation.openLogs();
        simulation.runBonly();
        simulation.logPaths();
        simulation.closeLogs();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        double dE = simulation.getElectronWithMaxEnergy().Et - E0 - 1;

        std::cout << "Simulation for dt : " << std::scientific << dT << " ns, Tsim = "  
                << std::fixed <<duration.count()/1000000.0 << "s, dE = " << dE << std::endl << std::flush;

        E_tries.push_back(dE);
        t_sims.push_back(duration.count()/1000000.0);
    }
    return make_pair(E_tries,t_sims);
}

std::pair<vector<double>, vector<double>> test_out_staticE_rk_lf_dt_dE(vector<double> dts){
    vector<double> E_tries;
    vector<double> t_sims;
    for(double dt : dts){
        dT = dt;

        auto start = high_resolution_clock::now();
        int simulation_time = 6;

        Simulator simulation(0);
        simulation.setEin(1);
        simulation.setNumberofElectrons(NUM_OF_ELECTRONS);
        simulation.setEndTime(simulation_time);

        vector3d E(0, -2*2.65616, 0);
        simulation.setStaticConstantEfield(E, R2, R2);
        simulation.openLogs();
        simulation.runStaticEonly();
        simulation.logPaths();
        simulation.closeLogs();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        double dE = simulation.getElectronWithMaxEnergy().Et - E0 - 5;

        std::cout << "Simulation for dt : " << std::scientific << dT << " ns, Tsim = "  
                << std::fixed <<duration.count()/1000000.0 << "s, dE = " << dE << std::endl << std::flush;

        E_tries.push_back(dE);
        t_sims.push_back(duration.count()/1000000.0);
    }
    return make_pair(E_tries,t_sims);
}

void plot(double simulation_time){
    Gnuplot gp;
    gp.setRange(-R2,R2,-R2, R2);
    gp.enableMinorTics();
    gp.setCbRange(1, 4);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 5");
    gp.addCommand("set output \"out.gif\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(simulation_time*10) + "] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    std::string plotCommandB = "do for [i=1:" + to_string(simulation_time*10) + "] {plot ";
    plotCommandB += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommandB +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommandB +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommandB += "}";

    gp.setPlotCommand(plotCommandB);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}