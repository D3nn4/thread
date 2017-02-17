print "Python"
import random
import string
import collections

file = open("text.txt", "w")

for x in range(0, 10000):
	for y in range(0, 10000):
		file.write(random.choice(string.ascii_lowercase))
file.close()

file = open("text.txt", "r")
text = file.read()
c = collections.Counter(text)
print c.most_common(1) 

file.close()
