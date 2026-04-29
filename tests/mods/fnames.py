import os
import shutil

# ----------------------------------------------------------------------------------------------------------
def transformed_files_names( src_files : list[str]) -> list[str] :
    modified_files = []
    for f in src_files:
        name, ext = os.path.splitext(f)  # разделяем имя и расширение
        modified_files.append(name + "d" + ext)
    return modified_files

# ----------------------------------------------------------------------------------------------------------
def restored_files_names( src_files : list[str]) -> list[str] :
    modified_files = []
    for f in src_files:
        name, ext = os.path.splitext(f)  # разделяем имя и расширение
        modified_files.append(name + "r" + ext)
    return modified_files

# ----------------------------------------------------------------------------------------------------------
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
