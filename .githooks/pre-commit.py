#!/usr/bin/env python3
"""Project pre-commit git hook.
    Validates files worthy of commit using clang-format, clang-tidy, and pylint.
    
    Clang-format must be functional, formatting commits is mandatory.
      If clang fails to format a file you will be unable to commit.
      Runs before clang-tidy as to reinforce style standards early.
      
    Clang-tidy must be functional, analyzing commits is mandatory.
      If clang fails to check a file or if you do not pass its checks,
                                          you will be unable to commit.
    
    Pylint must be functional, pylint checking is mandatory.
      If pylint fails to check a file or if you do not pass its checks,
                                          you will be unable to commit.
"""

import subprocess
import sys

def run_cmake_build():
    """Runs cmake build for current cmake configuration.
    Fails if project build is unsuccessful.
    (All commits must be able to compile properly.)
    """
    result = subprocess.run(["cmake", "--build", "build/"],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            text=True, check=False)
    if result.returncode != 0:
        print("Failed to build project.")
        print(result.stderr)
        print(result.stdout)
        print("Aborting Commit.")
        print("Project must compile to commit.")





def run_clang_format(staged_c_family_files):
    """Runs clang-format against all staged c-family files.
        Fails only if clang-format encounters an error.
        (formatting is NOT optional and will result in immediate failure if not functional.)

    Args:
        stagedCFamilyFiles (type=File-Dict): self-explanatory.
    """
    for c_family_file in staged_c_family_files:
        print(f"Running Clang-Format on: {c_family_file}")
        result = subprocess.run(["clang-format", "-i", c_family_file], check=False)
        if result.returncode != 0:
            print(f"Failed to run Clang-format on file: {c_family_file}")
            print("Aborting Commit.")
            print("Clang-format is required for style standards.",
                  "Please resolve issue before trying again.")
            sys.exit(1)




def run_clang_tidy(staged_c_family_files):
    """Runs clang-tidy against all staged c-family files.
        Fails if clang-tidy reports any problems.
        (clang-tidy rules are defined in projectRoot/.clang-tidy)

    Args:
        stagedCFamilyFiles (type=File-Dict): self-explanatory.
    """
    failures = []
    for c_family_file in staged_c_family_files:
        print(f"Running Clang-tidy on: {c_family_file}")
        result = subprocess.run(["clang-tidy", "-p", "build/", c_family_file],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True, check=False)
        subprocess.run(["git", "add", c_family_file], check=True)

        if result.returncode != 0:
            failures.append(c_family_file)
            print(f"Failed to pass Clang-tidy checks on file: {c_family_file}")
            print(result.stderr)
            print(result.stdout)

    if failures:
        print("Aborting Commit.")
        print("Please resolve issues in files: ",",".join(failures))
        sys.exit(1)




def run_pylint(staged_python_files):
    """Runs Pylint against all staged python files.
        Fails if pylint reports any problems.

    Args:
        stagedPythonFiles (type=File-Dict): self-explanatory.
    """
    failures = []
    for python_file in staged_python_files:
        print(f"Running Pylint on: {python_file}")
        result = subprocess.run(["pylint",
                                "--max-line-length=120",
                                "--module-naming-style=any",
                                python_file],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True, check=False)
        if result.returncode != 0:
            failures.append(python_file)
            print(f"Failed to pass Pylint checks on file: {python_file}")
            print(result.stderr)
            print(result.stdout)

    if failures:
        print("Aborting Commit.")
        print("Please resolve issues in files: ",",".join(failures))
        sys.exit(1)




if __name__ == "__main__":
    print("Pre-Commit hook is running...")
    staged_files = [file.strip() for file in subprocess.run(["git", "diff",
                                                             "--cached", "--name-only",
                                                             "--diff-filter=ACM"],
                                stdout=subprocess.PIPE,
                                text=True, check=True).stdout.splitlines() if file.strip()]

    staged_c_family = [file for file in staged_files if file.endswith((".cpp", ".hpp"))]
    staged_python = [file for file in staged_files if file.endswith(".py")]


    if staged_c_family:
        run_cmake_build()
        run_clang_format(staged_c_family)
        run_clang_tidy(staged_c_family)

    if staged_python:
        run_pylint(staged_python)

    print("Passed all pre-commit checks.")
    sys.exit(0)
