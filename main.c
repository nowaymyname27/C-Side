#include "buffer.h"
#include "editor.c"
#include "raylib.h"
#include <stdio.h>

int main(void) {
  InitWindow(1024, 768, "C-Side");
  SetTargetFPS(60);

  // Initialize the buffer to all zeros
  TextBuffer buffer = {
      .text = {0},
      .letterCount = 0,
      .timer = 0,
      .cursorIndex = 0
  };

  buffer.editorFont = LoadFontEx("./resources/JetBrainsMono-Light.ttf", 55, NULL, 0);
  SetTextureFilter(buffer.editorFont.texture, TEXTURE_FILTER_BILINEAR);
  // Main game loop
  while (!WindowShouldClose()) {
    handle_input(&buffer);
    draw_to_screen(&buffer);
  }

  UnloadFont(buffer.editorFont);
  CloseWindow();
  return 0;
}
