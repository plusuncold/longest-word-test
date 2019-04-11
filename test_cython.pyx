import time

path_to_file = 'corpus.txt'

with open(path_to_file, 'r') as f:
    text = f.read()


# Loop through lines and words in lines and keep track of longest
start = int(round(time.time() * 1000))
longest = ''
lines = text.split('\n')
for line in lines:
    for word in line.split(' '):
        if len(word) > len(longest):
            longest = word

end = int(round(time.time() * 1000)) - start
print(longest)
print('Loop and keep track of longest:\n{} ms\n\n'.format(end))


# Loop through lines and words to create list. Sort list.
start = int(round(time.time() * 1000))
lines = text.split('\n')
words = set()
for line in lines:
    for word in line.split(' '):
        words.add(word)

end = int(round(time.time() * 1000)) - start
print(sorted(
    words,
    key=lambda k: len(k),
    reverse=True
)[0])
print('Loop to build list and sort:\n{} ms\n\n'.format(end))


# Use comprehension to build list and sort it
start = int(round(time.time() * 1000))
print(sorted(
    set([
        x
        for t in text.split('\n')
        for x in t.split(' ')
    ]),
    key=lambda k: len(k),
    reverse=True
)[0])
end = int(round(time.time() * 1000)) - start
print('Use comprehensions:\n{} ms\n\n'.format(end))
