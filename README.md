# slog
This is just a basic logger (that also provides colourful stdout output)

Requires C++17

It is not supposed to be performant or even smart.
Just basic output (in a simple date-based file) with colour

Make sure you `simple_logger::set_basename("my-log-file-name");` before you use it.
You do not have to, but you will just get a filename like `10-07-22.txt`. Up to you.

By default it logs to output but you can disable that `simple_logger::set_stdout(false);`

```
#include <slog/slog.hpp>

int main(int argc, char* argv[]) {
    // simple_logger::set_stdout(false);
    simple_logger::set_basename("my-server");

    slog(info) << "All is good in the hood.... Order a pizza...";
    slogf(info) << "... and don't forget to get a beer too ...";
    slog(warn) << "SOMETHING WICKED THIS WAY COMES";
    slogf(warn) << "oh ... its the pizza man...";
    slog(error) << "... but he didnt bring a beer ...";
    slogf(error) << "----------SORT THIS OUT MATE -----------";

    return 0;
}
```

![Alt text](example.png?raw=true "Title")
