# System Resource Monitor

A lightweight, cross-platform system telemetry monitor built with a **C++ backend engine** and a **Python terminal dashboard**. The system utilizes JSON file-based Inter-Process Communication (IPC) to pass real-time physical memory telemetry from the backend collector to the frontend ASCII display.

---

## Architecture & Inter-Process Communication
+------------------------------------+                         +------------------------------------+
|        C++ Telemetry Engine        |   Writes every 2s       |      Python Terminal Dashboard     |
|         ./monitor.exe              | ----------------------> |      python src/dashboard.py       |
| Queries physical OS RAM via APIs   | data/system_stats.json  | Reads JSON & renders ASCII UI      |
+------------------------------------+                         +------------------------------------+

### Component Breakdown
1. **Backend Collector (`src/monitor.cpp`)**:
   - Direct integration with OS APIs (`windows.h` on Windows / `<sys/sysinfo.h>` on Linux).
   - Queries total physical memory and available physical memory.
   - Calculates RAM usage percentages and exports telemetry data to `data/system_stats.json` every 2 seconds.
2. **Frontend UI (`src/dashboard.py`)**:
   - Reads telemetry metrics from `data/system_stats.json`.
   - Generates a real-time ASCII progress bar representation.
   - Built-in exception handling to manage missing files or concurrent read/write locks gracefully.

---

## Prerequisites & Setup

### Required Tools
- **C++ Compiler**: `g++` (GCC 11+ or equivalent supporting C++11 standard).
- **Python**: Python 3.8+ (No external `pip` packages required; uses standard libraries `json`, `os`, `time`).
- **Terminal**: Git Bash (Windows) or standard Unix terminal.

---

## Building and Running

### 1. Compile the C++ Engine
In your terminal, compile the C++ backend:

```bash
g++ -std=c++11 src/monitor.cpp -o monitor