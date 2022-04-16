#!/bin/bash
find . -name hw_* | xargs -n 1 -I {} sed -i 's/#define HW_NAME.*/#define HW_NAME "Rage_Mechanics"/g' {}
