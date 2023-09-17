#include <iostream>
#include <BashExecutor.h>
#include <fstream>

int main()
{
    std::string command = "timeout 10 chromium --log-level=0 --alsa-check-close-timeout=1000 --noerrdialogs --validate-input-event-stream --headless --disable-gpu --no-sandbox --dump-dom ";
    std::string url = "https://r.onliner.by";

    BashExecutor executor;

    std::string str = executor.Exec(command + url);
    std::ofstream output;

    output.open("log.txt");

    if (output.is_open())
        output << str;

    output.close();

    return 0;
}
