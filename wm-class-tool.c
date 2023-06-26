#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int eq(char *c1, char *c2);
int getSetWindowClass(int cmd, char *windowIdHexStr, char *wmClass);
Window getWindowId(char *windowIdHexStr);

int CMD_GET = 1;
int CMD_SET = 2;

const char* USAGE = ""
  "Usage:\n"
  "  wm-class-tool -h|--help\n"
  "  wm-class-tool WINDOW_ID_HEX\n"
  "  wm-class-tool -g WINDOW_ID_HEX\n"
  "    print class property of 'WM_CLASS'\n"
  "\n"
  "  wm-class-tool WINDOW_ID_HEX TARGET_WM_CLASS\n"
  "  wm-class-tool -s WINDOW_ID_HEX TARGET_WM_CLASS\n"
  "    set class property of 'WM_CLASS' to TARGET_WM_CLASS\n"
;

int main(int argc, char *argv[]) {
  int cmd;
  char *windowIdHexStr;
  char *wmClass;
  if(argc == 2 && (eq(argv[1], "-h") || eq(argv[1], "--help"))){
    printf(USAGE);
    return 0;
  }else if(argc == 2){
    cmd = CMD_GET;
    windowIdHexStr = argv[1];
  }else if(argc == 3 && eq(argv[1], "-g")){
    cmd = CMD_GET;
    windowIdHexStr = argv[2];
  }else if(argc == 3){
    cmd = CMD_SET;
    windowIdHexStr = argv[1];
    wmClass = argv[2];
  }else if(argc == 4 && eq(argv[1], "-s")){
    cmd = CMD_SET;
    windowIdHexStr = argv[2];
    wmClass = argv[3];
  }else{
    printf(USAGE);
    return 1;
  }

  if(getSetWindowClass(cmd, windowIdHexStr, wmClass) != 0){
    printf("ERROR: obtaining window class failed\n");
    return 1;
  }

  return 0;
}

int eq(char *c1, char *c2){
  return strcmp(c1, c2) == 0;
}

int getSetWindowClass(int cmd, char *windowIdStrHex, char *wmClass){
  Window windowId = getWindowId(windowIdStrHex);
  if(windowId == 0){
    printf("ERROR: could not parse window id: %s\n", windowIdStrHex);
    return -1;
  }

  Display *display = XOpenDisplay(NULL);
  XClassHint classHint;
  Status status = XGetClassHint(display, windowId, &classHint);
  if(!status){
    return -1;
  }

  if(cmd == CMD_GET){
    printf("%s\n", classHint.res_class);
  }else if(cmd == CMD_SET){
    printf("WM_CLASS.class %s \"%s\" => \"%s\"\n", windowIdStrHex, classHint.res_class, wmClass);
    XFree(classHint.res_class);
    classHint.res_class = strdup(wmClass);
    XSetClassHint(display, windowId, &classHint);
  }

  XCloseDisplay(display);
  XFree(classHint.res_name);
  XFree(classHint.res_class);

  return 0;
}

Window getWindowId(char *windowIdHexStr){
  char *pEnd;
  Window windowId = strtoul(windowIdHexStr, &pEnd, 16);
  if (strlen(pEnd) != 0){
    return 0;
  }
  return windowId;
}
