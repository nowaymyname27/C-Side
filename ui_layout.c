#include "./clay/clay.h"
#include "raylib.h"
#include "buffer.h"

void layout_ui(TextBuffer *buffer) {
    Clay_SetLayoutDimensions((Clay_Dimensions){
        (float)GetScreenWidth(), 
        (float)GetScreenHeight()
    });

    Clay_BeginLayout();

    CLAY(
        CLAY_ID("OuterContainer"), 
        { 
            .layout = { 
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .childGap = 10,
                .padding = {10, 10}
            },
            .backgroundColor = {40, 40, 40, 255} 
        }
    ) {
        // Sidebar
        CLAY(
            CLAY_ID("Sidebar"), 
            { 
                .layout = { 
                    .sizing = { .width = CLAY_SIZING_FIXED(250), .height = CLAY_SIZING_GROW(0) } 
                },
                .backgroundColor = {70, 70, 70, 255} 
            }
        ) {}

        // Main Content Area
        CLAY(
            CLAY_ID("MainContent"), 
            { 
                .layout = { 
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } 
                },
                .backgroundColor = {20, 20, 20, 255} 
            }
        ) {}
    }

    // 1. Draw the UI Backgrounds
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    Clay_Raylib_Render(renderCommands, &buffer->editorFont);

    // 2. Draw the Text Editor ON TOP of the MainContent box
    // Sidebar (250) + Outer Padding (10) + Gap (10) = 270 X
    // Outer Padding (10) = 10 Y
    // We add a tiny bit of internal padding (+10) so text isn't glued to the edge
    draw_text_buffer(buffer, 280, 20);
}
