#include <stdio.h>
#include "splitter.h"
#include "pipeline.h"
// splitter and pipeline are separate components!
int
main(int argc, const char *argv[])
{
    ++ argv;
    -- argc;
    set_array(argc, argv);
    set_delimiter("--");
    int begin, end;
    while (next_block(&begin, &end) == 0) {
        if (add_to_pipeline(&argv[begin], end - begin) != 0) {
            fprintf(stderr, "Can't add to pipeline; abort\n");
            free_pipeline();
            free_splitter();
            return 0;
        }
    }
    run_pipeline();
    free_pipeline();
    free_splitter();
    return 0;
}
