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



}
