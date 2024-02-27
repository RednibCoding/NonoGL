// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


int mgFps(float dt) {
    static int fps = 0;
    static float accumulatedTime = 0.0f;
    static int frameCount = 0;

    accumulatedTime += dt;
    (frameCount)++;

    if (accumulatedTime >= 1.0f) {
        fps = frameCount;
        frameCount = 0;
        accumulatedTime = 0.0f;
    }

    return fps;
}


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^