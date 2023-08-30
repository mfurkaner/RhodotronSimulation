#include "gui_msgbox_handler.h"


//namespace RhodotronSimulatorGUI::Handlers{
    GUIMessageBoxHandler* GUIMessageBoxHandler::_singleton = NULL;
    
    GUIMessageBoxHandler::GUIMessageBoxHandler(TGWindow *_main){
        if(_singleton == NULL){
            main = _main;
            _singleton = this;
        }
    }

    GUIMessageBoxHandler* GUIMessageBoxHandler::GetObject(){
        return _singleton;
    }

    void GUIMessageBoxHandler::CreateObject(TGWindow *_main){
        if(_singleton == NULL){
            _singleton = new GUIMessageBoxHandler(_main);
        }
    }
    void GUIMessageBoxHandler::DestroyObject(){
        if(_singleton != NULL){
            delete _singleton;
            _singleton = NULL;
        }
    }

    MessageBoxResult GUIMessageBoxHandler::DrawYesNoExclamation(const char * title, const char * message){
        int msg_ret;
        MessageBoxResult result;
        auto msg = new TGMsgBox(gClient->GetRoot(), main, title, message, EMsgBoxIcon::kMBIconExclamation, EMsgBoxButton::kMBYes | EMsgBoxButton::kMBNo, &msg_ret);
        if(msg_ret == EMsgBoxButton::kMBYes){
            result = Yes;
        }
        else if(msg_ret == EMsgBoxButton::kMBNo){
            result = No;
        }
        return result;
    }

    MessageBoxResult GUIMessageBoxHandler::DrawYesNoQuestion(const char * title, const char * message){
        int msg_ret;
        MessageBoxResult result;
        auto msg = new TGMsgBox(gClient->GetRoot(), main, title, message, EMsgBoxIcon::kMBIconQuestion, EMsgBoxButton::kMBYes | EMsgBoxButton::kMBNo, &msg_ret);
        if(msg_ret == EMsgBoxButton::kMBYes){
            result = Yes;
        }
        else if(msg_ret == EMsgBoxButton::kMBNo){
            result = No;
        }
        return result;
    }

//}