import json  # Standard library module to read and parse JSON data
import time  # Standard library module for time/sleep delays
import os    # Standard library module for system commands (clearing screen)

def render_progress_bar(percentage, length=30):
    """
    Renders an ASCII progress bar for memory usage.
    Example: [███████████████---------------] 50.0%
    """
    # Clamp percentage between 0 and 100 for safety
    percentage = max(0.0, min(100.0, percentage))
    
    # Calculate how many full blocks to display
    filled_length = int(length * percentage // 100)
    
    # Construct bar string: '█' for used memory, '-' for free memory
    bar = "█" * filled_length + "-" * (length - filled_length)
    return f"[{bar}] {percentage:.1f}%"

def clear_screen():
    """
    Clears the terminal screen for smooth dashboard re-rendering.
    Supports both Windows ('cls') and Unix/Linux ('clear').
    """
    os.system("cls" if os.name == "nt" else "clear")

def main():
    json_path = "data/system_stats.json"
    print("[Python Dashboard] Initializing System Resource Monitor...")

    while True:
        clear_screen()
        print("================================================")
        print("        SYSTEM RESOURCE MONITOR DASHBOARD       ")
        print("================================================\n")

        try:
            # Open and read the JSON file written by the C++ engine
            with open(json_path, "r") as f:
                data = json.load(f)

            # Extract metrics from JSON dictionary
            total_ram = data.get("total_ram_gb", 0.0)
            free_ram = data.get("free_ram_gb", 0.0)
            used_pct = data.get("used_ram_pct", 0.0)
            used_ram = total_ram - free_ram

            # Print formatted statistics
            print(f" Total RAM      : {total_ram:.2f} GB")
            print(f" Used RAM       : {used_ram:.2f} GB")
            print(f" Free RAM       : {free_ram:.2f} GB")
            print(f" RAM Usage      : {render_progress_bar(used_pct)}")
            print("\n------------------------------------------------")
            print(" Engine Status  : LIVE (Data synced from C++)")

        except (FileNotFoundError, json.JSONDecodeError):
            # Gracefully handle missing file or temporary file write conflicts
            print(" Engine Status  : WAITING FOR C++ ENGINE DATA...")
            print(" (Ensure ./monitor.exe is running in another terminal)")

        print("\nPress Ctrl+C to exit dashboard.")
        time.sleep(2)  # Refresh dashboard display every 2 seconds

if __name__ == "__main__":
    main()