#include <gui/endscreen_screen/EndScreenView.hpp>
#include <cmsis_os.h>
#include <gui/playscreen_screen/PlayScreenView.hpp>

EndScreenView::EndScreenView()
{
	uint16_t score = PlayScreenView::finalscore;
	Unicode::snprintf(finalscoretxtBuffer, FINALSCORETXT_SIZE, "%d", score);
	finalscoretxt.resizeToCurrentText();
	invalidate();
}

void EndScreenView::setupScreen()
{
    EndScreenViewBase::setupScreen();
}

void EndScreenView::tearDownScreen()
{
    EndScreenViewBase::tearDownScreen();
}

extern osMessageQueueId_t Queue1Handle;

void EndScreenView::handleTickEvent()
{
	EndScreenViewBase::handleTickEvent();
	uint8_t res = 0;
	uint32_t count = osMessageQueueGetCount(Queue1Handle);

	if(count > 0){
		osMessageQueueGet(Queue1Handle, &res, NULL, osWaitForever);
		if(res == 'R'){
			static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(65); // keycode from image send
		}
	}

}


