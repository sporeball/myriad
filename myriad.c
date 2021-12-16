/*
 * 法的な理由により、ゲームの名前はMyriadに変更されました。
 * それは難しい！　がんばれ！
 *
 * instructions
 * Windows: try to compile under Windows, give up and use the Unix instructions with WSL (requires libncursesw5-dev)
 * Mac: no fucking idea
 * Unix: gcc -Wall myriad.c -o myriad -lm -lncurses && ./myriad
 *
 * manipulate your pieces and fill the whole board to win
 * you cannot place pieces over the white stars
 * there are 6144 puzzles to solve; restart the program to get another one
 *
 * placement input is taken as a coordinate pair e.g. "0,0"
 * send an empty input to cancel
 * invalid input should hopefully just be ignored instead of causing a segfault
 *
 * all puzzles should be solvable, unless I fucked up
 * if you want proof that it is possible to win the game, comment line 551, uncomment line 549 and follow the instructions there
 *
 * this submission fixes 1 bug found in the first submission
 */

#include <math.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> // Abi in malam crucem!

#define nulo -1
#define gris 8
#define rojo 9
#define verde 10
#define amarillo 11
#define azul 12
#define magenta 13
#define cian 14
#define blanco 15

#define non 0
#define rien 0
#define premier 0
#define eteint 0
#define bon 0
#define oui 1
#define mauvais 1

int w, h, cx, cy, ix, iy;
int tema;
int cur = 0;
int placing = non;

int *temp;

char id_str[12];
char cur_str[9];
char in[12];

struct grid {
  int data[16];
  // nooen o teh below prop[reties are used fro the board grids because Fuck you
  int color;
  int bold;
  char repr[1];
  int placed;
};

// black grids only, to save time
struct grid grid_1 = {
  {
     -1, 999,  -1, 999,
     -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1
  },
  nulo, non, " ", 0
};
struct grid grid_2 = {
  {
     -1,  -1,  -1,  -1,
     -1, 999,  -1,  -1,
     -1,  -1,  -1,  -1,
    999,  -1,  -1,  -1
  },
  nulo, non, " ", 0
};
struct grid grid_3 = {
  {
    999,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,
     -1, 999,  -1,  -1,
     -1,  -1,  -1,  -1
  },
  nulo, non, " ", 0
};
struct grid grid_4 = {
  {
     -1,  -1,  -1,  -1,
    999,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1
  },
  nulo, non, " ", 0
};

