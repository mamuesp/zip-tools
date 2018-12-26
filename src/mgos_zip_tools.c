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

#include "mgos_zip_tools.h"

bool mgos_init_archive(const char *archFile, mz_zip_archive *zipArch) {
	mz_bool status;
	// init the structure
	mz_zip_zero_struct(zipArch);
	status = mz_zip_reader_init_file(zipArch, archFile, 0);
	if (!status) {
		LOG(LL_ERROR, ("ZIP file <%s> appears invalid/not loadable!", archFile));
		return false;
	}
	return true;
}

bool mgos_exit_archive(mz_zip_archive *zipArch) {
  LOG(LL_DEBUG, ("ZIP reader end."));
	mz_zip_reader_end(zipArch);
  return true;
}

uint16_t mgos_get_zipped_buffer_size(char *archFile, char *extFile) {

  mz_zip_archive zipArch;
	mz_zip_archive_file_stat zipFileStat;
	mz_uint32 zipIdx;
	mz_bool zipFile;
	uint16_t result;
	char *comment = NULL;
	long archSize = 0;

	result = 0;

	if (!mgos_init_archive(archFile, &zipArch)) {
		return result;
	}

	LOG(LL_DEBUG, ("Check ZIP file <%s> ...", extFile));
	archSize = zipArch.m_archive_size;
	if (archSize > 0) {
		LOG(LL_DEBUG, ("ZIP file checked, archive size <%ld> ...", (long) archSize));
		zipFile = mz_zip_reader_locate_file_v2(&zipArch, extFile, comment, MZ_ZIP_FLAG_IGNORE_PATH, &zipIdx);
		if (zipFile == MZ_TRUE) {
			LOG(LL_DEBUG, ("ZIP file found, index is <%d> ...", zipIdx));
			if (mz_zip_reader_file_stat(&zipArch, zipIdx, &zipFileStat)) {
				result = (uint16_t) zipFileStat.m_uncomp_size;
			}
		}
	}

  LOG(LL_DEBUG, ("ZIP reader end."));
	mz_zip_reader_end(&zipArch);

	return result;
}

void *mgos_get_zipped_data(const char *archFile, const char *extFile, size_t *unCompSize, const char *buffer, size_t bufLen) {

	mz_zip_archive zipArch;
	mz_zip_archive_file_stat zipFileStat;
	mz_uint32 zipIdx;
	mz_bool zipFile;
	char *result;
	char *comment = NULL;
	size_t zipSize = 0;
	long archSize = 0;

	result = NULL;
	if (!mgos_init_archive(archFile, &zipArch)) {
		return result;
	}

	LOG(LL_DEBUG, ("Check ZIP file <%s> ...", extFile));
	archSize = zipArch.m_archive_size;
	if (archSize <= 0) {
		return result;
	}

	LOG(LL_DEBUG, ("ZIP file checked, archive size <%ld> ...", (long) archSize));
	zipFile = mz_zip_reader_locate_file_v2(&zipArch, extFile, comment, MZ_ZIP_FLAG_IGNORE_PATH, &zipIdx);
	if (zipFile == MZ_FALSE) {
    LOG(LL_DEBUG, ("Could not locate <%s> in ZIP file <%s> ...", extFile, archFile));
		return result;
	}

	LOG(LL_DEBUG, ("ZIP file found, index is <%d> ...", zipIdx));
	if (mz_zip_reader_file_stat(&zipArch, zipIdx, &zipFileStat)) {
		zipSize = zipFileStat.m_uncomp_size;
		LOG(LL_DEBUG, ("ZIP file <%s> checked, unzipped size <%ld> ...", extFile, (long) zipSize));
    if (buffer != NULL) {
      if (mz_zip_reader_extract_to_mem(&zipArch, zipIdx, (char *) buffer, bufLen, 0) == MZ_TRUE) {
        result = (char *) buffer;  
      }
    } else {
      result = mz_zip_reader_extract_to_heap(&zipArch, zipIdx, &zipSize, 0);
    }
    *unCompSize = result ? zipSize : 0;
		if (result) {
			LOG(LL_DEBUG, ("ZIP file <%s> uncompressed (%ld bytes)", extFile, (long) zipSize));
		}
	}

	mgos_exit_archive(&zipArch);
	return (void *) result;
}

bool mgos_free_zipped_data(void *buffer) {
	mz_free(buffer);
	return true;
}

bool mgos_zip_tools_init(void) {

  if (!mgos_sys_config_get_zip_tools_enable()) {
    return true;
  }
    
  return true;
}

