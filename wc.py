#!/usr/bin/python
import curses
import time
import sys
import os
from stat import *

def my_func(stdscr):
    curses.curs_set(0);

    while True:
        num_bytes = os.stat(sys.argv[1]).st_size
        #num_lines = sum(1 for line in open(sys.argv[1]))
        stdscr.addstr(3, 0, "Bytes in file: {0}".format(num_bytes))
        stdscr.refresh()
        time.sleep(0.1)

curses.wrapper(my_func)


