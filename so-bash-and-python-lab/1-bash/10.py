import sys
import os

print()

files_1 = set([f for f in os.listdir(sys.argv[1]) if os.path.isfile(os.path.join(sys.argv[1], f))])
files_2 = set([f for f in os.listdir(sys.argv[2]) if os.path.isfile(os.path.join(sys.argv[2], f))])

print([files_1.intersection(files_2)])

print()
