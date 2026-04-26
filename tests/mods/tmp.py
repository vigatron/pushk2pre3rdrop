# MD5_OUT_RESULT_EXPECTED = "456"
# MD5_OUT_RTABLE_EXPECTED = ""

# EXAMPLES_DIR = os.path.join(BASE_DIR, "..", "examples")
# EXAMPLE_OFFS = "6144"
# os.path.join(EXAMPLES_DIR, "trix1.scr") , EXAMPLE_OFFS

# self.assertIn("7E8BC0800080F0807A8BC288F2FBFBE400000000000000000000000000000000", result.stdout)

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
