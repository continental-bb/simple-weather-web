OpenWeather C++ Example

This small program queries the OpenWeather Current Weather API and prints the raw JSON response.

Requirements
- `libcurl` development library installed (linker: `-lcurl`).
- A C++17-compatible compiler (e.g., `g++`).

Build

On MSYS2 / MinGW (Windows):
```powershell
g++ ./.vscode/python/firstcpp.cpp -o ./.vscode/python/firstcpp -lcurl -std=c++17
```

On Linux / macOS:
```bash
g++ ./.vscode/python/firstcpp.cpp -o firstcpp -lcurl -std=c++17
```

Run

Set your OpenWeather API key in the environment or pass it as the second argument.

Example (environment):
```powershell
$env:OPENWEATHER_API_KEY = "YOUR_API_KEY"
./.vscode/python/firstcpp.exe London
```

Example (args):
```powershell
./.vscode/python/firstcpp.exe London YOUR_API_KEY
```

Notes
- The program prints HTTP status code and raw JSON. You can pipe the output to `jq` for pretty-printing or add a JSON library to parse it in C++.
- To get an API key, sign up at https://openweathermap.org/.
