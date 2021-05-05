#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

// #define DEBUG_ON

char *getExtensionName(char*);
bool doesFolderExist(char*);
bool isNull(char*);
int categorizeFile(char*);
void *__categorizeFile(void*);
void createFolder(char*, bool*);
void moveFileToFolder(char*, char*);
bool doesFileExist(char*);
int recusrivelyCategorizeFile(char*);
bool isHiddenFile(char*);
void getFileName(char*, char*);

/*
  Main function.
*/
int main(int argc, char *argv[]) {
  // Change main directory
  if (chdir("./") < 0) {
    exit(EXIT_FAILURE);
  }
  if (argc == 2) {
    if (strcmp(argv[1], "*") == 0) {
      int moved_files = recusrivelyCategorizeFile(".");
      if (moved_files == 0) {
        // Moved files is zero T w T, so it is failed
        printf("Yah, gagal disimpan :(\n");
      }
      else {
        // Yeah! atleast there is one file was moved!
        printf("Direktori sukses disimpan!\n");
      }
    }
  }
  else if (argc > 2) {
    if (strcmp(argv[1], "-f") == 0) {
      for (int i = 2; i < argc; i++) {
        int res = categorizeFile(argv[i]);
        if (res == 0) {
          printf("File %d : Sad, gagal :(\n", i - 1);
        }
        else {
          printf("File %d : Berhasil dikategorikan\n", i - 1);
        }
      }
    }
    else if (strcmp(argv[1], "-d") == 0) {
      int moved_files = recusrivelyCategorizeFile(argv[2]);
      if (moved_files == 0) {
        // Moved files is zero T w T, so it is failed
        printf("Yah, gagal disimpan :(\n");
      }
      else {
        // Yeah! atleast there is one file was moved!
        printf("Direktori sukses disimpan!\n");
      }
    }
  }
  return 0;
}

/*
  Categorize files (opening) by main thread.
*/
int categorizeFile(char *filePath) {
  // File does not exist
  if (!doesFileExist(filePath)) return 0;

  pthread_t tid;

  int err = pthread_create(&(tid), NULL, &(__categorizeFile), &filePath[0]);

  if (err != 0) {
    // There is error „?ƒ in creating thread
    exit(EXIT_FAILURE);
  }
  
  pthread_join(tid, NULL);
  return 1;
}

/*
  Categorize files (by thread)
*/
void *__categorizeFile(void *filePath) {
  char ext[16];
  if (isHiddenFile(filePath)) {
    strcpy(ext, "Hidden");
  }
  else {
    strcpy(ext, getExtensionName(filePath));
    // Alert! No extension?! „?ƒ Move it to Unknown folder right away
    if (isNull(ext)) {
      strcpy(ext, "Unknown");
    }
    else {
      // Lower extension (building consistency between JPG and jpg extension)
      for(int i = 0; ext[i] != '\0'; i++){
        ext[i] = tolower(ext[i]);
      }
    }
  }
  if (!doesFolderExist(ext)) {
    // Create the folder first
    bool status = false;
    createFolder(ext, &status);
    while(!status);
    #if defined DEBUG_ON
    printf("[Debug] Folder %s has been created\n", ext);
    #endif
  }
  moveFileToFolder(filePath, ext);
}

/*
  Get extension name from file path.
*/
char *getExtensionName(char *filePath) {
  char *dot = strchr(filePath, '.');
  if (!dot || dot == filePath) {
    // Extension not found
    return "";
  }
  return dot + 1;
}

/*
  Check if folder exist or not.
*/
bool doesFolderExist(char *folderPath) {
  DIR *dir = opendir(folderPath);
  if (dir) {
    closedir(dir);
    return true;
  }
  return false;
}

/*
  Check if string is null or not.
*/
bool isNull(char *str) {
  return (str[0] == '\0' ? true : false);
}

/*
  Create folder with busy waiting.
*/
void createFolder(char *folderPath, bool *trigger) {
  int check = mkdir(folderPath, 0777);
  // Directory creation successful
  if (!check)
    *trigger = true;
}

/*
  Move file to folder.
*/
void moveFileToFolder(char *filePath, char *folderPath) {
  FILE *fin = fopen(filePath, "rb");
  if (!fin) {
    // Failed to open file pointer
    exit(EXIT_FAILURE);
  }

  char newFilePath[1024];
  char fileName[256];
  getFileName(filePath, fileName);
  sprintf(newFilePath, "%s/%s", folderPath, fileName);

  FILE *fout = fopen(newFilePath, "wb");
  if (!fout) {
    // Failed to open file pointer
    exit(EXIT_FAILURE);
  }

  char ch;
  while (fscanf(fin, "%c", &ch) != EOF) {
    fprintf(fout, "%c", ch);
  }

  fclose(fout);

  fclose(fin);

  // Remove old file
  remove(filePath);

  /*
  pid_t cid;
  cid = fork();
  if (cid < 0) {
    // Child spawn failed.
    exit(EXIT_FAILURE);
  }
  if (cid == 0) {
    #if defined DEBUG_ON
    printf("[Debug] Moving file %s to %s\n", filePath, folderPath);
    #endif
    char *argv[] = {"mv", filePath, folderPath, NULL};
    execv("/bin/mv", argv);
  }
  */
}

/*
  Check if file exists or not.
*/
bool doesFileExist(char *filePath) {
  struct stat buffer;
  return (stat(filePath, &buffer) == 0);
}

/*
  Recursively categorize files.
*/
int recusrivelyCategorizeFile(char *basePath) {
  struct dirent *dp;
  DIR *dir = opendir(basePath);
  
  // Failed to open directory
  if (!dir) {
    return 0;
  }

  int count = 0;

  while ((dp = readdir(dir)) != NULL) {
    // Ignore . or ..
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;

    // Categorize file
    char path[1024];
    // -- This scope is not optimized UwU
    if (basePath[0] == '.' && basePath[1] == '\0') {
      sprintf(path, "%s", dp->d_name);
    }
    else {
      sprintf(path, "%s/%s", basePath, dp->d_name);
    }
    // --
    if (dp->d_type == DT_REG) {
      #if defined DEBUG_ON
      printf("[Debug] Categorizing %s\n", path);
      #endif
      int res = categorizeFile(path);
      if (res == 0) {
        // Fail, do nothing
      }
      else {
        count++;
        // File has been moved successfully, increment count! > w <
      }
    }
    else {
      count += recusrivelyCategorizeFile(path);
    }
  }

  return count;
}

/*
  Check for hidden filesI (`P¤P)`
  Hidden file has . on its name at index 0
  Compexity: O(N) while N is file path length
*/
bool isHiddenFile(char *filePath) {
  if (filePath[0] == '.' && filePath[1] != '/') return true;
  // Loop and look for /.
  int len = strlen(filePath);
  for (int i = 0; i < (len - 1); i++) {
    if(filePath[i] == '/' && filePath[i+1] == '.')
      return true;
  }
  return false;
}

/*
  Get file name.
*/
void getFileName(char *filePath, char *fileName) {
  int index = 0;
  for (int i = strlen(filePath) - 1; i >= 0; i--) {
    if (filePath[i] == '/') break;
    fileName[index++] = filePath[i];
  }
  for (int i = 0; i < index / 2; i++) {
    char temp;
    temp = fileName[index-i-1];
    fileName[index-i-1] = fileName[i];
    fileName[i] = temp;
  }
  fileName[index] = '\0';
}
