#include <iostream>  // For printing standard output logs to the terminal
#include <fstream>   // For creating and writing data to files (JSON output)
#include <chrono>    // For handling time units (e.g., 2 seconds delay)
#include <thread>    // For pausing/sleeping the program execution thread

// Operating system detection to run platform-specific memory API calls
#ifdef _WIN32
#include <windows.h> // Windows system header to fetch system memory status
#else
#include <sys/sysinfo.h> // Linux system header to fetch system memory status
#endif

// Function to query system memory and write metrics into data/system_stats.json
void write_stats_json() {
    double total_ram_gb = 0.0;
    double free_ram_gb = 0.0;
    double used_ram_pct = 0.0;

#ifdef _WIN32
    // Windows API struct to hold RAM metrics
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    // Convert bytes into Gigabytes (Divide by 1024^3)
    total_ram_gb = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
    free_ram_gb = memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
    used_ram_pct = memInfo.dwMemoryLoad; // Percentage of memory in use
#else
    // Linux API struct to hold RAM metrics
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        total_ram_gb = info.totalram / (1024.0 * 1024.0 * 1024.0);
        free_ram_gb = info.freeram / (1024.0 * 1024.0 * 1024.0);
        used_ram_pct = ((total_ram_gb - free_ram_gb) / total_ram_gb) * 100.0;
    }
#endif

    // Open file stream to output JSON payload
    std::ofstream file("data/system_stats.json");
    if (file.is_open()) {
        file << "{\n";
        file << "  \"total_ram_gb\": " << total_ram_gb << ",\n";
        file << "  \"free_ram_gb\": " << free_ram_gb << ",\n";
        file << "  \"used_ram_pct\": " << used_ram_pct << "\n";
        file << "}\n";
        file.close(); // Close stream to save changes
    }
}

int main() {
    std::cout << "[C++ Engine] Starting System Resource Monitor backend...\n";
    std::cout << "[C++ Engine] Writing metrics to data/system_stats.json every 2s...\n";

    // Infinite loop acting as a background daemon/worker service
    while (true) {
        write_stats_json();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause execution for 2s
    }
    return 0;
}