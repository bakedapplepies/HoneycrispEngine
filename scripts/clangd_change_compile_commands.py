# for dev work

import json
import os
import sys


if __name__ == "__main__":
    n_arguments = len(sys.argv)
    if (n_arguments != 2):  # the first element is the name of the program
        raise Exception("CLI arguments not 1.")

    if sys.argv[1] != "Release" and sys.argv[1] != "Debug":
        raise Exception("CLI arguments can only be 'Debug' or 'Release'.")

    # get path to settings.json
    current_path = os.path.dirname(os.path.realpath(__file__))
    new_path = current_path + "/../.vscode/settings.json"

    data: dict = None
    with open(new_path, "r") as compile_command:
        data = json.load(compile_command)

    with open(new_path, "w") as compile_command:
        if sys.argv[1] == "Debug":
            data["clangd.arguments"][0] = data["clangd.arguments"][0].replace("Release", "Debug")
        else:
            data["clangd.arguments"][0] = data["clangd.arguments"][0].replace("Debug", "Release")

        compile_command.write(json.dumps(data, indent=4))
