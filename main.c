#define CLAY_IMPLEMENTATION
#include "./clay/clay.h"
#include "./clay/renderers/raylib/clay_renderer_raylib.c"

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "editor.c"
#include "file_io.c"
#include "ui_layout.c"

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

int main(void) {
    // 1. Initialize Clay Memory
    u64 size = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(size, malloc(size));
    
    // 2. Initialize Clay + Raylib together
    // This replaces InitWindow, SetTargetFPS, etc.
    Clay_Initialize(arena, (Clay_Dimensions){1024, 768}, (Clay_ErrorHandler){HandleClayErrors});
    Clay_Raylib_Initialize(1024, 768, "C-Side Notes", FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    // 3. Setup Logic Data
    TextBuffer buffer = { 
      .text = {0},
      .cursorIndex = 0 
    };
    
    // 4. Load Resources
    buffer.fonts[0] = LoadFontEx("resources/JetBrainsMono-Light.ttf", 55, NULL, 0);
    SetTextureFilter(buffer.fonts[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, &buffer.fonts);
    
    scan_directory(&buffer);

    // 5. Main Loop
    while (!WindowShouldClose()) {
        handle_input(&buffer);

        // We wrap everything in the standard Raylib drawing block
        BeginDrawing();
        ClearBackground(DARKPURPLE);
        
        layout_ui(&buffer);
        
        EndDrawing();
    }

    return 0;
}
