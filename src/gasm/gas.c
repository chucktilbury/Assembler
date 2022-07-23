
#include "gas.h"
#include "cmdline.h"


/*
 * Find all of the format markers in the string with the format {name} and
 * replace them with the index number of the variable. If the name does not
 * name a variable, then leave the sequence in the string unchanged.
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
                    Index idx = findValTabIdx(tmp->buf);
                    if(idx != 0)
                        addStrFmt(sptr, "{%d}", idx);
                    else
                        addStrFmt(sptr, "{%s}", tmp->buf);
                    sidx--;
                    state = 0;
                }
                break;

            case 3: // abort marker and end
                addStrChar(sptr, '{');
                addStrStr(sptr, tmp->buf);
                state = 255;
                break;

            case 4: // abort marker and continue
                addStrChar(sptr, '{');
                addStrStr(sptr, tmp->buf);
                state = 0;
                break;

            case 255:
                // terminate the loop
                finished = true;
                break;
        }

        sidx++;
    }

    return sptr->buf;
}

static void save_binary(const char* fname)
{
    FILE* fp = fopen(fname, "w");
    if(fp == NULL)
        fatalError("cannot open output file: %s: %s", fname, strerror(errno));

    // output the file header.
    uint64_t magic = MAGIC_NUMBER;
    fwrite(&magic, sizeof(magic), 1, fp);

    char fbuf[FNAME_LEN];
    memset(fbuf, 0, sizeof(fbuf));
    strncpy(fbuf, fname, FNAME_LEN-1);
    fwrite(fbuf, sizeof(fbuf), 1, fp);

    saveInstStream(fp);
    saveValTab(fp);
    //saveStrTab(fp);
}


Module* module;

int main(int argc, char** argv)
{
    _init_memory();
    initCmdLine(CL_FL_ONE, "This is the assembler. It reads the assembly language input\n"
                            "and converts it to a binary for use by the virtual machine.\n");
    addStrParam("-o", "ofile", "output file name", "output.bin", CL_NONE);
    addTogParam("-e", "entry", "disable emitting the entry and exit points in the output", true, CL_NONE);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "show the help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    //initVM();

    if(isatty(fileno(stdin))) {
        resetCLFileList();
        const char* name = iterateCLFileList();
        if(name != NULL && strlen(name) > 0)
            open_file(name);
        else
            showUseCmdLine();
    }
    // else flex will open the pipe

    // post processing data structures
    initInstStream();
    initValTab();
    initStrTab();

    module = createModule();
    if(getTogParam("entry")) {
        addClass8(module, OP_CALL, "_start");
        addClass0(module, OP_NOP);
        addClass8(module, OP_JMP, "_ending");
    }
    yyparse();
    if(getTogParam("entry")) {
        addLabel(module, "_ending");
        addClass0(module, OP_NOP);
        addClass0(module, OP_EXIT);
    }
    doPostProcess(module);

    if(!getErrors())
        save_binary(getStrParam("ofile"));

    return getErrors();
}



