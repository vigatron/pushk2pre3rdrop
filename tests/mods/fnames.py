import os

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
