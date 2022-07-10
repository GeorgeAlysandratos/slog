# slog
This is just a basic logger (that also provides colourful stdout output)

It is not supposed to be performant or even smart.
Just basic output (in a simple date-based file) with colour

Make sure you `simple_logger::set_basename("my-log-file-name");` before you use it

By default it logs to output but you can disable that `simple_logger::set_stdout(false);`

```
#include <slog/slog.hpp>

int main(int argc, char* argv[]) {
	// simple_logger::set_stdout(false);
	simple_logger::set_basename("my-server");

	int count = 0;
	while(count < 2) {
		slog(info) << "All is good in the good. Order a pizza...";
		slog(info) << "... and don't forget your beer";
		slog(warn) << "SOMETHING WICKED THIS WAY COMES";
		slog(warn) << "... its the pizza man...";
		slog(error) << "... but he didnt bring a beer ...";
		slog(error) << "----------SORT THIS OUT MATE -----------";
		++count;
	}

	return 0;
}
```