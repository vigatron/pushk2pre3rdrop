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

# EXAMPLES_DIR = os.path.join(BASE_DIR, "..", "examples")
# EXAMPLE_OFFS = "6144"
# os.path.join(EXAMPLES_DIR, "trix1.scr") , EXAMPLE_OFFS

# 
EXPECTED1 = ["37fb285e628b0c085f5581bc3d340d24", 16384]
MD5_OUT_RESULT_EXPECTED = "456"
MD5_OUT_RTABLE_EXPECTED = ""


def FILEMD5Sum(fname):
	result = subprocess.run( ["md5sum", fname], capture_output=True, text=True)
	md5 = result.stdout.split()[0].strip()
	return md5


class TestMyProgram(unittest.TestCase):

    def test_version(self):
        result = subprocess.run( [TOOL_NAME_FULL, "v"], capture_output=True, text=True )
        self.assertEqual(result.returncode, 0)
        # self.assertIn("7E8BC0800080F0807A8BC288F2FBFBE400000000000000000000000000000000", result.stdout)


    def test_full(self):
        file_name = os.path.join(EXAMPLES_DIR, "example7b.bin")
        real_filesize = os.path.getsize(file_name)
        real_md5 = FILEMD5Sum( file_name )
        self.assertEqual(real_filesize, EXPECTED1[1])
        self.assertEqual(real_md5, EXPECTED1[0])


    # def test_success_case_s(self):
    #     result = subprocess.run(
    #         [os.path.join(BUILD_DIR, TOOL_NAME), "s", os.path.join(EXAMPLES_DIR, "trix1.scr"), EXAMPLE_OFFS],
    #         capture_output=True,
    #         text=True
    #     )
    #     self.assertEqual(result.returncode, 0)
    #     self.assertIn("56", result.stdout)

    # def test_error_case(self):
    #     result = subprocess.run(
    #         [os.path.join(BUILD_DIR, TOOL_NAME), "n", os.path.join(BASE_DIR, "..", "exampl", "trix1.scr"), EXAMPLE_OFFS],
    #         capture_output=True,
    #         text=True
    #     )
    #     self.assertNotEqual(result.returncode, 0)

if __name__ == "__main__":
    unittest.main()
