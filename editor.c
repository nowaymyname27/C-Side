#include "buffer.h"
#include "raylib.h"
#include <stdio.h>

// Helper to get the visual X/Y for a specific index in the text
Vector2 get_visual_position(TextBuffer *buffer, int targetIndex, int startX, int startY, int maxWidth) {
    int currentX = 0; // Relative to startX
    int currentY = 0; // Relative to startY
    
    // Measure the width of a single character (using Monospace assumption)
    // "M" is usually the standard width test
    float charWidth = MeasureTextEx(buffer->fonts[0], "M", 50, 0).x;
    int lineHeight = 50; // Or your configured LineHeight

    for (int i = 0; i < targetIndex; i++) {
        char c = buffer->text[i];

        if (c == '\n') {
            currentX = 0;
            currentY += lineHeight;
        } else {
            // Advance cursor
            currentX += (int)charWidth;

            // WRAP LOGIC: Check if we just went past the limit
            // Only checks if the flag is enabled!
            if (currentX >= maxWidth) {
                currentX = 0; // Reset to start of next line
                currentY += lineHeight;
                
                // If the character wasn't a newline, we draw it on the NEW line, so advance X
                // (Unless we want it to sit on the left edge)
                currentX += (int)charWidth; 
            }
        }
    }

    return (Vector2){ (float)(startX + currentX), (float)(startY + currentY) };
}

// Inserts char into where the cursorIndex is
void buffer_insert(TextBuffer *buffer, char c) {
  for (i32 i = buffer->letterCount; i >= buffer->cursorIndex; i--) {
    buffer->text[i + 1] = buffer->text[i];
  }
  buffer->text[buffer->cursorIndex] = c;
  buffer->letterCount++;
  buffer->cursorIndex++;
}

void buffer_delete(TextBuffer *buffer) {
  for (i32 i = buffer->cursorIndex; i <= buffer->letterCount; i++) {
    buffer->text[i - 1] = buffer->text[i];
  }
  buffer->letterCount--;
  buffer->cursorIndex--;
}

i32 get_line_start(TextBuffer *buffer, i32 from_index) {
  i32 i = from_index - 1;
  while (i >= 0 && buffer->text[i] != '\n') {
    i--;
  }
  return i + 1;
}

i32 get_line_end(TextBuffer *buffer, i32 from_index) {
  i32 i = from_index;
  while (buffer->text[i] != '\n' && buffer->text[i] != '\0') {
    i++;
  }
  return i;
}

i32 get_column(TextBuffer *buffer) {
  i32 line_start = get_line_start(buffer, buffer->cursorIndex);
  return buffer->cursorIndex - line_start;
}

void debbug_log(TextBuffer *buffer) {
  i32 line_start = get_line_start(buffer, buffer->cursorIndex);
  i32 column = get_column(buffer);
  printf("text: %s \n", buffer->text);
  printf("letterCount: %d \n", buffer->letterCount);
  printf("cursorIndex: %d \n", buffer->cursorIndex);
  printf("line_start: %d \n", line_start);
  printf("column: %d \n", column);
}

void handle_typing(TextBuffer *buffer) {
  int key = GetCharPressed();
  while (key != 0) {
    buffer_insert(buffer, (char)key);
    key = GetCharPressed();
    if (buffer->debugFlag != 0){
      debbug_log(buffer);
    }
  }
}

void handle_deletion(TextBuffer *buffer) {
  // 1. Single Press
  if (IsKeyPressed(KEY_BACKSPACE)) {
    if (buffer->cursorIndex > 0) {
      buffer_delete(buffer);
      buffer->backspaceTimer = 0; // Reset timer on fresh press
    }
    if (buffer->debugFlag != 0){
      debbug_log(buffer);
    }
  }

  // 2. Rapid Hold
  if (IsKeyDown(KEY_BACKSPACE)) {
    buffer->backspaceTimer += GetFrameTime();
    if (buffer->backspaceTimer > 0.6) { // Threshold
      if (buffer->cursorIndex > 0) {
        buffer_delete(buffer);
      }
      buffer->backspaceTimer = 0.58; // Repeat rate
    }
  } else {
    buffer->backspaceTimer = 0;
  }
  if (buffer->debugFlag != 0){
      debbug_log(buffer);
  }
}

