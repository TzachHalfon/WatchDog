#!/bin/bash

gcc -ansi -pedantic-errors -Wall -Wextra -g -I ./include -I include ./source/dlist.c ./source/priorityq.c ./source/scheduler.c ./source/sortedlist.c ./source/task.c ./source/UID.c ./source/wd.c ./source/watchdog_main.c -lpthread -o watchdog.out
gcc -ansi -pedantic-errors -Wall -Wextra -g -I ./include -I include ./source/dlist.c ./source/priorityq.c ./source/scheduler.c ./source/sortedlist.c ./source/task.c ./source/UID.c ./source/wd.c ./test/watchdog_test.c -lpthread -o user.out