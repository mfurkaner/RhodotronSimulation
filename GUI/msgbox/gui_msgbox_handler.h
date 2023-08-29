#ifndef MSGBOX_HANDLER_H
#define MSGBOX_HANDLER_H

#include "TGWindow.h"
#include "TGMsgBox.h"


namespace RhodotronSimulatorGUI{
    enum MessageBoxResult{
        Yes,
        No,
    };
}



namespace RhodotronSimulatorGUI::Handlers{


    class GUIMessageBoxHandler{
    private:
        static GUIMessageBoxHandler* _singleton;
        TGWindow *main;
        
        GUIMessageBoxHandler(TGWindow *main);
    public:
        GUIMessageBoxHandler(const GUIMessageBoxHandler& other) = delete;

        static GUIMessageBoxHandler* GetObject();

        static void CreateObject(TGWindow *main);
        static void DestroyObject();

        RhodotronSimulatorGUI::MessageBoxResult DrawYesNoExclamation(const char * title, const char * message);

        RhodotronSimulatorGUI::MessageBoxResult DrawYesNoQuestion(const char * title, const char * message);

    };
    GUIMessageBoxHandler* GUIMessageBoxHandler::_singleton = NULL;

}

#endif