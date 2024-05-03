# Automatically adds shaders (.glsl files) from "resources/shaders/postprocessing"

import sys
import json
import os
from os import listdir
from os.path import isfile, join


if __name__ == "__main__":
    n_arguments = len(sys.argv)
    if (n_arguments != 2):  # the first element is the name of the program
        raise Exception("CLI arguments not 1.")
    
    serialized_postprocessing_dir = sys.argv[1] + "/src/serialized/postprocessing"
    serialized_postprocessing_file = serialized_postprocessing_dir + "/postprocessing_shaders.json"
    if not os.path.exists(serialized_postprocessing_dir):
        os.makedirs(serialized_postprocessing_dir)

    # get path to settings.json
    current_path = os.path.dirname(os.path.realpath(__file__))
    new_path = current_path + "/../resources/shaders/postprocessing"

    files = [file for file in listdir(new_path) if isfile(join(new_path, file))]
    files = [file.replace(".glsl", "") for file in files if file != "ScreenQuadVertex.glsl"]
    json_obj = {
        "file_names": files
    }
    
    with open(serialized_postprocessing_file, "w") as file:
        file.write(json.dumps(json_obj, indent=4))
