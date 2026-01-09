#include "raylib.h"

#define BUFFER_SIZE 2049

typedef struct {
  char text[BUFFER_SIZE];
  int letterCount;
} TextBuffer;

void handleInput(TextBuffer *text){
  int key = GetCharPressed();
  while (key != 0){
    if (text->letterCount < BUFFER_SIZE - 1) {
      text->text[text->letterCount] = key;
      text->letterCount++;
      text->text[text->letterCount] = '\0';
    }
      key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) == 1){
      text->letterCount--;
      text->text[text->letterCount] = '\0';
    }
}
int main(void) {
  InitWindow(1024, 768, "C-Side");
  SetTargetFPS(60);
  TextBuffer myText = { .text = {0}, .letterCount = 0};


  while(!WindowShouldClose()){
    BeginDrawing();
      ClearBackground(DARKGRAY);
      handleInput(&myText);
      DrawText(myText.text, 50, 50, 50, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;

}
