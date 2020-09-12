#include <hal/debug.h>
#include <hal/video.h>

const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;

int main() {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
}
