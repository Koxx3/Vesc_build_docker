#!/bin/bash
sed -i '1s/^/#define DISABLE_HW_LIMITS\n/' ./bldc-503/conf_general.h

