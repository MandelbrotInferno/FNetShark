import os
import platform
import subprocess
import sys

def run_command(command, shell=False):
    try:
        subprocess.check_call(command, shell=shell)
    except subprocess.CalledProcessError as e:
        print(f"Command failed: {e}")
        sys.exit(1)

def main():
    repo_url = "https://github.com/microsoft/vcpkg.git"
    vcpkg_dir = "vcpkg"

    # Clone vcpkg if it doesn't exist
    if not os.path.isdir(vcpkg_dir):
        print(f"Cloning vcpkg from {repo_url}...")
        run_command(["git", "clone", repo_url])

    # Determine OS and run the appropriate bootstrap command
    system = platform.system()
    print(f"Detected OS: {system}")

    if system == "Windows":
        bootstrap_script = os.path.join(vcpkg_dir, "bootstrap-vcpkg.bat")
        run_command([bootstrap_script], shell=True)
    else:
        bootstrap_script = os.path.join(vcpkg_dir, "bootstrap-vcpkg.sh")
        run_command(["chmod", "+x", bootstrap_script])
        run_command([bootstrap_script])

    print("vcpkg bootstrapped successfully.")

if __name__ == "__main__":
    main()