struct grid piece_1 = {
  {
       1,    1, -999, -999,
    -999,    1,    1, -999,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  rojo, non, "a", 0
};
struct grid piece_2 = {
  {
       2, -999, -999, -999,
       2,    2,    2,    2,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  rojo, oui, "A", 0
};
struct grid piece_3 = {
  {
       3, -999,    3, -999,
       3,    3,    3, -999,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  verde, non, "b", 0
};
struct grid piece_4 = {
  {
       4,    4,    4, -999,
       4,    4, -999, -999,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  verde, oui, "B", 0
};
struct grid piece_5 = {
  {
    -999,    5, -999, -999,
    -999,    5,    5, -999,
       5,    5, -999, -999,
    -999, -999, -999, -999
  },
  amarillo, non, "c", 0
};
struct grid piece_6 = {
  {
       6,    6,    6,    6,
    -999,    6, -999, -999,
    -999, -999, -999, -999,
    -999, -999, -999, -999,
  },
  amarillo, oui, "C", 0
};
struct grid piece_7 = {
  {
    -999,    7,    7, -999,
    -999,    7, -999, -999,
       7,    7, -999, -999,
    -999, -999, -999, -999
  },
  azul, non, "d", 0
};
struct grid piece_8 = {
  {
       8, -999, -999, -999,
       8, -999, -999, -999,
       8,    8,    8, -999,
    -999, -999, -999, -999
  },
  azul, oui, "D", 0
};
struct grid piece_9 = {
  {
    -999, -999,    9, -999,
    -999,    9,    9, -999,
       9,    9, -999, -999,
    -999, -999, -999, -999
  },
  magenta, non, "e", 0
};
struct grid piece_10 = {
  {
      10,   10,   10, -999,
    -999, -999,   10,   10,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  magenta, oui, "E", 0
};
struct grid piece_11 = {
  {
    -999,   11, -999, -999,
    -999,   11, -999, -999,
      11,   11,   11, -999,
    -999, -999, -999, -999
  },
  cian, non, "f", 0
};
struct grid piece_12 = {
  {
      12,   12, -999, -999,
    -999,   12, -999, -999,
    -999, -999, -999, -999,
    -999, -999, -999, -999
  },
  cian, oui, "F", 0
};

// C'est difficile.
struct grid *permutations[24][4] = {
  {&grid_1, &grid_2, &grid_3, &grid_4},
  {&grid_1, &grid_2, &grid_4, &grid_3},
  {&grid_1, &grid_3, &grid_2, &grid_4},
  {&grid_1, &grid_3, &grid_4, &grid_2},
  {&grid_1, &grid_4, &grid_2, &grid_3},
  {&grid_1, &grid_4, &grid_3, &grid_2},
  {&grid_2, &grid_1, &grid_3, &grid_4},
  {&grid_2, &grid_1, &grid_4, &grid_3},
  {&grid_2, &grid_3, &grid_1, &grid_4},
  {&grid_2, &grid_3, &grid_4, &grid_1},
  {&grid_2, &grid_4, &grid_1, &grid_3},
  {&grid_2, &grid_4, &grid_3, &grid_1},
  {&grid_3, &grid_1, &grid_2, &grid_4},
  {&grid_3, &grid_1, &grid_4, &grid_2},
  {&grid_3, &grid_2, &grid_1, &grid_4},
  {&grid_3, &grid_2, &grid_4, &grid_1},
  {&grid_3, &grid_4, &grid_1, &grid_2},
  {&grid_3, &grid_4, &grid_2, &grid_1},
  {&grid_4, &grid_1, &grid_2, &grid_3},
  {&grid_4, &grid_1, &grid_3, &grid_2},
  {&grid_4, &grid_2, &grid_1, &grid_3},
  {&grid_4, &grid_2, &grid_3, &grid_1},
  {&grid_4, &grid_3, &grid_1, &grid_2},
  {&grid_4, &grid_3, &grid_2, &grid_1}
};
struct grid **permutation; // The Chosen One
struct grid *pieces[12] = {
  &piece_1, &piece_2, &piece_3, &piece_4,  &piece_5,  &piece_6,
  &piece_7, &piece_8, &piece_9, &piece_10, &piece_11, &piece_12
};

void mvclr(int x, int y) {
  move(y, x);
  clrtoeol();
}

void rotg(struct grid *g, int times) {
  for (int i = 0; i < 3 * times; i++) {
    memcpy(temp, g->data, sizeof(g->data)); // evil floating point bit level hacking
    for (int i = 0; i < 16; i++) {
      int real = (int) 4 * (i % 4) - floor(i / 4) + 3; // what the fuck?
      g->data[i] = temp[real];
    }
  }
}

// natural extensions
void hflg(struct grid *g) {
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    int real = (int) 4 * floor(i / 4) - (i % 4) + 3;
    g->data[i] = temp[real];
  }
}
void vflg(struct grid *g) {
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    int real = (int) 15 - (4 * floor(i / 4)) - (3 - (i % 4));
    g->data[i] = temp[real];
  }
}

// who cares about being DRY?
void unjv(struct grid *g, int v) {
  for (int i = 0; i < 16; i++) {
    if ((int) floor(i / 4) == 0 && g->data[i] == v) {
      return;
    }
  }
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    g->data[i] = -999;
  }
  for (int i = 0; i < 16; i++) {
    if (temp[i] == v) {
      g->data[i - 4] = v;
    }
  }
}
void rnjv(struct grid *g, int v) {
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 3 && g->data[i] == v) {
      return;
    }
  }
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    g->data[i] = -999;
  }
  for (int i = 0; i < 16; i++) {
    if (temp[i] == v) {
      g->data[i + 1] = v;
    }
  }
}
void dnjv(struct grid *g, int v) {
  for (int i = 0; i < 16; i++) {
    if ((int) floor(i / 4) == 3 && g->data[i] == v) {
      return;
    }
  }
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    g->data[i] = -999;
  }
  for (int i = 0; i < 16; i++) {
    if (temp[i] == v) {
      g->data[i + 4] = v;
    }
  }
}
void lnjv(struct grid *g, int v) {
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0 && g->data[i] == v) {
      return;
    }
  }
  memcpy(temp, g->data, sizeof(g->data));
  for (int i = 0; i < 16; i++) {
    g->data[i] = -999;
  }
  for (int i = 0; i < 16; i++) {
    if (temp[i] == v) {
      g->data[i - 1] = v;
    }
  }
}

