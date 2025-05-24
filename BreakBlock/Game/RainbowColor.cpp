#include "../AllSetting.h"
#include "RainbowColor.h"

RainbowColor::RainbowColor(float speed) :
	r(0.0f),
	g(1.0f),
	b(0.0f),
	speed(speed),
	state(0) {

}

void RainbowColor::Update() {

	try
	{
		// 6つの状態を回り続ける(それぞれ0～1,1～0がある)
		switch (state) {
		case 0:
			r += speed * frameDurationS;
			if (r >= 1.0f) {
				r = 1.0f;
				state = 1;
			}
			break;

		case 1:
			g -= speed * frameDurationS;
			if (g <= 0.0f) {
				g = 0.0f;
				state = 2;
			}
			break;

		case 2:
			b += speed * frameDurationS;
			if (b >= 1.0f) {
				b = 1.0f;
				state = 3;
			}
			break;

		case 3:
			r -= speed * frameDurationS;
			if (r <= 0.0f) {
				r = 0.0f;
				state = 4;
			}
			break;

		case 4:
			g += speed * frameDurationS;
			if (g >= 1.0f) {
				g = 1.0f;
				state = 5;
			}
			break;

		case 5:
			b -= speed * frameDurationS;
			if (b <= 0.0f) {
				b = 0.0f;
				state = 0;
			}
			break;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("レインボーカラーUpdateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}