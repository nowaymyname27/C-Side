// ui_layout.c
#include "./clay/clay.h"
#include "buffer.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

// 1. Helper to convert C strings (char*) to Clay strings
Clay_String AsClayString(char *c) {
  return (Clay_String){.length = (int)strlen(c), .chars = c};
}

// 2. Forward Declaration for the Renderer
// (This tells the compiler what the function looks like since it lives in a .c
// file)
void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands, Font *fonts);

// Component: Sidebar Button
void RenderSidebarButton(TextBuffer *buffer, char *filename) {
  // We convert the filename to a ClayString once to use it multiple times
  Clay_String clayName = AsClayString(filename);

  // We use the ID based on the string content
  Clay_ElementId elementId = Clay_GetElementId(clayName);

  bool isActive = (strcmp(buffer->currentFilename, filename) == 0);

  CLAY(elementId, (Clay_ElementDeclaration){
    .layout = {
      .sizing = {
        .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(40)
      },
      .padding = {10, 10, 10, 10},
      .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}
    },
    // Interaction: Check hover state using the ID
    .backgroundColor = 
      isActive
      ? (Clay_Color){255, 90, 90, 255} // rgb(255,90,90)
      : Clay_PointerOver(elementId) 
        ? (Clay_Color){255, 100, 100, 255} // rgb(255,100,100)
        : (Clay_Color){60, 60, 60, 255}, 
    .cornerRadius = CLAY_CORNER_RADIUS(8)
  })

  CLAY_TEXT(clayName, // Use the converted string
      CLAY_TEXT_CONFIG({
        .fontSize = 20, 
        .textColor = {200, 200, 200, 255}, // rgb(200,200,200)
        .fontId = 0}));

  // Interaction Logic: Click to Open
  if (Clay_PointerOver(elementId) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    printf("Clicked on: %s\n", filename);
    open_file(buffer, filename);
  }
}

void layout_ui(TextBuffer *buffer) {
  // 1. Setup Layout
  Clay_SetLayoutDimensions(
      (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()});

  // 2. Mouse Input
  Vector2 mouse = GetMousePosition();
  Clay_SetPointerState((Clay_Vector2){mouse.x, mouse.y},
                       IsMouseButtonDown(MOUSE_LEFT_BUTTON));

  Clay_BeginLayout();

  // --- ROOT CONTAINER ---
  CLAY(
      CLAY_ID("Outer"),
      (Clay_ElementDeclaration){
        .layout = {
          .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
          .layoutDirection = CLAY_LEFT_TO_RIGHT},
          .backgroundColor = {255, 229, 244, 255} // rgb(255,229,244)
       }) 
  {
  // --- SIDEBAR ---
  CLAY(
    CLAY_ID("Sidebar"), 
    (Clay_ElementDeclaration){
      .layout = {
        .sizing = {
          .width = CLAY_SIZING_FIXED(250),
          .height = CLAY_SIZING_GROW(0)},
          .padding = {10, 10, 10, 10},
          .childGap = 5,
          .layoutDirection = CLAY_TOP_TO_BOTTOM},
          .backgroundColor = {255, 66, 66, 255} // rgb(255,66,66)
         })  
  {
    // sidebar buttons loop here 
    for (int i = 0; i < buffer->fileCount; i++) {
      RenderSidebarButton(buffer, buffer->files[i]);
    }
  }
    // --- EDITOR AREA ---
    // We give this an ID so we can find it later!
    CLAY(
      CLAY_ID("EditorArea"),
        (Clay_ElementDeclaration){
          .layout = {
            .sizing = {
              .width = CLAY_SIZING_GROW(0),
              .height = CLAY_SIZING_GROW(0)},
              .padding = {20, 20, 20, 20} // Nice padding for text
          },
          .backgroundColor = {255, 229, 244, 255} }) {} // rgb(255,229,244)
  }

  // 3. Render the Layout
  Clay_RenderCommandArray renderCommands = Clay_EndLayout();
  Clay_Raylib_Render(renderCommands, buffer->fonts);

  // Sidebar (250) + Padding (20)
  int editorX = 250 + 20;
  int editorY = 20;

  // Draw your logic inside that calculated space
  draw_text_buffer(buffer, editorX, editorY);
}
