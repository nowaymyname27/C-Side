#include "buffer.h"
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Update to accept a filename, or keep hardcoded but add folder
void write_to_file(TextBuffer *buffer) {
  if (strlen(buffer->currentFilename)){
    printf("No file open! Cannot save.\n");
    return;
  }
  char filepath[128];
  sprintf(filepath, "notes/%s", buffer->currentFilename); // Or use a variable filename

  FILE *f = fopen(filepath, "w");
  if (f != NULL) {
      fprintf(f, "%s", buffer->text);
      printf("Saved to: %s\n", filepath);
      fclose(f);
  }
}

void open_file(TextBuffer *buffer, char *filename) {
  // 1. Construct the path
  char filepath[128]; 
  sprintf(filepath, "notes/%s", filename); // glues "notes/" + "test.txt"

  // 2. Open using the full path
  FILE *f = fopen(filepath, "r");
  
  if (f != NULL) {
    fseek(f, 0, SEEK_END);
    buffer->letterCount = ftell(f);
    rewind(f);
    fread(buffer->text, 1, buffer->letterCount, f);
    buffer->text[buffer->letterCount] = '\0';
    buffer->cursorIndex = buffer->letterCount; // Move cursor to end
    printf("Opened: %s\n", filepath);
    fclose(f);
  } else {
    printf("Failed to open: %s\n", filepath);
  }
}

void scan_directory(TextBuffer *buffer) {
  DIR *d = opendir("notes");

  if (d == NULL) {
    printf("Could not open directory!\n");
    return;
  }

  struct dirent *dir;

  // Safety: Reset count before scanning
  buffer->fileCount = 0;

  while ((dir = readdir(d)) != NULL) {

    // 1. Check extension
    if (strstr(dir->d_name, ".txt") != NULL) {

      // 2. Safety Check: Do we have room?
      if (buffer->fileCount < MAX_FILES) {

        // 3. Copy into the CURRENT count slot
        strncpy(buffer->files[buffer->fileCount], dir->d_name,
                MAX_FILENAME_LEN);

        // 4. Increment count for next time
        buffer->fileCount++;

      } else {
        printf("Too many files! Skipped: %s\n", dir->d_name);
      }
    }
  }

  closedir(d);
}
