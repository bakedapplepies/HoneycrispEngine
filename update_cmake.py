import os
from subprocess import Popen
import time
    

def edit_cmake():
    srcList: list = []
    with open(".\\CMakeLists.txt", "r") as cmake:
        srcList = cmake.readlines();
        
        i: int = 0
        while i < len(srcList):
            if ("set(CORE" in srcList[i]   or
                "set(SCENES" in srcList[i] or
                "set(APPLICATION" in srcList[i]):
                i += 1
                while (")\n" != srcList[i]):
                    srcList.pop(i)
            i += 1
                
        for root, dirs, files in os.walk("./src"):
            if (root == "./src\\core"):
                i: int = 0
                while i < len(srcList):
                    if ("set(CORE" in srcList[i]):
                        for file in files:
                            if (os.path.splitext(file)[1] == ".cpp"):
                                srcList.insert(i + 1, "\t" + root.replace("\\", "/") + "/" + file + '\n')
                        break
                    i += 1
                continue
            elif (root == "./src\\scenes"):
                i: int = 0
                while i < len(srcList):
                    if ("set(SCENES" in srcList[i]):
                        for file in files:
                            if (os.path.splitext(file)[1] == ".cpp"):
                                srcList.insert(i + 1, "\t" + root.replace("\\", "/") + "/" + file + '\n')
                        break
                    i += 1
                continue
            elif (root == "./src"):
                i: int = 0
                while i < len(srcList):
                    if ("set(APPLICATION" in srcList[i]):
                        for file in files:
                            if (os.path.splitext(file)[1] == ".cpp"):
                                srcList.insert(i + 1, "\t" + root.replace("\\", "/") + "/" + file + '\n')
                        break
                    i += 1
                continue
        
    with open(".\\CMakeLists.txt", "w") as cmake:
        cmake.writelines(srcList);

if __name__ == "__main__":
    edit_cmake()
    p = Popen("build.bat", cwd="./")
    stdout, stderr = p.communicate()
    