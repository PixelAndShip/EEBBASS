#include "screens.h"

std::unique_ptr<Screen> makeScreen(ScreenId id)
{
    switch (id)
    {
    case ScreenId::Tutorial:
        return makeTutorialScreen();
    case ScreenId::SaveFiles:
        return makeFileListScreen(false);
    case ScreenId::Logs:
        return makeFileListScreen(true);
    case ScreenId::Create:
        return makeCreateScreen();
    case ScreenId::CustomVariables:
        return makeVariablesScreen(false);
    case ScreenId::MultiVariables:
        return makeVariablesScreen(true);
    case ScreenId::FromSave:
        return makeFromSaveScreen();
    case ScreenId::Current:
        return makeCurrentScreen();
    }
    return makeTutorialScreen();
}
