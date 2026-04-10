import glob
import os
import glbdefs
import subprocess
import shutil

from archs import ARCHS_APPS , ARCHS_EXTS
from archs import packexts, packcmds

from cfgexamples import CFGDROP, getFilesInFolder, find_config

from logs import generate_logs


# Scan examples folder

# Move each example to dedicated folder in "results"






def cleanfolder( folder ):
    for filename in os.listdir(folder):
        file_path = os.path.join(folder, filename)
        if os.path.isfile(file_path):
            os.remove(file_path)
        else:
            shutil.rmtree(file_path)


def step_prepare_results_folders( arr_original_files , dst_dir ):

    # Prepare results folder
    shutil.rmtree('results')
    os.mkdir('results')

    # Create subfolders
    r = []
    for file in arr_original_files:
        basefile = os.path.basename(file)
        dst_folder = os.path.join(dst_dir, basefile )
        os.mkdir(dst_folder)
        dst_file = os.path.join(dst_folder,basefile)
        r.append(dst_file)

    return r


def copy_examples( arr_src_file : list[str], arr_dst_files : list[str]):
    # Copy *.bin files to 'results/[dedicated]/*' folder
    for src, dst in zip(arr_src_file, arr_dst_files):
        shutil.copy2(src, dst)


def pack_files( src_files : list[str]):
    r = []
    # pack each file
    for fname_bin in src_files:
        print( "Processing : ", fname_bin )
        arrexts = packexts( os.path.basename(fname_bin) )
        arrcmds = packcmds( os.path.basename(fname_bin) , arrexts )

        for arrcmd in arrcmds:
            print( arrcmd )
            rundir = os.path.dirname(fname_bin)
            try:
                subprocess.run(arrcmd, check=True, cwd=rundir)
                print(f"Archive created successfully.")
            except subprocess.CalledProcessError as e:
                print(f"Error while creating archive: {e}")
            except FileNotFoundError:
                print( arrcmd )
                print("utility not found. Make sure it is installed and in PATH.")
        

        r.append( [ [fname_bin] , [ f"{fname_bin}.{ext}" for ext in ARCHS_EXTS] ] )

        print("* Done *")

    return r



# ----------------------------------------------------------------------------------------------------------

def transformed_files_names( src_files : list[str]) -> list[str] :
    modified_files = []
    for f in src_files:
        name, ext = os.path.splitext(f)  # разделяем имя и расширение
        modified_files.append(name + "d" + ext)
    return modified_files


# Process RDROP
def process_rdrop( source_fnames , transformed_fnames ):
    
    transformed_tables = [fname + ".rdrop.bin" for fname in transformed_fnames]
    print( "###", source_fnames )
    print( "###", transformed_fnames )
    print( "###", transformed_tables )

    tool_drop_name = "pushk2pre3rdrop"

    for src, tbl , dst  in zip(source_fnames, transformed_tables, transformed_fnames):
        cfg = find_config( src )
        if None == cfg:
            return None
        # print( cfg )
        arr_cmd = [ tool_drop_name, "s", src, tbl, dst, str(cfg[0]), str(cfg[1]), str(cfg[2]) ]
        print( "Calling : " , arr_cmd )
        try:
            subprocess.run(arr_cmd, check=True )  # cwd=rundir
            print(f"Archive created successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Error while creating archive: {e}")
            return None
        except FileNotFoundError:
            print("utility not found. Make sure it is installed and in PATH.")
            return None

    return transformed_tables



def process(src_dir, dst_dir):

    # Scan original .bin files
    original_files = glob.glob(os.path.join(src_dir, "*.bin"))
    results_files = step_prepare_results_folders( original_files , dst_dir)
    copy_examples(original_files,results_files)

    # Pack original
    original_archs = pack_files(results_files)

    # Generate DROP Transform
    transformed_fnames = transformed_files_names( results_files )
    transformed_tables = process_rdrop( results_files , transformed_fnames )
    if not transformed_tables:
        print( "ERROR !")
        return False

    # Pack transformed
    transformed_archs = pack_files(transformed_fnames)

    # Generate Logs
    generate_logs( original_files, transformed_fnames, transformed_tables , original_archs, transformed_archs )
    
    return True

    # Restored


if __name__ == "__main__":

    # Clear results
    # shutil.rmtree(glbdefs.RESULTS_DIR)
    cleanfolder(glbdefs.RESULTS_DIR)
    process(glbdefs.EXAMPLES_DIR , glbdefs.RESULTS_DIR )
