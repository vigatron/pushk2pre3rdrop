import os
import hashlib

from mods.archs import get_arr_info , get_arr_filtered_version
from mods.archs import ARCHS_APPS



def calcmd5(fname_full):
    with open( fname_full, "rb") as f:
        file_hash = hashlib.md5(f.read()).hexdigest()
        return file_hash


def generatelog_archs():
    ret = []
    arr_info = get_arr_info()
    arr_vers = get_arr_filtered_version(arr_info)

    ret.append("|  Archiver  |  Version  |  Info  |")
    ret.append("|--------|-----------|--------|")
    for a, b, c in zip(ARCHS_APPS, arr_vers, arr_info ):
        ret.append(f"|  {a}  |  {b}  |  {c}  |")

    return ret

def tblrow_fileprops(fname_full) -> str:
    fname = os.path.basename( fname_full )
    size = os.path.getsize( fname_full )
    md5 = calcmd5( fname_full )
    ret = f"| {fname } | {size} | {md5} |"
    return ret

# Original files props
def generatelog_src( original_files ):
    ret = []
    # Header
    ret.append("| File Name  | Size   | MD5    |")
    ret.append("|------------|--------|--------|")
    for src_file in original_files:
        txtrow = tblrow_fileprops(src_file)
        ret.append( txtrow )
    return ret

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


def arr_to_console( arrlines : list[str]):
    print( "\n", "\n".join(arrlines))

def arr_to_file(lines : list[str], fname : str):
    with open(fname, "w", encoding="utf-8") as file:
        file.write("\n".join(lines))

def generate_logs(
        folder_results : str,
        original_files,
        transformed_fnames,
        transformed_tables,
        original_archs,
        transformed_archs ):

    # Arch info
    arrtxt_archs = generatelog_archs()
    # arr_to_console(arrtxt_archs)
    file_log_archs = os.path.join(folder_results, "arch_ver.txt") 
    arr_to_file(arrtxt_archs, file_log_archs)

    # Original files
    marktbl = generatelog_src( original_files )
    file_log_srcfiles = os.path.join(folder_results, "srcfiles.txt") 
    arr_to_file(marktbl, file_log_srcfiles)

    # Benchmark results table
    print()
    generatelog_src_pck ( original_archs )

    # RDROP Tables
    marktbl = generatelog_src( transformed_tables )
    file_log_rtblfiles = os.path.join(folder_results, "rtblfiles.txt") 
    arr_to_file(marktbl, file_log_rtblfiles)

    # Transformed files
    marktbl = generatelog_src( transformed_fnames )
    file_log_transfiles = os.path.join(folder_results, "transfiles.txt") 
    arr_to_file(marktbl, file_log_transfiles)

    print()
    generatelog_src_pck ( transformed_archs )

