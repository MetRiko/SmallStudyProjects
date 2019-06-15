import re
import sys
import os

def replaceWords(word, newWord, text):
    regxStr = "\\b{}\\b".format(word)
    newText = re.sub(re.compile(regxStr), newWord, text)
    return newText
        

wordsNum = (len(sys.argv)-2) / 2

print("\nReplacements:")
for i in range(wordsNum):
    word = sys.argv[i*2+2]
    newWord = sys.argv[i*2+3]
    print("{} -> {}".format(word, newWord))
    

print("\nBefore replace:")

with open(sys.argv[1]) as f:
    for line in f:
        print(line[:-1])

print("\nAfter replace:")

with open(sys.argv[1]) as f:
    for line in f:
        for i in range(wordsNum):
            word = sys.argv[i*2+2]
            newWord = sys.argv[i*2+3]
            line = replaceWords(word, newWord, line)
        print(line[:-1])

