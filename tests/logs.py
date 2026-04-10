from archs import get_arr_info , get_arr_filtered_version
from archs import ARCHS_APPS

import os
import hashlib



def calcmd5(fname_full):
    with open( fname_full, "rb") as f:
        file_hash = hashlib.md5(f.read()).hexdigest()
        return file_hash


def generatelog_archs():
    arr_info = get_arr_info()
    arr_vers = get_arr_filtered_version(arr_info)

    print("|  Archiver  |  Version  |  Info  |")
    print("|--------|-----------|--------|")
    for a, b, c in zip(ARCHS_APPS, arr_vers, arr_info ):
        print(f"|  {a}  |  {b}  |  {c}  |")


def print_fileprops(fname_full):
    fname = os.path.basename( fname_full )
    size = os.path.getsize( fname_full )
    md5 = calcmd5( fname_full )
    print( f"| {fname } | {size} | {md5} |" )


def generatelog_src( original_files ):
    # Original file props
    print("| File Name  | Size   | MD5    |")
    print("|------------|--------|--------|")
    for src_file in original_files:
        print_fileprops( src_file )


def gen_archshdrs():
    r = ""
    for item in ARCHS_APPS:
        r += item + " |"
    return r

# --------|--------| ...
def gen_archsseps():
    r = ""
    for item in ARCHS_APPS:
        r += "---|"
    return r



def generatelog_src_pck( archs ):
    print("| File Name  | bin    | " + gen_archshdrs() )
    print("|------------|--------|"  + gen_archsseps() )
    for item in archs:
        origfile = os.path.basename( item[0][0] )
        txt = "| " + origfile + " | " + str(os.path.getsize( item[0][0] )) + " | "
        for filepck in item[1]:
            txt += str(os.path.getsize( filepck ))
            txt += " |"
        print( txt )


def generate_logs( original_files, transformed_fnames, transformed_tables, original_archs, transformed_archs ):

    print()
    generatelog_archs()

    # Original files
    print()
    generatelog_src( original_files )

    print()
    generatelog_src_pck ( original_archs )

    # RDROP Tables
    print()
    generatelog_src( transformed_tables )

    # Transformed files
    print()
    generatelog_src( transformed_fnames )

    print()
    generatelog_src_pck ( transformed_archs )

    # src_files = [os.path.basename(f) for f in src_files_fullpath]

    # # RDrop processing
    # print()
    # print("| Original File Name  | Result RDrop Table  | Result File Name  |")
    # print("|---------------------|---------------------|-------------------|")

    # # dropped.bin + rdrop 
    # print()
    # print("| File Name  | + RDrop Tbl  | bin    | ")
    # print("|------------|--------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|")

