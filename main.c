#include "raylib.h"

#define BUFFER_SIZE 2049

typedef struct {
  char text[BUFFER_SIZE];
  int letterCount;
} TextBuffer;

void handle_input(TextBuffer *buffer){
  int key = GetCharPressed();
  while (key != 0){
    if (buffer->letterCount < BUFFER_SIZE - 1) {
      buffer->text[buffer->letterCount] = key;
      buffer->letterCount++;
      buffer->text[buffer->letterCount] = '\0';
    }
      key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) == 1 && buffer->letterCount > 0){
      buffer->letterCount--;
      buffer->text[buffer->letterCount] = '\0';
    }
}

void draw_to_screen(TextBuffer *buffer) {
  BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText(buffer->text, 50, 50, 50, RED);
    int width = MeasureText(buffer->text, 50);
    int half_sec = (int)GetTime();
    if (GetTime() - half_sec  < 0.5){
      DrawRectangle(55 + width, 50, 10, 45, PINK);
    }
  EndDrawing();
}

int main(void) {
  InitWindow(1024, 768, "C-Side");
  SetTargetFPS(60);

  TextBuffer buffer = { 
    .text = {0}, 
    .letterCount = 0
  };

  while(!WindowShouldClose()){
    handle_input(&buffer);
    draw_to_screen(&buffer);
  }

  CloseWindow();
  return 0;

}
