import subprocess
import os


os.chdir("./bin/intermediates")
files = os.listdir()
for file in files:
    os.remove(file)

cmd_str = "buildrun.bat"
subprocess.run([cmd_str])