import sys
import json

if __name__ == "__main__":
    if (len(sys.argv) != 2):
        raise Exception("CLI arguments not 1.")

    build_type: str = sys.argv[1]
    jFile = None
    with open("./settings.json", "r") as f:
        jFile = json.load(f)
    
    workspaceFolderMacro: str = "${workspaceFolder}"
    jFile["clangd.arguments"] = []
    jFile["clangd.arguments"].append(f"--compile-commands-dir={workspaceFolderMacro}/build/{build_type}")
    jFile["clangd.arguments"].append("--header-insertion=never")
    jFile["clangd.arguments"].append("--query-driver=C:/MinGW-W64/MinGW-W64/bin/clang++.exe")

    with open("./settings.json", "w") as f:
        f.write(json.dumps(jFile, indent=4))