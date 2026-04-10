import subprocess
import re


ARCHS_EXTS = [
    "zip", "rar", "lzma", "7z", "xz",
    "zst", "br", "bz2", "gz", "arj" ]

ARCHS_APPS = [
    "zip", "rar", "lzma", "7z", "xz",
    "zstd", "brotli", "bzip2", "gzip", "arj"
]

ARCH_CMDS = {
    "zip":   ["zip", "--version"],
    "rar":   ["rar"],
    "lzma":  ["lzma", "--version"],
    "7z":    ["7z", "i"],
    "xz":    ["xz", "--version"],
    "zst":   ["zstd", "--version"],
    "br":    ["brotli", "--version"],
    "bz2":   ["bzip2", "--version"],
    "gz":    ["gzip", "--version"],
    "arj":   ["arj"]
}

def packexts(fname : "str") -> list:
    exts = [f".{ext}" for ext in ARCHS_EXTS]
    return exts


def packcmds(fname : str, packnames : list[str]) -> list:
    cmd_zip    = [ "zip"        , "-9"          , f"{fname}{packnames[0]}", fname]
    cmd_rar    = [ "rar"        , "-m5"         , "a"   , f"{fname}{packnames[1]}", fname]
    cmd_lzma   = [ "lzma"       , "-9"          , "-k"  , fname ]
    cmd_7z     = [ "7z"         , "-mx9"        , "a"   , f"{fname}{packnames[3]}", fname]
    cmd_xz     = [ "xz"         , "-9"          , "-k"  , fname ]
    cmd_zst    = [ "zstd"       , "--ultra"     , "-22" , fname ]
    cmd_br     = [ "brotli"     , "-q"          , "11"  ,  fname ]
    cmd_bzip2  = [ "bzip2"      , "-9"          , "-k"  , fname ]
    cmd_gzip   = [ "gzip"       , "-9"          , "-k"  , fname ]
    cmd_arj    = [ "arj"        , "-jm"         , "a"   , f"{fname}{packnames[9]}", fname]  # "-m1"

    arr = [
        cmd_zip,
        cmd_rar,
        cmd_lzma,
        cmd_7z,
        cmd_xz,
        cmd_zst,
        cmd_br,
        cmd_bzip2,
        cmd_gzip,
        cmd_arj ]
    return arr


def get_info_arch(app: str) -> str:
    try:
        if app in ("rar", "7z"):
            # Эти утилиты печатают версию без аргументов
            result = subprocess.run(
                [app],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=3
            )
            lines = result.stdout.strip().splitlines()
        else:
            # Общие флаги
            for flag in ["--version", "-v", "-V"]:
                result = subprocess.run(
                    [app, flag],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    timeout=3
                )
                if result.stdout:
                    lines = result.stdout.strip().splitlines()
                    break
            else:
                return "unknown"

        # Ищем строку с номером версии
        for line in lines:
            if re.search(r"\d+\.\d+", line):
                return line.strip()
        # Если ничего не нашли — вернём первую строку
        return lines[0] if lines else "unknown"

    except FileNotFoundError:
        return "not found"
    except Exception as e:
        return f"error: {e}"


def get_arr_info():
    archver_versions: list[str] = [f"{get_info_arch(app)}" for app in ARCHS_APPS]
    return archver_versions

def get_arr_filtered_version( arr: list[str]):
    filtered = []
    for line in arr:
        match = re.search(r"\d+(?:\.\d+)+", line)
        if match:
            prefix = line.split(":")[0]
            version = match.group(0)
            filtered.append(f"{version}")

    return filtered


if __name__ == "__main__":
    
    for vermsg in get_arr_info():
       print(f"{vermsg}")

