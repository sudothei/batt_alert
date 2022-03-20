#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int interval = 1;
int threshold = 9;

int check_charging() {
    char charging_status[12];
    FILE *fp = fopen("/sys/class/power_supply/BAT0/status", "r");
    fgets(charging_status, 12, fp);
    fclose(fp);
    if (strcmp(charging_status, "Discharging") == 0) {
        return 1;
    }
}

int check_threshold() {
    char capacity_str[4];
    FILE *fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    fgets(capacity_str, 4, fp);
    fclose(fp);
    int capacity = atoi(capacity_str);
    if (capacity < threshold) {
        return 1;
    }
}

int send_battery_alert() {
    char alert_command[255] = "notify-send -u critical -c power_supply_low 'Low Battery!' 'Battery ";
    char rest_of_command[255] = "! \nPlease plug in charger.'";
    char threshold_str[10];
    sprintf(threshold_str, "%d", threshold);
    strcat(alert_command, threshold_str);
    strcat(alert_command, rest_of_command);
    system(alert_command);

    for (int i = 1; i <= 9; i++) {
        int tone_int = 497 - ( i * 32 );
        char tone[10];
        sprintf(tone, "%d", tone_int);
        char tone_command[255] = "speaker-test -t sine -f ";
        char rest_of_tone_command[255] = " -l 1 & sleep .1 && kill -9 $!";
        strcat(tone_command, tone);
        strcat(tone_command, rest_of_tone_command);
        system(tone_command);
    }
}

int main() {
    while (1 == 1) {
        if (check_charging() == 1) {
            if (check_threshold() == 1) {
                send_battery_alert();
                while (check_charging() == 1) {
                    sleep(1);
                }
            }
        }
        sleep(interval);
    }
}
