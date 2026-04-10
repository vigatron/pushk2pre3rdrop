import os
import glob


CFGDROP = [
    [ "example7b.bin"   , [ 0,  256,   64 ] ],
    [ "example8.bin"    , [ 0,    8, 2048 ] ],
    [ "example9b.bin"   , [ 0,  256,   64 ] ],
    [ "example10b.bin"  , [ 0,  256,   64 ] ],
]

def getFilesInFolder( folder : str ):
    bin_files = glob.glob(os.path.join(folder, "*.bin"))
    return bin_files

def find_config( fname ):
    basefname = os.path.basename( fname )
    for name, config in CFGDROP:
        if name == basefname:
            return config
    return None