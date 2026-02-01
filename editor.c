#include "buffer.h"
#include "raylib.h"
#include <stdio.h>

// Inserts char into where the cursorIndex is
void buffer_insert(TextBuffer *buffer, char c) {
  for (int i = buffer->letterCount; i >= buffer->cursorIndex; i--) {
    buffer->text[i + 1] = buffer->text[i];
  }
  buffer->text[buffer->cursorIndex] = c;
  buffer->letterCount++;
  buffer->cursorIndex++;
}

void buffer_delete(TextBuffer *buffer) {
  for (int i = buffer->cursorIndex; i <= buffer->letterCount; i++) {
    buffer->text[i - 1] = buffer->text[i];
  }
  buffer->letterCount--;
  buffer->cursorIndex--;
}

int get_line_start(TextBuffer *buffer, int from_index) {
  int i = from_index - 1;
  while (i >= 0 && buffer->text[i] != '\n') {
    i--;
  }
  return i + 1;
}

int get_line_end(TextBuffer *buffer, int from_index) {
  int i = from_index;
  while (buffer->text[i] != '\n' && buffer->text[i] != '\0') {
    i++;
  }
  return i;
}

int get_column(TextBuffer *buffer) {
  int line_start = get_line_start(buffer, buffer->cursorIndex);
  return buffer->cursorIndex - line_start;
}

void debbug_log(TextBuffer *buffer) {
  int line_start = get_line_start(buffer, buffer->cursorIndex);
  int column = get_column(buffer);
  printf("text: %s \n", buffer->text);
  printf("letterCount: %d \n", buffer->letterCount);
  printf("cursorIndex: %d \n", buffer->cursorIndex);
  printf("line_start: %d \n", line_start);
  printf("column: %d \n", column);
}

// Process keyboard input and update the buffer state
void handle_input(TextBuffer *buffer) {
  // GetCharPressed returns characters from the internal Raylib queue
  int key = GetCharPressed();

  // Drain the queue to handle multiple keypresses in one frame
  while (key != 0) {
    buffer_insert(buffer, (char)key);
    key = GetCharPressed();
    debbug_log(buffer);
  }

  // Handles new line: Pressing ENTER allows for text to go to new line
  if (IsKeyPressed(KEY_ENTER)) {
    buffer_insert(buffer, '\n');
    buffer->timer = 0;
    debbug_log(buffer);
  }

  // Handle deletions: Decrease count and move the 'stop sign' back
  if (IsKeyPressed(KEY_BACKSPACE) && buffer->cursorIndex > 0) {
    buffer_delete(buffer);
    buffer->timer = 0;
    debbug_log(buffer);
  }

  // Handle Deletions: When key is held deletes rapidly
  if (IsKeyDown(KEY_BACKSPACE) && buffer->cursorIndex > 0) {
    debbug_log(buffer);
    buffer->timer += GetFrameTime();
    if (buffer->timer > 0.6) {
      buffer_delete(buffer);
      buffer->timer = 0.58;
    }
  } else {
    buffer->timer = 0;
  }

  if (IsKeyPressed(KEY_LEFT) && buffer->cursorIndex > 0) {
    buffer->cursorIndex--;
    buffer->timer = 0;
    debbug_log(buffer);
  }

  if (IsKeyPressed(KEY_RIGHT) && buffer->cursorIndex < buffer->letterCount) {
    buffer->cursorIndex++;
    buffer->timer = 0;
    debbug_log(buffer);
  }

  if (IsKeyPressed(KEY_UP)) {
    int column = get_column(buffer);
    int current_line_start = get_line_start(buffer, buffer->cursorIndex);

    if (current_line_start != 0) {
      int previous_line_start = get_line_start(buffer, current_line_start - 1);
      int previous_line_end = get_line_end(buffer, previous_line_start);
      int target_index = previous_line_start + column;
      if (target_index < previous_line_end) {
        buffer->cursorIndex = target_index;
      } else {
        buffer->cursorIndex = previous_line_end;
      }
    }
  }

  if (IsKeyPressed(KEY_DOWN)) {
    int column = get_column(buffer);
    int current_line_end = get_line_end(buffer, buffer->cursorIndex);
    if (buffer->text[current_line_end] != '\0') {
      int next_line_start = current_line_end + 1;
      int next_line_end = get_line_end(buffer, next_line_start);
      int target_index = next_line_start + column;
      if (target_index >= next_line_end) {
        buffer->cursorIndex = next_line_end;
      } else {
        buffer->cursorIndex = target_index;
      }
    }
  }

  /* printf("Column: %d, current_line_start: %d \n", column,
   * current_line_start); */
}

// Render the editor state to the window
void draw_to_screen(TextBuffer *buffer) {
  BeginDrawing();
  ClearBackground(BLACK);
  // Draw the main text buffer
  DrawText(buffer->text, 58, 50, 50, RED);

  // Calculate cursor horizontal position based on text width
  int line_start = get_line_start(buffer, buffer->cursorIndex);

  char tempChar = buffer->text[buffer->cursorIndex];
  buffer->text[buffer->cursorIndex] = '\0';
  int width = MeasureText(&buffer->text[line_start], 50);
  buffer->text[buffer->cursorIndex] = tempChar;

  // Calculate cursor vertical position
  int height = 0;
  int counter = 0;
  while (counter < buffer->cursorIndex) {
    if (buffer->text[counter] == '\n') {
      height++;
      counter++;
    } else {
      counter++;
    }
  }

  // Blinking logic: Draw cursor only during the first half of every second
  int LineHeight = 52;
  int whole_seconds = (int)GetTime();
  if (GetTime() - whole_seconds < 0.5) {
    DrawRectangle(57 + width, 50 + (height * LineHeight), 2, 45, PINK);
  }
  EndDrawing();
}
