#!/usr/bin/env python3
"""Sets local git hooks to project defined hooks.
    (DO NOT USE IF YOU ARE NOT CONTRIBUTING OR FORKING)
    
    If you are contributing:
      These hooks are MANDATORY.
      Any avoidance of project standards will result in you being barred from further contributions.
      Preservation of standards and conventions is absolutely essential to maintainability.
    
    If you are forking:
      These hooks are recommended.
      As you develop your fork you should strive to maintain your own standards and conventions,
      its highly recommended that you keep using everything available at the time of your fork.
"""
import os
import sys
import shutil
import stat


if __name__ == "__main__":
    print("Setting local git hooks to Venus-githooks")

    GIT_HOOKS_DIR = "./.git/hooks"
    VENUS_HOOKS_DIR = "./.githooks"

    if ".git" in os.listdir("."):
        print("Found .git directory.")
    else:
        print("Failed to find .git directory.")
        sys.exit(1)

    if ".githooks" in os.listdir("."):
        print("Found Venus .githooks directory.")
    else:
        print("Failed to find Venus .githooks directory.")
        print("Please verify project files.")
        sys.exit(1)

    for file in os.listdir(VENUS_HOOKS_DIR):
        if file.endswith(".py"):
            input_hook_path = os.path.join(VENUS_HOOKS_DIR, file)
            finished_hook = os.path.splitext(file)[0]
            output_hook_path = os.path.join(GIT_HOOKS_DIR, finished_hook)

            shutil.copy2(input_hook_path, output_hook_path)
            os.chmod(output_hook_path, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
            print(f"Succeeded in setting local git hook: {finished_hook}")

    print("Finished setting hooks.")
    print("Please verify local hooks are set as project hooks.")
    sys.exit(0)
