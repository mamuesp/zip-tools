/**
 *
 * Copyright (c) 2018 Manfred Mueller-Spaeth <fms1961@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **/
#ifndef __MGOS_ZIP_TOOLS_H
#define __MGOS_ZIP_TOOLS_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "mgos.h"
#include "mgos_app.h"
#include "miniz.h"
#include "mgos_config.h"
#include "mgos_ro_vars.h"
#include "mgos_rpc.h"

bool mgos_init_archive(const char *archFile, mz_zip_archive *zipArch);

bool mgos_exit_archive(mz_zip_archive *zipArch);

uint16_t mgos_get_zipped_buffer_size(char *archFile, char *extFile);

void *mgos_get_zipped_data(const char *archFile, const char *extFile, size_t *unCompSize, const char *buffer, size_t bufLen);

bool mgos_free_zipped_data(void *buffer);

#endif // __MGOS_ZIP_TOOLS_H
