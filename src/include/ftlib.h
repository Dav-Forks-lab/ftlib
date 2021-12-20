#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "../ftl/ftl.h"
#include "json.hpp"

typedef struct _datafile DFILE;
using json = nlohmann::json;
DFILE* readfile(char* filename, char* fname);

#endif 