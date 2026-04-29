import glob
import os
import subprocess
import shutil

import mods.glbdefs as glbdefs

from mods.archs import ARCHS_APPS , ARCHS_EXTS
from mods.archs import packexts, packcmds

from mods.cfgexamples import find_config, step_prepare_results_folders, copy_multiple_files
from mods.cfgexamples import CFGDROP, getFilesInFolder
from mods.cfgexamples import clean_results_folder

from mods.logs import generate_logs

from mods.fnames import transformed_files_names, restored_files_names

# 1) Sources Defined as EXAMPLES_FILES instead of Scan 'examples' folder
# 2) Move each example to dedicated folder in "results"



# ----------------------------------------------------------------------------------------------------------

def pack_files( src_files : list[str]):
    r = []
    # pack each file
    for fname_bin in src_files:
        print( "Multipack : ", fname_bin )
        arrexts = packexts( os.path.basename(fname_bin) )
        arrcmds = packcmds( os.path.basename(fname_bin) , arrexts )

        for arrcmd in arrcmds:
            # print( arrcmd )
            rundir = os.path.dirname(fname_bin)
            try:
                subprocess.run(arrcmd, check=True, cwd=rundir, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            except subprocess.CalledProcessError as e:
                print(f"Error while creating archive: {e}")
            except FileNotFoundError:
                print( arrcmd )
                print("utility not found. Make sure it is installed and in PATH.")

        r.append( [ [fname_bin] , [ f"{fname_bin}.{ext}" for ext in ARCHS_EXTS] ] )
        # print("* Done *\n")

    return r

# ----------------------------------------------------------------------------------------------------------
# Process RDROP
# ----------------------------------------------------------------------------------------------------------

tool_drop_name = "pushk2pre3rdrop"


def process_rdrop( source_fnames , transformed_tables, transformed_fnames ):

    print()
    print( "### PROCESS R-DROP ###" )

    # print( "\n###", source_fnames )
    # print( "\n###", transformed_fnames )
    # print( "\n###", transformed_tables )

    for idx in range(len(source_fnames)):
        src = source_fnames[idx]
        tbl = transformed_tables[idx]
        dst = transformed_fnames[idx]
        
        cfg = find_config( src )
        if None == cfg:
            return False

        arr_cmd = [ tool_drop_name, "s", src, tbl, dst, str(cfg[0]), str(cfg[1]), str(cfg[2]) ]
        print( "RDrop for " , src )
        try:
            # , stderr=subprocess.STDOUT , stdout=f
            subprocess.run(arr_cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL )
        except subprocess.CalledProcessError as e:
            print(f"Error while creating rdrop-table: {e}")
            return False
        except FileNotFoundError:
            print("utility not found. Make sure it is installed and in PATH.")
            return False

    print()
    return True

# ----------------------------------------------------------------------------------------------------------
def process(src_dir, dst_dir):

    # Clear results
    clean_results_folder(dst_dir)

    # Scan original .bin files
    original_files = glob.glob(os.path.join(src_dir, "*.bin"))
    results_files = step_prepare_results_folders( original_files , dst_dir)
    copy_multiple_files(original_files,results_files)

    # Generate RDROP Transform
    transformed_fnames = transformed_files_names( results_files )
    transformed_tables = [fname + ".rdrop.bin" for fname in transformed_fnames]
    result_rdrop = process_rdrop( results_files , transformed_tables, transformed_fnames )
    if not result_rdrop:
        print( "ERROR !")
        return False

    # Restore from RDROP
    fnames_restored = restored_files_names(transformed_fnames)
    print("restored", fnames_restored)

    # Pack original
    original_archs = pack_files(results_files)

    # Pack transformed
    transformed_archs = pack_files(transformed_fnames)

    # Generate Logs
    generate_logs(
        glbdefs.RESULTS_DIR,
        original_files,
        transformed_fnames,
        transformed_tables,
        original_archs,
        transformed_archs )
    
    return True

    # Restored

# ----------------------------------------------------------------------------------------------------------
def printvars():
    print( "LOCAL_DIR   : ", glbdefs.LOCAL_DIR )
    print( "ROOT_DIR    : ", glbdefs.ROOT_DIR  )
    print( "BUILD_DIR   : ", glbdefs.BUILD_DIR )
    print( "EXMPLS_DIR  : ", glbdefs.EXMPLS_DIR )
    print( "RESULTS_DIR : ", glbdefs.RESULTS_DIR )
    print()


# ----------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    printvars()
    process(glbdefs.EXMPLS_DIR , glbdefs.RESULTS_DIR )
