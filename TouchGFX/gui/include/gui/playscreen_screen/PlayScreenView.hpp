#ifndef PLAYSCREENVIEW_HPP
#define PLAYSCREENVIEW_HPP

#include <gui_generated/playscreen_screen/PlayScreenViewBase.hpp>
#include <gui/playscreen_screen/PlayScreenPresenter.hpp>


class PlayScreenView : public PlayScreenViewBase
{
public:
    PlayScreenView();
    virtual ~PlayScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void handleTickEvent();
    bool gamestart;
    uint16_t score;
    static uint16_t finalscore;
protected:
};

#endif // PLAYSCREENVIEW_HPP
