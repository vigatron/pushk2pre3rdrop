import os

LOCAL_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(LOCAL_DIR)

BUILD_DIR = os.path.join(ROOT_DIR, "build")
EXAMPLES_DIR = os.path.join(ROOT_DIR, "examples")
RESULTS_DIR = os.path.join(ROOT_DIR, "results")

SRC_FILES = ["example7b.bin", "example8.bin", "example9b.bin", "example10b.bin"]
EXAMPLES_FILES = [f"{EXAMPLES_DIR}/{fname}" for fname in SRC_FILES]


# arj a -m1 "$1".arj "$1"
# 7z -mx9 a "$1".7z "$1"
# rar a -m5 "$1".rar "$1"
# zip -9 "$1".zip "$1"

# bzip2 -k -9 "$1"
# gzip -k -9 "$1"
# xz -k -9 "$1"
# lzma -k -9 "$1"

