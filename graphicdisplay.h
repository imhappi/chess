#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__
#include <iostream>
#include "view.h"
#include "window.h"

class GraphicDisplay : public View {
    Xwindow *window;
 public:
  GraphicDisplay();

  ~GraphicDisplay();

  void notify(int r, int c, char ch);

  /*
   * Prints the grid as specified in the assignment specification.
   */
   void print(std::ostream &out);
};

#endif
