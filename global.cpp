#include "global.h"
#include <QDateTime>

Global::Global()
{

}

Global::~Global()
{

}

void Global::setMainWindow(MainWindow *w)
{
    main_ui = w;
}


Global *Global::mGlobal = NULL;

Global *Global::getGlobal()
{
    if(mGlobal == NULL)
    {
        mGlobal = new Global();
    }
    return mGlobal;
}



