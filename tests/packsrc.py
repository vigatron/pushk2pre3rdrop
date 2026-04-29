import glob
import os
import subprocess

import mods.glbdefs as glbdefs


from mods.archs import ARCHS_EXTS
from mods.archs import packexts, packcmds

from mods.cfgexamples import find_config, copy_multiple_files
from mods.cfgexamples import CFGDROP_S, CFGDROP_M
from mods.cfgexamples import clean_results_folder

from mods.logs import generate_logs

from mods.fnames import transformed_files_names, restored_files_names
from mods.fnames import step_prepare_results_folders


# 1) Sources Defined as EXAMPLES_FILES instead of Scan 'examples' folder
# 2) Move each example to dedicated folder in "results"


tool_drop_name = os.path.join(glbdefs.ROOT_DIR, "build/pushk2pre3rdrop")


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

# ----------------------------------------------------------------------------------------------------------
def process_rdrop_split( source_fnames , transformed_tables, transformed_fnames ):

    print( "### R-DROP Split ###" )

    for idx in range(len(source_fnames)):
        src = source_fnames[idx]
        tbl = transformed_tables[idx]
        dst = transformed_fnames[idx]

        cfg = find_config(CFGDROP_S, src)
        if None == cfg:
            return False

        arr_cmd = [ tool_drop_name, "s", src, tbl, dst, str(cfg[0]), str(cfg[1]), str(cfg[2]) ]
        print( "RDrop split " , src )
        try:
            # , stderr=subprocess.STDOUT , stdout=f
            subprocess.run(arr_cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL )
        except subprocess.CalledProcessError as e:
            print(f"Error while creating rdrop-table: {e}")
            return False
        except FileNotFoundError:
            print(tool_drop_name, " utility not found. Make sure it is installed and in PATH.")
            return False

    print()
    return True

# ----------------------------------------------------------------------------------------------------------
def process_rdrop_merge( transformed_fnames , transformed_tables, restored_fnames ):

    print( "### R-DROP Merge ###" )

    for idx in range(len(transformed_fnames)):
        fname_trf = transformed_fnames[idx]
        fname_tbl = transformed_tables[idx]
        fname_rcv = restored_fnames[idx]

        cfg = find_config(CFGDROP_M, fname_trf)
        if None == cfg:
            return False

        arr_cmd = [ tool_drop_name, "m",
                   fname_trf,
                   fname_tbl,
                   fname_rcv, str(cfg[0]), str(cfg[1]), str(cfg[2]) ]

        print( "RDrop merge " , fname_trf )
        try:
            # subprocess.run(arr_cmd, check=True )  # stdout=subprocess.STDOUT, stderr=subprocess.STDOUT
            subprocess.run(arr_cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL )
        except subprocess.CalledProcessError as e:
            print(f"Error while creating rdrop-table: {e}")
            return False
        except FileNotFoundError:
            print(tool_drop_name, " utility not found. Make sure it is installed and in PATH.")
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

    fnames_transformed = transformed_files_names( results_files )
    fnames_rdroptbls = [fname + ".rdrop.bin" for fname in fnames_transformed]
    fnames_restored = restored_files_names(fnames_transformed)

    # print( "\n###", source_fnames )
    # print( "\n###", fnames_rdroptbls )
    # print( "\n###", fnames_transformed )
    # print( "\n###", fnames_restored )

    # RDROP Transform
    result_rdrop_s = process_rdrop_split( results_files , fnames_rdroptbls, fnames_transformed )
    if not result_rdrop_s:
        print( "RDrop split ERROR !")
        return False

    # Restore from RDROP
    result_rdrop_m = process_rdrop_merge( fnames_transformed, fnames_rdroptbls, fnames_restored )
    if not result_rdrop_m:
        print( "RDrop merge ERROR !")
        return False

    # Pack original
    original_archs = pack_files(results_files)

    # Pack transformed
    transformed_archs = pack_files(fnames_transformed)

    # Generate Logs
    generate_logs(
        glbdefs.RESULTS_DIR,
        original_files,
        fnames_rdroptbls,
        fnames_transformed,
        fnames_restored,
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