void initclr() {
  init_pair(gris, gris, -1);
  init_pair(rojo, rojo, -1);
  init_pair(verde, verde, -1);
  init_pair(amarillo, amarillo, -1);
  init_pair(azul, azul, -1);
  init_pair(magenta, magenta, -1);
  init_pair(cian, cian, -1);
  init_pair(blanco, blanco, -1);
}

void printk(char *str, int color, int bold) {
  if (bold == oui) {
    attron(COLOR_PAIR(color) | A_BOLD);
  } else {
    attron(COLOR_PAIR(color));
  }
  printw(str);
  refresh();
  attroff(COLOR_PAIR(color) | A_BOLD);
}

void rtnbd() {
  for (int grid = 0; grid < 4; grid++) {
    for (int cell = 0; cell < 16; cell++) {
      // Re: Horrible syntax
      unsigned xa = grid & 1;
      unsigned ya = (grid & 2) >> 1;
      int x = (cx - 8) + (2 * (cell % 4)) + (8 * (int) xa);
      int y = (cy - 8) + (int) floor(cell / 4) + (4 * (int) ya) + 1;
      move(y, x);
      // Congratulation! You just invented APL!
      if (permutation[grid]->data[cell] == -1) {
        printk(".", gris, non);
      } else {
        printw("*");
      }
    }
  }
}
void rtncr() {
  mvclr(cx - 9, cy + 2);
  printk("current: ", tema, 1);
  sprintf(cur_str, "piece %d", cur + 1);
  printw(cur_str);
}
void rtncg() {
  struct grid *now = pieces[cur];
  for (int cell = 0; cell < 16; cell++) {
    int x = (cx - 4) + (2 * (cell % 4));
    int y = (cy + 3) + (int) floor(cell / 4);
    move(y, x);
    if (now->data[cell] == -999) {
      printk(".", gris, non);
    } else {
      printk(now->repr, now->color, now->bold);
    }
  }
}
void rtnhp() {
  // one
  move(cy + 7, cx - 24);
  printk("z/x    ", tema, oui);
  printw("cycle piece             ");
  printk("j      ", tema, oui);
  printw("flip horiz");
  // two
  move(cy + 8, cx - 24);
  printk("q/e    ", tema, oui);
  printw("rotate                  ");
  printk("k      ", tema, oui);
  printw("flip vert");
  // three
  move(cy + 9, cx - 24);
  clrtoeol();
  printk("wasd   ", tema, oui);
  printw("nudge                   ");
  printk("Space  ", tema, oui);
  if (pieces[cur]->placed == 0) {
    printw("place down");
  } else {
    printw("pick up");
  }
}
void rtnwn() {
  for (int grid = 0; grid < 4; grid++) {
    for (int cell = 0; cell < 16; cell++) {
      unsigned xa = grid & 1;
      unsigned ya = (grid & 2) >> 1;
      int x = (cx - 8) + (2 * (cell % 4)) + (8 * (int) xa);
      int y = (cy - 8) + (int) floor(cell / 4) + (4 * (int) ya) + 1;
      move(y, x);
      char there = inch() & A_CHARTEXT;
      if (there == '.') {
        return;
      }
    }
  }
  move(cy + 10, cx - 9);
  printk("congratulations!", amarillo, oui);
}
void rtnpl() {
  struct grid *now = pieces[cur];
  int x;
  int y;
  for (int cell = 0; cell < 16; cell++) {
    x = (cx - 8) + (2 * (cell % 4)) + (2 * ix);
    y = (cy - 8) + (int) floor(cell / 4) + iy + 1;
    move(y, x);
    char under = inch() & A_CHARTEXT;
    if (now->data[cell] == cur + 1 && under != '.') {
      return;
    }
  }
  // Again.
  for (int cell = 0; cell < 16; cell++) {
    x = (cx - 8) + (2 * (cell % 4)) + (2 * ix);
    y = (cy - 8) + (int) floor(cell / 4) + iy + 1;
    move(y, x);
    if (now->data[cell] > 0) {
      printk(now->repr, now->color, now->bold);
    }
  }
  pieces[cur]->placed = 1;
  rtnhp();
  rtnwn();
}
void rtnpk() {
  struct grid *now = pieces[cur];
  for (int cell = 0; cell < 64; cell++) {
    int x = (cx - 8) + (2 * (cell % 8));
    int y = (cy - 8) + (int) floor(cell / 8) + 1;
    move(y, x);
    char there = inch() & A_CHARTEXT;
    if (there == now->repr[0]) {
      printk(".", gris, non);
    }
  }
  pieces[cur]->placed = 0;
  rtnwn();
  rtnhp();
}
void rtnin() {
  echo();
  curs_set(1);
  move(cy + 10, cx + 14);
  printw("where? ");
  getstr(in);
  move(cy + 10, cx - 12);
  clrtoeol();
  noecho();
  curs_set(0);
  // do validation to it
  if (strstr(in, ",")) {
    ix = atoi(strtok(in, ","));
    iy = atoi(strtok(NULL, ","));
    if (ix >= 0 && ix < 5 && iy >= 0 && iy < 5) {
      rtnpl();
    }
  }
}

