#include "buffer.h"
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Update to accept a filename, or keep hardcoded but add folder
void write_to_file(TextBuffer *buffer) {
  if (strlen(buffer->currentFilename) == 0){
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
  char filepath[128]; 
  sprintf(filepath, "notes/%s", filename); 

  FILE *f = fopen(filepath, "r");
  
  if (f != NULL) {
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f); // Get raw size
    rewind(f);

    // Clamp size to buffer limit
    if (filesize >= BUFFER_SIZE) {
        printf("Warning: File too large! Truncating.\n");
        filesize = BUFFER_SIZE - 1; // Leave room for \0
    }

    buffer->letterCount = (i32)filesize;
    
    // Read only the safe amount
    fread(buffer->text, 1, buffer->letterCount, f);
    
    buffer->text[buffer->letterCount] = '\0';
    buffer->cursorIndex = buffer->letterCount;
    
    strncpy(buffer->currentFilename, filename, MAX_FILENAME_LEN);
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

void create_new_file(TextBuffer *buffer) {
    char filename[64];
    int counter = 1;

    // 1. Find a unique name
    // Loop until we find a name that DOESN'T exist
    while (true) {
        sprintf(filename, "Untitled_%d.txt", counter);
        
        char filepath[128];
        sprintf(filepath, "notes/%s", filename);
        
        // Try to open it in Read mode just to check if it exists
        FILE *check = fopen(filepath, "r");
        if (check == NULL) {
            // File doesn't exist! This name is free.
            break; 
        }
        fclose(check);
        counter++;
    }

    // 2. Create the file (Write empty string)
    char filepath[128];
    sprintf(filepath, "notes/%s", filename);
    FILE *f = fopen(filepath, "w");
    if (f) {
        fprintf(f, ""); // Empty file
        fclose(f);
        printf("Created new file: %s\n", filename);
    }

    // 3. Update UI
    scan_directory(buffer);

    // 4. Open it
    open_file(buffer, filename);
}
