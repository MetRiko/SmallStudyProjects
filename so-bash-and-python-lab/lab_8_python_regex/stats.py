import re
from collections import Counter 

text = "This is my text with words. And is this its end? And this is all about nothing!"

printLists = True

letters = re.findall(r"\w", text)
print("letters: {}".format(len(letters)))
if(printLists): print("{}\n".format(letters))

chars = re.findall(r"[^\s]", text)
print("chars: {}".format(len(chars)))
if(printLists): print("{}\n".format(chars))

words = re.findall(r"\b(\w+)\b", text)
print("words: {}".format(len(words)))
if(printLists): print("{}\n".format(words))

sentences = re.findall(r"\b([\w\s]+[\.!?])", text)
print("sentences: {}".format(len(sentences)))
if(printLists): print("{}\n".format(sentences))

textWithoutCase = text.lower()
wordsWithoutCase = re.findall(r"\b([a-z]+)", text)
topWordsNum = 3
topWords = Counter(wordsWithoutCase).most_common(topWordsNum)
print("Top {} words:".format(topWordsNum))
place = 1
for word in topWords:
    print("{}. {} : {}".format(place, word[0], word[1]))
    place += 1
    