int main() {
  initscr();
  raw();
  curs_set(eteint);
  if (has_colors() == non) {
    endwin();
    printf("myriad requires color support");
    return mauvais;
  }
  use_default_colors();
  start_color();
  initclr();
  keypad(stdscr, oui);
  noecho();

  getmaxyx(stdscr, h, w);
  if (w < 48 || h < 24) {
    endwin();
    printf("myriad requires at least 24 rows and 48 columns\n");
    return mauvais;
  }
  cx = (int) floor(w / 2);
  cy = (int) floor(h / 2);

  temp = malloc(sizeof(grid_1.data));

  // Seedy Business
  srand((unsigned) time(NULL));

  // j 3,1
  // xjkww 4,0
  // x 0,4
  // xj 3,4
  // xea 0,1
  // xj 0,0
  // x 2,1
  // xjs 4,4
  // xs 1,4
  // xjss 3,4
  // xqd 4,1
  // xjaass 0,4
  /* int id = 2096; */

  int id = rand() % 6144;

  sprintf(id_str, "puzzle %d", id);
  unsigned r = (unsigned) id % 256;
  permutation = permutations[(int) floor(id / 256)];
  tema = (rand() % 6) + 9;

  rotg(permutation[0], (int) (r & 192) >> 6);
  rotg(permutation[1], (int) (r & 48) >> 4);
  rotg(permutation[2], (int) (r & 12) >> 2);
  rotg(permutation[3], (int) r & 3);

  // I tried doing this in a loop and it didn't work
  move(cy - 12, cx - 24);
  printk("             _       _ \n", tema, non);
  move(cy - 11, cx - 24);
  printk(" _ _ _ _ ___|_|___ _| |\n", tema, non);
  move(cy - 10, cx - 24);
  printk("| ' | | |  _| |_  |   |\n", tema, non);
  move(cy - 9, cx - 24);
  printk("|_._|_  |_| |_|_'_|_'_|\n", tema, non);
  move(cy - 8, cx - 24);
  printk("    |___|", tema, non);

  move(cy - 10, cx + 24 - strlen(id_str));
  printw(id_str);

  // rest of the
  rtnbd();
  rtncr();
  rtncg();
  rtnhp();

  while (oui) {
    switch (getch()) {
      case 'z':
        cur--;
        if (cur == -1) {
          cur = 11;
        }
        rtncr();
        rtncg();
        break;
      case 'x':
        cur++;
        if (cur == 12) {
          cur = 0;
        }
        rtncr();
        rtncg();
        break;
      case 'q':
        rotg(pieces[cur], 3);
        rtncg();
        break;
      case 'e':
        rotg(pieces[cur], 1);
        rtncg();
        break;
      case 'w':
        unjv(pieces[cur], cur + 1);
        rtncg();
        break;
      case 'a':
        lnjv(pieces[cur], cur + 1);
        rtncg();
        break;
      case 's':
        dnjv(pieces[cur], cur + 1);
        rtncg();
        break;
      case 'd':
        rnjv(pieces[cur], cur + 1);
        rtncg();
        break;
      case 'j':
        hflg(pieces[cur]);
        rtncg();
        break;
      case 'k':
        vflg(pieces[cur]);
        rtncg();
        break;
      case ' ':
        if (pieces[cur]->placed == 1) {
          rtnpk();
        } else {
          rtnin();
        }
        break;
      case 'c' & 0x1f:
        endwin();
        return bon;
    }
  }
}
