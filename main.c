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

int main(void) {
  InitWindow(1024, 768, "C-Side");
  SetTargetFPS(60);

  // Initialize the buffer to all zeros
  TextBuffer buffer = {
      .text = {0},
      .letterCount = 0,
      .backspaceTimer = 0,
      .moveTimer = 0,
      .cursorIndex = 0
  };

  // Clay Initialization
  u64 size = Clay_MinMemorySize();
  Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(size, malloc(size));
  Clay_Initialize(arena, (Clay_Dimensions){1024,768}, (Clay_ErrorHandler){0});
  buffer.editorFont = LoadFontEx("./resources/JetBrainsMono-Light.ttf", 55, NULL, 0);
  SetTextureFilter(buffer.editorFont.texture, TEXTURE_FILTER_BILINEAR);
  Clay_SetMeasureTextFunction(Raylib_MeasureText, &buffer.editorFont);

  // Main game loop
  while (!WindowShouldClose()) {
    handle_input(&buffer);
     BeginDrawing();
        ClearBackground(BLACK);
        
        // This calculates layout AND draws it using the Raylib renderer
        layout_ui(&buffer); 
        
    EndDrawing();
  }

  UnloadFont(buffer.editorFont);
  CloseWindow();
  return 0;
}
