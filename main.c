#include "raylib.h"
#include "raymath.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(void)
{
    float timePlay = 0.0f;
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);

	// window settings
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "масюня");
	SetWindowMinSize(170, 240);
	SetTargetFPS(60);
    int getMon = GetCurrentMonitor();
	
	// bgm init and loading
	InitAudioDevice();
	Music music = LoadMusicStream("res/bgm.mp3");
	PlayMusicStream(music);
	SetMusicVolume(music, 1.0f);
    bool pause = false;
	
	// masyunya`s model and position loading
    Model model = LoadModel("res/masyunya.obj");
    Vector3 position = { -0.12f, 0.0f, 0.0f };
    Texture2D texture = LoadTexture("res/masyunya.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // camera settings
    Camera camera = { { 18.0f, 18.0f, 18.0f }, { 0.0f, 2.3f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 20.0f, 0 };
    SetCameraMode(camera, CAMERA_ORBITAL);

    while (!WindowShouldClose())
    {
		
		// scaling while window resizing
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        
        UpdateCamera(&camera);
        UpdateMusicStream(music);
        
		timePlay = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
		
		// music loop
		if (timePlay > 1.0f)
		{
			timePlay = 1.0f;
			StopMusicStream(music);
			PlayMusicStream(music);
		}
		
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) StopMusicStream(music);
            else PlayMusicStream(music);
        }
        
        if (IsKeyPressed(KEY_F11))
        {
            if (IsWindowFullscreen())
            {
                SetWindowSize(screenWidth, screenHeight);
            }
            else
            {
                SetWindowSize(GetMonitorWidth(getMon), GetMonitorHeight(getMon));
            }
				ToggleFullscreen();
        }
        
        BeginDrawing();
                
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModel(model, position, 4.0f, WHITE);
                
            EndMode3D();
            
        if (IsKeyDown(KEY_F3))
        {
			DrawFPS(10, 10);
			DrawText(TextFormat("width: [%i], height: [%i]",(int)GetMonitorWidth(0), (int)GetMonitorHeight(0)), 10, 30, 10, GREEN);
			DrawText(TextFormat("music state: [%d]",(int)IsMusicStreamPlaying(music)), 10, 40, 10, GREEN);
				DrawText(TextFormat("length: [%f]",(float)GetMusicTimeLength(music)), 40, 50, 10, GREEN);
				DrawText(TextFormat("played: [%lf]",(float)GetMusicTimePlayed(music)), 40, 60, 10, GREEN);
        }

        EndDrawing();

    }
	
	UnloadMusicStream(music);
    UnloadTexture(texture);
    UnloadModel(model);

    CloseWindow();

    return 0;
}
