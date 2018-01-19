#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include "view.h"

class TextDisplay : public View {
  char **theDisplay;

 public:
  TextDisplay();
  ~TextDisplay();
  void notify(int r, int c, char ch);
   void print(std::ostream &out);
};

#endif
