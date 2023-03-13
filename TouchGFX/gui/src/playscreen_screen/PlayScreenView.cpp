#include <gui/playscreen_screen/PlayScreenView.hpp>
#include <cmsis_os.h>

#define JUMPSPEED -6
#define FALLINGCONST 0.4

void moveGround(touchgfx::Image&);
void movePipe(touchgfx::Container&);
bool checkOverlap(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t,int16_t,int16_t);
bool checkCollision(touchgfx::AnimatedImage&, touchgfx::Container&, touchgfx::Container&);
bool checkGetScore(touchgfx::AnimatedImage&, touchgfx::Container&, touchgfx::Container&);

// để sinh ngẫu nhiên các pipe với vị trí theo trục dọc khác nhau
// để đơn giản nhóm sử dụng mảng ngẫu nhiên độ dịch chuyển của pipe
int16_t random_arr[50] = {-34, -10, -36, -47, -37, 2, 22, -33, -31, 43, 21, 11, 10, 41, 47, 28, 43, -31, -11, 10, 23, -12, -23, -33, -45, 3, -23, -46, -50, -19, -25, -30, -19, -45, -43, -31, -21, 13, 28, 47, 25, 2, 24, -21, -8, -2, -50, -14, 15};
int16_t i = 0;

// vận tốc của chim theo chiều dọc
float vertSpeed;

// khởi tạo
uint16_t PlayScreenView::finalscore = 0;

// hàm dịch chuyển các pipe
void movePipe(touchgfx::Container &pipe)
{
	int16_t x = pipe.getX();
	if (x == -52) x = 240;
	if (x == 240) {
		int16_t random_num = random_arr[i];
		i = (i + 1) % 50;
		int16_t y = -236 + random_num;
		pipe.setY(y);
	}
	x = x - 2;
	pipe.setX(x);
}

// hàm dịch chuyển mặt đất
void moveGround(Image &groundimg)
{
	int16_t x = groundimg.getX();
	x = x - 2;
	if (x == -24) x = 0;
	groundimg.setX(x);
}

// hàm kiểm tra 2 hình chữ nhật có giao nhau hay không
bool checkOverlap(int16_t x11, int16_t y11, int16_t x12, int16_t y12,
 int16_t x21, int16_t y21, int16_t x22, int16_t y22)
{
	if(x11 <= x22 && x12 >= x21){
		if(y11 <= y22 && y12 >= y21) return true;
	}
	return false;
}

// hàm kiểm tra va chạm giữa bird và pipe
bool checkCollision(touchgfx::AnimatedImage &birdimg, touchgfx::Container &pipe1, touchgfx::Container &pipe2)
{
	int16_t xbird = birdimg.getX(), ybird = birdimg.getY();
	int16_t xpipe1 = pipe1.getX(), ypipe1 = pipe1.getY();
	int16_t xpipe2 = pipe2.getX(), ypipe2 = pipe2.getY();

	//Check land ground
	if(ybird >= 264) return true;
	//Check collision with pipe1
	if(checkOverlap(xbird, ybird, xbird + 34, ybird + 24, xpipe1, ypipe1, xpipe1 + 52, ypipe1 + 320)) return true;
	if(checkOverlap(xbird, ybird, xbird + 34, ybird + 24, xpipe1, ypipe1 + 420, xpipe1 + 52, ypipe1 + 740)) return true;
	//Check collision with pipe2
	if(checkOverlap(xbird, ybird, xbird + 34, ybird + 24, xpipe2, ypipe2, xpipe2 + 52, ypipe2 + 320)) return true;
	if(checkOverlap(xbird, ybird, xbird + 34, ybird + 24, xpipe2, ypipe2 + 420, xpipe2 + 52, ypipe2 + 740)) return true;

	return false;
}

// hàm kiểm tra ghi được điểm hay không
bool checkGetScore(touchgfx::AnimatedImage &birdimg, touchgfx::Container &pipe1, touchgfx::Container &pipe2)
{
	int16_t xpipe1 = pipe1.getX();
	int16_t xpipe2 = pipe2.getX();
	//Check get score
	if(xpipe1 == 46) return true;
	if(xpipe2 == 46) return true;
	return false;
}


PlayScreenView::PlayScreenView()
{
	score = 0;
	gamestart = false;
	vertSpeed = 0;
}


void PlayScreenView::setupScreen()
{
    PlayScreenViewBase::setupScreen();
}

void PlayScreenView::tearDownScreen()
{
    PlayScreenViewBase::tearDownScreen();
}

extern osMessageQueueId_t Queue1Handle;



void PlayScreenView::handleTickEvent()
{
	PlayScreenViewBase::handleTickEvent();
	moveGround(groundimg);

	uint8_t res = 0;
	uint32_t count = osMessageQueueGetCount(Queue1Handle);
	if(!gamestart){
		if(count > 0){
			osMessageQueueGet(Queue1Handle, &res, NULL, osWaitForever);
			if(res == 'R'){
				scoretxt.setVisible(1);
				introimg.setVisible(0);
				madebytxt.setVisible(0);
				gamestart = true;
			}
		}
	} else{
		movePipe(pipe1);
		movePipe(pipe2);
		if(count > 0){
			osMessageQueueGet(Queue1Handle, &res, NULL, osWaitForever);
			if(res == 'R'){
				vertSpeed = JUMPSPEED;
			}
		}
		int16_t y = birdimg.getY();
		y += (int)vertSpeed;
		vertSpeed += FALLINGCONST;
		birdimg.setY(y);
		if(checkCollision(birdimg, pipe1, pipe2)){

			finalscore = score;
			static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(65);

		}
		if(checkGetScore(birdimg, pipe1, pipe2)){
			score += 1;
			Unicode::snprintf(scoretxtBuffer, SCORETXT_SIZE, "%d", score);
			scoretxt.resizeToCurrentText();
		}
	}
	invalidate();
}
