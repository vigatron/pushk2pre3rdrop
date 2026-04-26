import os
import glob
import shutil


CFGDROP = [
    [ "example7b.bin"   , [ 0,  256,   64 ] ],
    [ "example8.bin"    , [ 0,    8, 2048 ] ],
    [ "example9b.bin"   , [ 0,  256,   64 ] ],
    [ "example10b.bin"  , [ 0,  256,   64 ] ],
]

def getFilesInFolder( folder : str ):
    bin_files = glob.glob(os.path.join(folder, "*.bin"))
    return bin_files

def find_config( example_fname ):
    basefname = os.path.basename( example_fname )
    for name, config in CFGDROP:
        if name == basefname:
            return config
    return None

def clean_results_folder( folder ):
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


def copy_multiple_files( arr_src_file : list[str], arr_dst_files : list[str]):
    # Copy *.bin files to 'results/[dedicated]/*' folder
    for src, dst in zip(arr_src_file, arr_dst_files):
        shutil.copy2(src, dst)
