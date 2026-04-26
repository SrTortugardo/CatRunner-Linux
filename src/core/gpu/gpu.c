/* This "works", but in some setups like laptops the GPU is card1 so maybe cant work. */
/* So... the solution is to see which GPU is using the wayland compositor, if i rebember well is on /dev/dri/ */
/* So.. why i dont do that?, because it wouldnt work with Xorg i guess*/

#include <stdio.h>
#include <string.h>

const char*
detect_gpu_vendor()
{
    FILE* fp = fopen("/sys/class/drm/card0/device/vendor", "r"); // This File Contains The Vendor
    if (!fp) {
        return "Unknown";
    }
    
    char id[16] = {0};
    fgets(id, sizeof(id), fp);
    fclose(fp);

    if (strstr(id, "0x10de")) return "NVIDIA";
    if (strstr(id, "0x1002")) return "AMD";
    if (strstr(id, "0x8086")) return "Intel";

    return "Unknown"; // This is if anything else works
}

float
get_gpu_usage()
{
    const char* vendor = detect_gpu_vendor();

    // NVIDIA
    if (strcmp(vendor, "NVIDIA") == 0) {
        FILE *p = popen("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits", "r"); // not tested cuz i dont have nvidia
        if (!p) {
            return 0;
        }

        float usage = 0;
        fscanf(p, "%f", &usage);
        pclose(p);
        return usage;
    }

    if (strcmp(vendor, "AMD") == 0)
    {
        FILE* fp = fopen("/sys/class/drm/card0/device/gpu_busy_percent", "r"); // easy right?
        if (!fp) {
            return 0;
        }

        float usage = 0;
        fscanf(fp, "%f", &usage);
        fclose(fp);
        return usage;
    }

    if (strcmp(vendor, "Intel") == 0)
    {
        FILE* fp = fopen("/sys/class/drm/card0/gt_busy_percent", "r");
        if (!fp) {
            return 0;
        }
        
        float usage = 0;
        fscanf(fp, "%f", &usage);
        fclose(fp);
        return usage;
    }
    return 0;
}

const char*
get_gpu_name() // returns the answer on string/char*
{
    static char name[256] = {0};

    FILE* fp = popen("lspci | grep -i 'vga\\|3d\\|display'", "r");
    if (!fp)
    {
        return "Unknown GPU";
    }

    char buffer[512] = {0};
    fgets(buffer, sizeof(buffer), fp);
    pclose(fp);

    char* start = strstr(buffer, ": "); // AFTER CONTROLLER!
    if (start) start += 2;
    else start = buffer;

    // copy texxt
    strncpy(name, start, sizeof(name));

    // delete newline so printf doesnt look weird
    name[strcspn(name, "\n")] = 0;

    return name;
}

