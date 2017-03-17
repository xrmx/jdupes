/* Print matched file sets
 * This file is part of jdupes; see jdupes.c for license information */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "jdupes.h"
#include "jody_win_unicode.h"
#include "act_printmatches.h"

extern void printmatches(file_t * restrict files)
{
  file_t * restrict tmpfile;
  int printed = 0;

  while (files != NULL) {
    if (ISFLAG(files->flags, F_HAS_DUPES)) {
      printed = 1;
      if (!ISFLAG(flags, F_OMITFIRST)) {
        if (ISFLAG(flags, F_SHOWSIZE)) printf("%" PRIdMAX " byte%c each:\n", (intmax_t)files->size,
         (files->size != 1) ? 's' : ' ');
        fwprint(stdout, files->d_name, 1);
      }
      tmpfile = files->duplicates;
      while (tmpfile != NULL) {
        fwprint(stdout, tmpfile->d_name, 1);
        tmpfile = tmpfile->duplicates;
      }
      if (files->next != NULL) fwprint(stdout, "", 1);

    }

    files = files->next;
  }

  if (printed == 0) fwprint(stderr, "No duplicates found.", 1);

  return;
}

extern void jsonoutput(file_t * restrict files)
{
  file_t * restrict tmpfile;

  fwprint(stdout, "[", 1);
  while (files != NULL) {
    if (ISFLAG(files->flags, F_HAS_DUPES)) {
      fwprint(stdout, "[", 0);
      if (!ISFLAG(flags, F_OMITFIRST)) {
        fwprint(stdout, "\"", 0);
        fwprint(stdout, files->d_name, 0);
        fwprint(stdout, "\"", 0);
      }
      tmpfile = files->duplicates;
      while (tmpfile != NULL) {
        fwprint(stdout, ",\"", 0);
        fwprint(stdout, tmpfile->d_name, 0);
        fwprint(stdout, "\"", 0);
        tmpfile = tmpfile->duplicates;
      }
      if (files->next != NULL) fwprint(stdout, "]", 1);
    }

    files = files->next;
  }
  fwprint(stdout, "]", 1);

  return;
}
