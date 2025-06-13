#include "CamerManager.hpp"

using namespace CameraManager;

int main(void){
    CAMERA_MANAGER c1(0, 0, 0, 0, 0, SD);
    c1.Start_Camera();
    c1.Show_video();
    c1.Stop_Camera();
    return 0;
}