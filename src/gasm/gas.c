
#include "gas.h"
#include "cmdline.h"

extern void dumpSymtab();

#if 0
/*
 * Find all of the format markers in the string with the format {name} and
 * replace them with the index number of the variable. If the name does not
 * name a varaible, then leave the sequence in the string unchanged.
 */
const char* preformat_str(const char* str)
{
    String* sptr = createStr(NULL);
    String* tmp;
    int sidx = 0;
    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0: // initial state: copy characters.
                switch(str[sidx]) {
                    case '\0':
                        // string ends, terminate normally
                        state = 255;
                        break;
                    case '{':
                        // potential format marker
                        tmp = createStr(NULL);
                        state = 1;
                        break;
                    default:
                        addStrChar(sptr, str[sidx]);
                        break;
                }
                break;

            case 1: // have a potential marker
                switch(str[sidx]) {
                    case '\0':
                        // string ends, abort the marker and end
                        state = 3;
                        break;
                    case '{':
                        // cannot be a marker. abort it and continue
                        addStrChar(tmp, '{');
                        state = 4;
                        break;
                    case '}':
                        // marker end found, try to resolve it
                        state = 2;
                        break;
                    default:
                        // add character to the tmp buffer
                        addStrChar(tmp, str[sidx]);
                        break;
                }
                break;

            case 2: { // try to resolve a found marker symbol
                    Index idx = symToIdx(tmp->list);
                    if(idx != 0)
                        addStrFmt(sptr, "{%d}", idx);
                    else
                        addStrFmt(sptr, "{%s}", tmp->list);
                    sidx--;
                    state = 0;
                }
                break;

            case 3: // abort marker and end
                addStrChar(sptr, '{');
                addStrStr(sptr, tmp->list);
                state = 255;
                break;

            case 4: // abort marker and continue
                addStrChar(sptr, '{');
                addStrStr(sptr, tmp->list);
                state = 0;
                break;

            case 255:
                // terminate the loop
                finished = true;
                break;
        }

        sidx++;
    }

    return sptr->list;
}
#endif

cmd_line cl;

int main(int argc, char** argv)
{
    _init_memory();
    cl = create_cmd_line("This is the assembler");
    add_str_param(cl, "ifile", "-i", "input file name", "", CF_NONE);
    add_str_param(cl, "ofile", "-o", "output file name", "output.bin", CF_NONE);
    add_num_param(cl, "verbose", "-v", "verbosity number from 0 to 10", 0, CF_NONE);
    parse_cmd_line(cl, argc, argv);

    //initVM();

    if(isatty(fileno(stdin))) {
        const char* name = get_str_param(cl, "ifile");
        if(strlen(name) > 0)
            open_file(name);
        else
            cmd_use(cl);
    }
    // else flex will open the pipe

    yyparse();

#if 0

    if(!getErrors()) {
        if(get_num_param(cl, "verbose") >= 1)
            showListing(stdout);

        if(get_num_param(cl, "verbose") >= 5) {
            //dumpRegs(vm, stdout);
            dumpSymtab(stdout);
            dumpVars(stdout);
            dumpStrs(stdout);
        }

        saveVM(get_str_param(cl, "ofile"));
    }
    _uninit_memory();
#endif
    return 0;
}
