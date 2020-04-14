#include <sl.h>

const int W = 400, H = 400;

int main(int args, char* argv[]) {
	// set up our window and a few resources we need
	slWindow(W, H, "Simple SIGIL Example", false);
	slSetFont(slLoadFont("ttf/white_rabbit.ttf"), 24);
	slSetTextAlign(SL_ALIGN_CENTER);
	int tex = slLoadTexture("png/glow.png");

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// background glow
		slSetForeColor(0.1, 0.9, 0.2, 0.4);

		slSprite(tex, 200, 240, 300, 200);

		// large text and fat line
		slSetForeColor(0.0, 0.8, 0.2, 1.0);
		slSetFontSize(24);
		slText(200, 250, "SIGIL:");
		slRectangleFill(200, 240, 100, 7);

		// smaller subtext
		slSetFontSize(14);
		slText(200, 220, "Sprites, text, sound, input, and more!");
		slLine(48, 210, 352, 210);

		// other lines
		slLine(100, 100, 360, 200);

		slRender();
		// draw everything
	}
	// close the window and shut down SIGIL
	slClose();
	return 0;
}