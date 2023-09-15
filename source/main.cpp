#include <iostream>
#include <cstdarg>
#include <string>
#include <fstream>
#include <memory>
#include <cstdio>

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

int main()
{
    std::string command = "timeout 3 chromium --log-level=0 --alsa-check-close-timeout=1000 --noerrdialogs --validate-input-event-stream --headless --disable-gpu --no-sandbox --dump-dom ";
    std::string url = "https://r.onliner.by";

    std::string str = exec((command + url).c_str());
    std::ofstream output;

    output.open("log.txt");

    if (output.is_open())
        output << str;

    output.close();

    return 0;
}
