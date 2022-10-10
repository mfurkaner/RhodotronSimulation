#include "gui_render_frame.h"


namespace RhodotronSimulatorGUI::frames{


    RenderFrame::RenderFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;

        progressBar = new TGHProgressBar(this, 250, 50);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();

        canvas = new TRootEmbeddedCanvas("output", this, 500, 500);

        canvas->GetCanvas()->cd(1);

        Draw3dRectangle(0, 100,100, 100, 100);

        canvas->GetCanvas()->Update();


        this->AddFrame(progressBar, center_layout);
        this->AddFrame(canvas, center_layout);

    }

    void RenderFrame::UpdateProgressBar(uint8_t progress){
        progressBar->SetPosition(progress);
    }

    TGProgressBar* RenderFrame::GetProgressBar(){
        return progressBar;
    }

/*
    void RenderFrame::GetGif(std::string filename){
        //dummy_image
        TImage* image;
        uint32_t i = 0;
        while ( true ) {
            if ( i == 10 ) break;
            std::string name = filename + ".";
            name += std::to_string(i);
            image = TImage::Open(name.c_str());
            //TASImage* image = new TASImage(
            std::cout << name << " is added" << std::endl;
            images.push_back(image);
            i++;
        }
        std::cout << "There are " << images.size() << " images."<< std::endl;
    }


    void RenderFrame::DrawGif(){
        

        for (uint32_t i = 0; i < images.size(); i++) {
            std::cout << "drawing " << i << std::endl;
            canvas->GetCanvas();
            images[i]->Draw();
            canvas->GetCanvas()->Modified();
            //canvas->GetCanvas()->cd(1)->Update();
            canvas->GetCanvas()->Update();
            sleep(1);
        }
    }*/


    void RenderFrame::Render(){
        renderer.fillLogs();
        renderer.render(canvas);
    }



}
