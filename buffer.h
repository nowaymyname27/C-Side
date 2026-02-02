#ifndef BUFFER_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;   // 0 to 255
typedef uint16_t u16; // 0 to 65,535
typedef uint32_t u32; // 0 to 4.2 Billion
typedef uint64_t u64; // 0 to 18 Quintillion

typedef int8_t i8;   // -128 to 127
typedef int16_t i16; // -32,768 to 32,767
typedef int32_t i32; // -2.1 Billion to 2.1 Billion
typedef int64_t i64; // The really big stuff

typedef float f32;  // Standard for 3D/Graphics (Raylib uses this)
typedef double f64; // High precision math

typedef size_t usize;
typedef intptr_t isize;

typedef bool b8;
typedef int32_t b32; // Used when you want the boolean to take exactly 4 bytes

#define BUFFER_H
#define BUFFER_SIZE 2049


typedef struct {
  char text[BUFFER_SIZE];
  Font editorFont;
  i32 letterCount;
  i32 cursorIndex;

  // Timers
  f32 backspaceTimer;
  f32 moveTimer;
} TextBuffer;

void write_to_file(TextBuffer *buffer);
void open_file(TextBuffer *buffer);

void draw_text_buffer(TextBuffer *buffer, int offX, int offY);

#endif
