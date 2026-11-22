#include "vhplatform.hpp"

#include "app_version.hpp"

#ifdef APP_NAME_STR
const std::string APP_NAME = APP_NAME_STR;
#else
const std::string APP_NAME = "pushk2pre3rdrop";
#endif

#ifndef APP_VERSION
#define APP_VERSION 0
#endif

#include "argsparser.hpp"
#include "misc/errors.hpp"
#include "misc/fileio.hpp"
#include "rdrop/rdroplayers.hpp"

int func_split(int argc, char *argv[]);
int func_merge(int argc, char *argv[]);

/**
 * 
 */
void ShowVersion()
{
    std::cout << "AppName: " << APP_NAME << " ver " << APP_VERSION << std::endl;
}

/**
 *
 */
void PrintUsage()
{

    const std::string str = R"(
pushk2pre3rdrop <cmd> <file_name_input> <file_name_rdroptbl> <file_name_output> <start_offset> <count> <block_size>

| Parameter              | Description                                              |
|------------------------|----------------------------------------------------------|
| `cmd`                  | Operation mode: `s` — split, `m` — merge, `v` — version  |
| `file_name_input`      | Path to input file                                       |
| `file_name_rdroptbl`   | Path to RDROP table file                                 |
| `file_name_output`     | Path to output file                                      |
| `start_offset`         | Start offset in bytes                                    |
| `count`                | Number of blocks to process (maximum: 256)               |
| `block_size`           | Size of each block in bytes                              |)";

    std::cout << str << std::endl;
    std::cout << std::endl;
}

/**
 *
 */
verr single_param(const std::string &param)
{
    std::vector<std::string> arrver = { "v", "-v", "--version" };
    bool foundver = std::find(arrver.begin(), arrver.end(), param) != arrver.end();

    if(foundver)
    {
        ShowVersion();
        return vok;
    }

    std::vector<std::string> arrhlp = { "-h", "--help" };
    bool foundhlp = std::find(arrhlp.begin(), arrhlp.end(), param) != arrhlp.end();

    if (foundhlp)
    {
        ShowVersion();
        PrintUsage();
        return vok;
    }

    std::cout << "Invalid command: " << param << std::endl;

    return verror(1);
}

/**
 *
 */
int main(int argc, char *argv[])
{
    // No params
    if (argc < 2)
    {
        ShowVersion();
        PrintUsage();
        return eOk;
    }

    // Single param
    std::string param = argv[1];

    if (argc <= 2)
    {
        return single_param(param);
    }

    verr rret;

    if (param == "s")
    {
        rret = func_split(argc, argv);
    }
    else if (param == "m")
    {
        rret = func_merge(argc, argv);
    }
    else
    {
        rret = eError_InvalidCommand;
        std::cout << "Invalid command : " << param << std::endl;
        return 1;
    }

    // Error ?
    if (rret != vok)
    {
        std::cout << "Error code: " << std::to_string(rret) << std::endl;
    }

    return rret;
}