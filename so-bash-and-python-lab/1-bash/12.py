import sys
import os

#print(os.getcwd())
#os.chdir(sys.argv[1])
#print(os.getcwd())

dirBase = sys.argv[1]

for d in os.listdir(dirBase):
    dp = os.path.join(dirBase, d)
    if os.path.isdir(dp):
        print('\n', dp)
        for f in os.listdir(dp):
            src = os.path.join(dp, f)
            dst = os.path.join(dirBase, f)
            print('rename: ', src, ' -> ', dst)
            #os.rename(src, dst)
        print('rmdir: ', dp)
        #os.rmdir(dp)
    

#files_1 = set([f for f in os.listdir(sys.argv[1]) if os.path.isfile(os.path.join(sys.argv[1], f))])
#files_2 = set([f for f in os.listdir(sys.argv[2]) if os.path.isfile(os.path.join(sys.argv[2], f))])

#print([files_1.intersection(files_2)])

#print()
