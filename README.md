# slog
This is just a basic logger (that also provides colourful stdout output)

It is not supposed to be performant or even smart.
Just basic output (in a simple date-based file) with colour

By default it logs to output but you can disable that

```
#include <slog/slog.hpp>

int main(int argc, char* argv[]) {
	// simple_logger::set_stdout(false);

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