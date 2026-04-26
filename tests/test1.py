import unittest
import subprocess
import os

LOCAL_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(LOCAL_DIR)

BUILD_DIR = os.path.join(ROOT_DIR, "build")
EXAMPLES_DIR = os.path.join(ROOT_DIR, "examples")
RESULTS_DIR = os.path.join(ROOT_DIR, "results")

TOOL_NAME = "pushk2pre3rdrop"
TOOL_NAME_FULL = os.path.join(BUILD_DIR, TOOL_NAME)

#
EXAMPLE1_FNAME = "example7b.bin"
EXAMPLE1_EXPECTED = ["37fb285e628b0c085f5581bc3d340d24", 16384]


def FILEMD5Sum(fname):
	result = subprocess.run( ["md5sum", fname], capture_output=True, text=True)
	md5 = result.stdout.split()[0].strip()
	return md5


class TestMyProgram(unittest.TestCase):

    def test_version(self):
        result = subprocess.run( [TOOL_NAME_FULL, "v"], capture_output=True, text=True )
        self.assertEqual(result.returncode, 0)


    def test_full(self):
        file_name = os.path.join(EXAMPLES_DIR, EXAMPLE1_FNAME)
        real_filesize = os.path.getsize(file_name)
        real_md5 = FILEMD5Sum( file_name )
        self.assertEqual(real_filesize, EXAMPLE1_EXPECTED[1])
        self.assertEqual(real_md5, EXAMPLE1_EXPECTED[0])


if __name__ == "__main__":
    unittest.main()
