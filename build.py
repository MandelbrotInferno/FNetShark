import platform
import subprocess
import sys

def detect_platform():
    system = platform.system()
    if system == "Windows":
        return "windows-vs2022-debug"
    elif system == "Linux":
        return "linux-debug"
    else:
        print(f"Unsupported operating system: {system}")
        sys.exit(1)

def run_cmake_preset(preset_name):
    try:
        print(f"Running: cmake --preset {preset_name}")
        subprocess.check_call(["cmake", "--preset", preset_name])
    except subprocess.CalledProcessError as e:
        print(f"Error during solution generation: {e}")
        sys.exit(e.returncode)

if __name__ == "__main__":
    preset = detect_platform()
    run_cmake_preset(preset)
