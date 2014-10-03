/* altk-xml2c.c -
 */

#include <clog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>



/* write_c_file:
 */
static void write_c_file ( FILE *fin,
                           FILE *fout,
                           const char *name )
{
  int c;
  fprintf(fout, "const char %s[] =\n", name);
  fprintf(fout, "  \"");
  errno = 0;
  while ((c = fgetc(fin)) >= 0)
    {
      if (c == '\n')
        fprintf(fout, "\\n\"\n  \"");
      else if (c >= 32 && c <= 127)
        fprintf(fout, "%c", c);
      else
        fprintf(fout, "\\x%02x", c);
    }
  if (errno != 0)
    CL_ERROR("error: %s", strerror(errno));
  fprintf(fout, "\";\n");
}



/* main:
 */
int main ( int argc,
           char **argv )
{
  char *input=NULL, *out_c=NULL, *name=NULL;
  char c;
  FILE *fin, *fout;
  /* CL_DEBUG("hello!"); */
  /* parse command line */
  opterr = 0;
  while ((c = getopt(argc, argv, "o:n:")) != -1)
    {
      switch (c)
        {
        case 'o':
          out_c = malloc(strlen(optarg)+3);
          sprintf(out_c, "%s.c", optarg);
          break;
        case 'n':
          name = optarg;
          break;
        case '?':
          CL_ERROR("unknown option: `%c'", optopt);
        default:
          CL_ERROR("oups: `%c'", c);
        }
    }
  /* args */
  if (optind == argc)
    CL_ERROR("input file required");
  else if (optind > (argc+1))
    CL_ERROR("too many args");
  input = argv[optind];
  /* check */
  if (!out_c)
    CL_ERROR("output required");
  if (!name)
    CL_ERROR("name required");
  /* go */
  /* CL_DEBUG("input: '%s'", input); */
  /* CL_DEBUG("out_c: '%s'", out_c); */
  /* CL_DEBUG("name:  '%s'", name); */
  if (!(fin = fopen(input, "r")))
    CL_ERROR("could not open '%s'", input);
  if (!(fout = fopen(out_c, "w")))
    CL_ERROR("could not open '%s'", out_c);
  write_c_file(fin, fout, name);
  fclose(fin);
  fclose(fout);
  /* CL_DEBUG("all done!"); */
  return 0;
}
