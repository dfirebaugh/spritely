#include "globals.h"
#ifndef __EMSCRIPTEN__

void run_spit_file(char *filename)
{
	FILE* fp;

	Py_Initialize();

	fp = _Py_fopen(filename, "rb");
	PyRun_SimpleFile(fp, filename);

	Py_Finalize();
}

#endif