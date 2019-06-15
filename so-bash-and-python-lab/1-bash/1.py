import sys
import os

files = os.listdir(sys.argv[1])

#print(files)
print()

for f in files:
    path = os.path.join(sys.argv[1], f)
    name, ext = os.path.splitext(f)
    if ext == '.old':
        print("remove: ", path)
        #os.remove(path)
    else:
        dst = os.path.join(sys.argv[1], f + ".old")
        print("rename: ", path, " -> ", dst)
        #os.rename(path, dst)
        
