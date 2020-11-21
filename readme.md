# Critter 

Critter is a (very) naive web crawler.

## Features

- Stores visited urls in log

## Improvements

- Storing visited pages in a hashmap is slow, will need to store in a trie or cloud database with indexing
- hyperlink regular expressions have not been thoroughly tested so many errors await (I'm guessing)
- Multithreading
- Memory management

## How to run


`git clone http::/github.com/zef/critter.git`

`g++ -o critter src/main.cpp -lcurl`

`./critter`

There are currently 3 root website nodes:
- bbc.co.uk
- theguardian.co.uk
- runescape.com

Edit the code to change/add root nodes
