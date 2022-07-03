# DDK_Shared_Memory
Introduction of shared memory

# Prior Knowledge
1. Memory-mapped file
2. Page cache

# How to implement it?
1. anonymous mapping: mmap() + fork()
2. file mapping: open() + ftruncate() + mmap()

# Let us test it
```sh
./out -m
```

```sh
./out -1
```

```sh
./out -2 -w hello
./out -2 -r read
```

```sh
./out -2 -w 1
./out -2 -w 2
```

# References
1. https://man7.org/linux/man-pages/man2/mmap.2.html
2. https://en.wikipedia.org/wiki/Memory-mapped_file
3. https://en.wikipedia.org/wiki/Page_cache
