#include "raylib.h"
#include "buffer.h"
#include "editor.c"
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

  // Main game loop
  while(!WindowShouldClose()){
    handle_input(&buffer);
    draw_to_screen(&buffer);
  }

  CloseWindow();
  return 0;
}
