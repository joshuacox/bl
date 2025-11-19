#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cctype>
#include <algorithm>   // added for std::all_of
#include <unistd.h>    // for geteuid()

namespace fs = std::filesystem;

/**
 * Utility to get/set screen brightness on Linux using the sysfs backlight interface.
 *
 * Usage:
 *   bl <percentage>
 *   bl +<delta>
 *   bl -<delta>
 *
 *   <percentage> : integer 0-100 (sets absolute brightness)
 *   +<delta>     : increase brightness by <delta> percent
 *   -<delta>     : decrease brightness by <delta> percent
 *
 * The program must be run with sufficient permissions to write to the
 * brightness sysfs file (typically as root or via sudo).
 */

static const std::string BACKLIGHT_ROOT = "/sys/class/backlight";

/**
 * Find the first backlight device directory.
 * Returns empty string if none found.
 */
std::string find_backlight_path() {
    if (!fs::exists(BACKLIGHT_ROOT) || !fs::is_directory(BACKLIGHT_ROOT)) {
        return "";
    }

    for (const auto& entry : fs::directory_iterator(BACKLIGHT_ROOT)) {
        if (fs::is_directory(entry.path())) {
            return entry.path().string();
        }
    }
    return "";
}

/**
 * Read an integer value from a file.
 */
bool read_int_from_file(const std::string& path, int& value) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }
    in >> value;
    return !in.fail();
}

/**
 * Write an integer value to a file.
 */
bool write_int_to_file(const std::string& path, int value) {
    std::ofstream out(path);
    if (!out.is_open()) {
        return false;
    }
    out << value;
    return !out.fail();
}

/**
 * Print usage information.
 */
void usage(const std::string& prog_name) {
    std::cerr << "USAGE: " << prog_name << " <percentage>\n"
              << "       " << prog_name << " +<delta>\n"
              << "       " << prog_name << " -<delta>\n"
              << "  where <percentage> is 0-100 (absolute brightness)\n"
              << "        +<delta>    increases brightness by delta percent\n"
              << "        -<delta>    decreases brightness by delta percent\n";
}

/**
 * Main program.
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    std::string arg = argv[1];
    if (arg.empty()) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Locate backlight sysfs directory
    std::string bl_path = find_backlight_path();
    if (bl_path.empty()) {
        std::cerr << "Error: No backlight device found under " << BACKLIGHT_ROOT << "\n";
        return EXIT_FAILURE;
    }

    std::string brightness_file = bl_path + "/brightness";
    std::string max_file       = bl_path + "/max_brightness";

    int max_brightness = 0;
    int cur_brightness = 0;

    if (!read_int_from_file(max_file, max_brightness) || max_brightness <= 0) {
        std::cerr << "Error: Unable to read max brightness from " << max_file << "\n";
        return EXIT_FAILURE;
    }

    if (!read_int_from_file(brightness_file, cur_brightness)) {
        std::cerr << "Error: Unable to read current brightness from " << brightness_file << "\n";
        return EXIT_FAILURE;
    }

    int target_brightness = cur_brightness; // default

    // Determine target based on argument format
    if (arg[0] == '+' || arg[0] == '-') {
        // Relative change
        bool is_plus = (arg[0] == '+');
        std::string num_str = arg.substr(1);
        if (num_str.empty() || !std::all_of(num_str.begin(), num_str.end(), ::isdigit)) {
            std::cerr << "Error: Invalid relative argument '" << arg << "'\n";
            usage(argv[0]);
            return EXIT_FAILURE;
        }
        int delta_percent = std::stoi(num_str);
        int delta = max_brightness * delta_percent / 100;
        target_brightness = is_plus ? cur_brightness + delta : cur_brightness - delta;
    } else {
        // Absolute percentage
        if (!std::all_of(arg.begin(), arg.end(), ::isdigit)) {
            std::cerr << "Error: Invalid percentage argument '" << arg << "'\n";
            usage(argv[0]);
            return EXIT_FAILURE;
        }
        int percent = std::stoi(arg);
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
        target_brightness = max_brightness * percent / 100;
    }

    // Clamp target to valid range
    if (target_brightness > max_brightness) target_brightness = max_brightness;
    if (target_brightness < 0) target_brightness = 0;

    if (target_brightness == cur_brightness) {
        std::cout << "Brightness already at requested level (" << cur_brightness << "). No change.\n";
        return EXIT_SUCCESS;
    }

    if (geteuid() != 0) {
        std::cerr << "Error: This program must be run as root to modify brightness.\n";
        return EXIT_FAILURE;
    }
    if (!write_int_to_file(brightness_file, target_brightness)) {
        std::cerr << "Error: Unable to write new brightness to " << brightness_file << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Brightness changed from " << cur_brightness
              << " to " << target_brightness << " ("
              << (target_brightness * 100 / max_brightness) << "% of max).\n";

    return EXIT_SUCCESS;
}
