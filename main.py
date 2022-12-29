import subprocess
import json

path = "C:\\Users\\giaco\\Desktop\\sokoban-generator\\Sokoban.exe"

height = 8
width = 8
boxCount = 3

output = list(subprocess.check_output([path, str(height), str(width), str(boxCount)]).decode("utf-8"))
for i in range(len(output)):
    if output[i] == "\b":
        output[i-1] = ""
        output[i] == ""

output = "".join(output).replace("\b", "")
output = json.loads(output)

print(output["level"])
