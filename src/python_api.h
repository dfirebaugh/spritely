#ifndef _PYTHON_API
#define _PYTHON_API

#ifndef __EMSCRIPTEN__

#define PY_SSIZE_T_CLEAN
#include <Python.h>



extern void run_spit_file(char *filename);
#endif

#endif