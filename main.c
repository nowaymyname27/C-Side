#include "raylib.h"

// 2048 characters plus 1 for the null terminator
#define BUFFER_SIZE 2049

// Container for the current text data and its length
typedef struct {
  char text[BUFFER_SIZE];
  int letterCount;
  float timer;
} TextBuffer;

// Process keyboard input and update the buffer state
void handle_input(TextBuffer *buffer){
  // GetCharPressed returns characters from the internal Raylib queue
  int key = GetCharPressed();

  // Drain the queue to handle multiple keypresses in one frame
  while (key != 0){
    if (buffer->letterCount < BUFFER_SIZE - 1) {
      buffer->text[buffer->letterCount] = (char)key;
      buffer->letterCount++;
      buffer->text[buffer->letterCount] = '\0'; // Ensure string is always terminated
    }
    key = GetCharPressed();
  }

  // Handle deletions: Decrease count and move the 'stop sign' back
  if (IsKeyPressed(KEY_BACKSPACE) && buffer->letterCount > 0){
    buffer->letterCount--;
    buffer->text[buffer->letterCount] = '\0';
    buffer->timer = 0;
  }

  // Handle Deletions: When key is held deletes rapidly
  if (IsKeyDown(KEY_BACKSPACE) && buffer->letterCount > 0){
    buffer->timer += GetFrameTime();
    if (buffer->timer > 0.6){
      buffer->letterCount--;
      buffer->text[buffer->letterCount] = '\0';
      buffer->timer = 0.58;
    }
  } else {
    buffer->timer = 0;
  }
}

// Render the editor state to the window
void draw_to_screen(TextBuffer *buffer) {
  BeginDrawing();
    ClearBackground(DARKGRAY);
    // Draw the main text buffer
    DrawText(buffer->text, 50, 50, 50, RED);

    // Calculate cursor position based on text width
    int width = MeasureText(buffer->text, 50);

    // Blinking logic: Draw cursor only during the first half of every second
    int whole_seconds = (int)GetTime();
    if (GetTime() - whole_seconds < 0.5){
      DrawRectangle(55 + width, 50, 10, 45, PINK);
    }
  EndDrawing();
}

int main(void) {
  InitWindow(1024, 768, "C-Side");
  SetTargetFPS(60);

  // Initialize the buffer to all zeros
  TextBuffer buffer = { 
    .text = {0}, 
    .letterCount = 0,
    .timer = 0
  };


  // Main game loop
  while(!WindowShouldClose()){
    handle_input(&buffer);
    draw_to_screen(&buffer);
  }

  CloseWindow();
  return 0;
}
