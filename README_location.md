C++ IP-based Location Example

This program uses `libcurl` to call `ip-api.com/json` and prints the JSON plus extracted location fields.

Build (MSYS2/MinGW or similar) from the workspace root `C:\cpp-projects`:

```powershell
g++ .\.vscode\python\firstcpp_location.cpp -o .\.vscode\python\firstcpp_location.exe -lcurl -std=c++17
```

Run:

```powershell
.\.vscode\python\firstcpp_location.exe
```

Optional: pass an IP or hostname to query instead of your own IP:

```powershell
.\.vscode\python\firstcpp_location.exe 8.8.8.8
```

Notes:
- This determines location based on public IP address (approximate). For precise device GPS location you'd need platform-specific APIs and permissions.
- Ensure `libcurl` is installed and visible to your compiler/linker.