void move_cursor(TextBuffer *buffer, int key) {
  if (key == KEY_LEFT && buffer->cursorIndex > 0) {
    buffer->cursorIndex--;
  } else if (key == KEY_RIGHT && buffer->cursorIndex < buffer->letterCount) {
    buffer->cursorIndex++;
  } else if (key == KEY_UP) {
    i32 column = get_column(buffer);
    i32 current_line_start = get_line_start(buffer, buffer->cursorIndex);
    if (current_line_start != 0) {
      i32 previous_line_start = get_line_start(buffer, current_line_start - 1);
      i32 previous_line_end = get_line_end(buffer, previous_line_start);
      i32 target_index = previous_line_start + column;
      if (target_index < previous_line_end) {
        buffer->cursorIndex = target_index;
      } else {
        buffer->cursorIndex = previous_line_end;
      }
    }
  } else if (key == KEY_DOWN) {
    i32 column = get_column(buffer);
    i32 current_line_end = get_line_end(buffer, buffer->cursorIndex);
    if (buffer->text[current_line_end] != '\0') {
      i32 next_line_start = current_line_end + 1;
      i32 next_line_end = get_line_end(buffer, next_line_start);
      i32 target_index = next_line_start + column;
      if (target_index >= next_line_end) {
        buffer->cursorIndex = next_line_end;
      } else {
        buffer->cursorIndex = target_index;
      }
    }
  }
  if (buffer->debugFlag != 0){
      debbug_log(buffer);
  }
}

void handle_navigation(TextBuffer *buffer) {
  // 1. Single Press Logic
  if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) ||
      IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {

    int key = 0;
    if (IsKeyPressed(KEY_LEFT))
      key = KEY_LEFT;
    else if (IsKeyPressed(KEY_RIGHT))
      key = KEY_RIGHT;
    else if (IsKeyPressed(KEY_UP))
      key = KEY_UP;
    else if (IsKeyPressed(KEY_DOWN))
      key = KEY_DOWN;

    move_cursor(buffer, key);
    buffer->moveTimer = 0;
    if (buffer->debugFlag != 0){
      debbug_log(buffer);
    }
  }

  // 2. Rapid Hold Logic
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_UP) ||
      IsKeyDown(KEY_DOWN)) {

    buffer->moveTimer += GetFrameTime();
    if (buffer->moveTimer > 0.6) {
      // Check keys and move... (use your existing logic here)
      if (IsKeyDown(KEY_LEFT))
        move_cursor(buffer, KEY_LEFT);
      else if (IsKeyDown(KEY_RIGHT))
        move_cursor(buffer, KEY_RIGHT);
      else if (IsKeyDown(KEY_UP))
        move_cursor(buffer, KEY_UP);
      else if (IsKeyDown(KEY_DOWN))
        move_cursor(buffer, KEY_DOWN);

      buffer->moveTimer = 0.58;
    }
  } else {
    buffer->moveTimer = 0;
  }
  if (buffer->debugFlag != 0){
      debbug_log(buffer);
  }
}

// Process keyboard input and update the buffer state
void handle_input(TextBuffer *buffer) {
  handle_typing(buffer);

  if (IsKeyPressed(KEY_ENTER)) {
    buffer_insert(buffer, '\n');
    if (buffer->debugFlag != 0){
      debbug_log(buffer);
    }
  }
  if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_S)) {
    write_to_file(buffer);
  }

  if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_D)){
    if (buffer->debugFlag == 0){
      buffer->debugFlag = 1;
    } else {
      buffer->debugFlag = 0;
    }
  }
  handle_deletion(buffer);
  handle_navigation(buffer);
}


void draw_text_buffer(TextBuffer *buffer, int startX, int startY, int maxWidth) {
    float charWidth = MeasureTextEx(buffer->fonts[0], "M", 50, 0).x;
    int lineHeight = 50; 
    
    int currentX = startX;
    int currentY = startY;

    for (int i = 0; i <= buffer->letterCount; i++) {
        
        // 1. Draw Cursor
        if (i == buffer->cursorIndex) {
            // Force cursor visible if moving, otherwise blink
            if (buffer->moveTimer > 0 || (GetTime() - (int)GetTime()) < 0.5) {
                DrawRectangle(currentX, currentY, 2, lineHeight, (Color){255, 200, 0, 255});
            }
        }

        if (buffer->text[i] == '\0') break;

        char c = buffer->text[i];

        if (c == '\n') {
            currentX = startX;
            currentY += lineHeight;
        } 
        else {
            // ALWAYS CHECK WRAPPING NOW
            // If adding this char would push us past maxWidth...
            if (currentX + charWidth > startX + maxWidth) {
                currentX = startX;      // Reset to left
                currentY += lineHeight; // Move down
            }

            // Draw char
            char charStr[2] = { c, '\0' };
            DrawTextEx(buffer->fonts[0], charStr, (Vector2){(float)currentX, (float)currentY}, 50, 0, (Color){30, 30, 30, 255});

            currentX += (int)charWidth;
        }
    }
}
