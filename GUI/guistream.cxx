// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME GUIdIguistream
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "GUI/frames/MainFrame.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_MainFrame(void *p);
   static void deleteArray_MainFrame(void *p);
   static void destruct_MainFrame(void *p);
   static void streamer_MainFrame(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MainFrame*)
   {
      ::MainFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MainFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MainFrame", ::MainFrame::Class_Version(), "GUI/frames/MainFrame.h", 47,
                  typeid(::MainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MainFrame::Dictionary, isa_proxy, 16,
                  sizeof(::MainFrame) );
      instance.SetDelete(&delete_MainFrame);
      instance.SetDeleteArray(&deleteArray_MainFrame);
      instance.SetDestructor(&destruct_MainFrame);
      instance.SetStreamerFunc(&streamer_MainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MainFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::MainFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MainFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MainFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MainFrame::Class_Name()
{
   return "MainFrame";
}

//______________________________________________________________________________
const char *MainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MainFrame.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MainFrame(void *p) {
      delete (static_cast<::MainFrame*>(p));
   }
   static void deleteArray_MainFrame(void *p) {
      delete [] (static_cast<::MainFrame*>(p));
   }
   static void destruct_MainFrame(void *p) {
      typedef ::MainFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MainFrame(TBuffer &buf, void *obj) {
      ((::MainFrame*)obj)->::MainFrame::Streamer(buf);
   }
} // end of namespace ROOT for class ::MainFrame

namespace {
  void TriggerDictionaryInitialization_guistream_Impl() {
    static const char* headers[] = {
"GUI/frames/MainFrame.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/homebrew/Cellar/root/6.28.04_1/include/root",
"/Users/mfurkaner/Desktop/Projeler/Cpp/RhodoSim/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "guistream dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$GUI/frames/MainFrame.h")))  MainFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "guistream dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "GUI/frames/MainFrame.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"MainFrame", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("guistream",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_guistream_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_guistream_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_guistream() {
  TriggerDictionaryInitialization_guistream_Impl();
}
