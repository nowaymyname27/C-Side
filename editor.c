#include "buffer.h"
#include "raylib.h"
#include <stdio.h>

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
        debbug_log(buffer);
    }
}

void handle_deletion(TextBuffer *buffer) {
    // 1. Single Press
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (buffer->cursorIndex > 0) {
            buffer_delete(buffer);
            buffer->backspaceTimer = 0; // Reset timer on fresh press
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
}

void handle_navigation(TextBuffer *buffer) {
    // 1. Single Press Logic
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || 
        IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        
        int key = 0;
        if (IsKeyPressed(KEY_LEFT)) key = KEY_LEFT;
        else if (IsKeyPressed(KEY_RIGHT)) key = KEY_RIGHT;
        else if (IsKeyPressed(KEY_UP)) key = KEY_UP;
        else if (IsKeyPressed(KEY_DOWN)) key = KEY_DOWN;

        move_cursor(buffer, key);
        buffer->moveTimer = 0;
    }

    // 2. Rapid Hold Logic
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || 
        IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {
        
        buffer->moveTimer += GetFrameTime();
        if (buffer->moveTimer > 0.6) {
            // Check keys and move... (use your existing logic here)
            if (IsKeyDown(KEY_LEFT)) move_cursor(buffer, KEY_LEFT);
            else if (IsKeyDown(KEY_RIGHT)) move_cursor(buffer, KEY_RIGHT);
            else if (IsKeyDown(KEY_UP)) move_cursor(buffer, KEY_UP);
            else if (IsKeyDown(KEY_DOWN)) move_cursor(buffer, KEY_DOWN);
            
            buffer->moveTimer = 0.58;
        }
    } else {
        buffer->moveTimer = 0;
    }
}



// Process keyboard input and update the buffer state
void handle_input(TextBuffer *buffer) {
    handle_typing(buffer);

    if (IsKeyPressed(KEY_ENTER)) {
        buffer_insert(buffer, '\n');
        debbug_log(buffer);
    }

    handle_deletion(buffer);
    handle_navigation(buffer);
}


// Render the editor state to the window
void draw_to_screen(TextBuffer *buffer) {
  BeginDrawing();
  ClearBackground(BLACK);
  // Draw the main text buffer
  DrawTextEx(buffer->editorFont, buffer->text, (Vector2){58.0f, 50.0f}, 50, 0,
             RED);

  // Calculate cursor horizontal position based on text width
  i32 line_start = get_line_start(buffer, buffer->cursorIndex);

  char tempChar = buffer->text[buffer->cursorIndex];
  buffer->text[buffer->cursorIndex] = '\0';
  Vector2 size =
      MeasureTextEx(buffer->editorFont, &buffer->text[line_start], 50, 0);
  i32 width = (i32)size.x;
  buffer->text[buffer->cursorIndex] = tempChar;

  // Calculate cursor vertical position
  i32 height = 0;
  i32 counter = 0;
  while (counter < buffer->cursorIndex) {
    if (buffer->text[counter] == '\n') {
      height++;
      counter++;
    } else {
      counter++;
    }
  }

  // Blinking logic: Draw cursor only during the first half of every second
  i32 LineHeight = 52;
  i32 whole_seconds = (int)GetTime();
  if (GetTime() - whole_seconds < 0.5) {
    DrawRectangle(57 + width, 50 + (height * LineHeight), 2, 45, PINK);
  }
  EndDrawing();
}
