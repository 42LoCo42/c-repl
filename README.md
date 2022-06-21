# c-repl
## Usage
`$` indicates a shell prompt, `C:\>` a c-repl prompt.
```bash
$ make
$ ./repl
C:\> ? O_WRONLY|O_CREAT fcntl
65
C:\> open my_file 65 420
3
C:\> write 3 abcde 5
5
C:\> close 3
0
C:\> exit
$ cat my_file
abcde$
```
## Special functions
- `?`: Lookup a symbol (or a combination of symbols) in some headers
- `/`: Load a library. E.g. to load libsodium, do: `/ sodium`
## Limitations / TODOs
- Numbers are only read as base 10. In the example above, it would be better to enter 0644 (octal) instead of 420.
- String input should understand some escape sequences and (optionally) quoting (or else spaces would have to be escaped as well)
- We need variables & autocompletion!
